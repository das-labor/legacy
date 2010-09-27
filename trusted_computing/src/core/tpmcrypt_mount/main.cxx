// Copyright (C) 2008, 2009 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//


#include <TpmCrypt.hxx>
#include <TpmCryptManager.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptSubject.hxx>
#include <TpmCryptConfigfile.hxx>
#include <TpmCryptMountHelper.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

const string helpOptionDesc	= "Shows the help string.\n";
#ifdef DEBUG
const string debugOptionDesc	= "Enables debugging output.\n";
#endif
const string mountOptionDesc	= "Mounts an encrypted volume with TpmCrypt.\n\tRequired parameters are:\n\t\tusers\n\t\tvolume\n\t\tpath\n\t\t[optionally] attach\n";
const string unmountOptionDesc	= "Unmounts an encrypted volume with TpmCrypt.\n\tRequired parameters are:\n\t\t--users\n\t\t--path\n";
const string userOptionDesc	= "Specifies the user, that performs an action.\n";
const string volumeOptionDesc	= "Specifies the source that should be decrypted\n";
const string pathOptionDesc	= "Specifies the destination, the decrypted volume is mounted to\n";
const string attachOptionDesc	= "Updates the source device, the encrypted volume is currently connected on, e.g., volume xyz is attached to /dev/sdf1\n";
const string configFileOptionDesc = "TpmCrypt configuration file.\n";

enum ReturnValues 
{
	SUCCESS = EXIT_SUCCESS,
	ERROR   = EXIT_FAILURE
};

////////////////////////////////
// EXTERN DECLARED VARIABLES!
string myKeyDirectory;
string myScriptDirectory;
string myServerSocket;
string myClientSocket;
string mySocketPass;
string UUIDpath;
string logfile;
string RNGDevice;
bool EnterpriseMode;
termios terminalBackup;
////////////////////////////////

void showUsage(CmdLineParser &parser)
{
                        cout << "========================================================================"  << endl;
                        cout << "                      TpmCrypt Mounthelper " << string(VERSION)             << endl;
                        cout << "            (c) 2008-2009 by Sirrix AG security technologies"              << endl;
                        cout << "------------------------------------------------------------------------"  << endl;

			cout << parser.getBriefHelp( "TpmCrypt Mounthelper" ) << endl;
			cout << parser.getLongHelp();
			cout << "========================================================================" << endl;
};

int main(int argc, const char *argv[]) 
{

    terminalBackup = getCurrentTerminalSettings();
    (void) signal(SIGINT, MountHelper::quitDaemon);
    (void) signal(SIGTERM, MountHelper::quitDaemon);


	try 
	{
		OptionList optionList;

#ifdef DEBUG
		Option<bool>    debugOption ( optionList, "debug", "d",  BaseOption::optional,   debugOptionDesc );
#endif
		Option<bool>	helpOption  ( optionList, "help",  "h",  BaseOption::single,     helpOptionDesc  );
                Option<bool>    mountOption      ( optionList, "mount",   "m",  BaseOption::multi,      mountOptionDesc   );
                Option<bool>    unmountOption      ( optionList, "unmount",   "um",  BaseOption::multi,      unmountOptionDesc   );
                // add categories to OptionList
                Option<string>  userOption      ( optionList, "user",   "u",  BaseOption::multi,      mountOptionDesc   );
                Option<string>  volumeOption      ( optionList, "volume",   "v",  BaseOption::multi,      mountOptionDesc   );
                Option<string>  pathOption      ( optionList, "path",   "p",  BaseOption::multi,      mountOptionDesc   );
                Option<string>  attachOption      ( optionList, "attach",   "a",  BaseOption::multi,      attachOptionDesc   );
                Option<string>  configFileOption( optionList, "configfile",  "c",  BaseOption::optional,     configFileOptionDesc  );
				

		CmdLineParser parser( optionList );
		parser.parseCmdLine( argc, argv );

                if (
                    (!(
                        unmountOption.getValue() |
                        mountOption.getValue() |
                        userOption.hasValue() |
                        volumeOption.hasValue() |
                        pathOption.hasValue() |
                        attachOption.hasValue()
                    )) ||
                        ( helpOption.getValue() )
                   )
		{
                    showUsage(parser);
                    return ERROR;
		}
                
                
                string myConfigfile;
                // Open our TpmCryptStorage Database
                if (configFileOption.hasValue())
                {
                    debug << "TpmCrypt configuration file: " + configFileOption.getValue() << endl;
                    // Check our configuration file
                    myConfigfile = configFileOption.getValue();
                }
                else
                {
                    debug << "No TpmCrypt configuration file given, using default: " + TpmCryptDefaultConfigfile << endl;
                    myConfigfile = TpmCryptDefaultConfigfile;
                }

                // Check our configuration file
                if (!verifyFilename(myConfigfile, false))
                    throw InvalidFilename("The supplied config filename was invalid!");

                // Check our configuration file
                TpmCryptConfigfile myTpmCryptConfig (myConfigfile);
                // get myKeyDirectory
                myKeyDirectory = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_KeyPathIdentifier));
                debug << "Key Directory: " << myKeyDirectory << endl;
                UUIDpath = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptUUIDPath));
                debug << "UUID Directory: " << UUIDpath << endl;
                logfile = myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptLogfile);
                debug << "Logfile: " << logfile << endl;
                RNGDevice = myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptRNGDevice);
                debug << "RNG-device (if no TPM available): " << RNGDevice << endl;
                string EnterpriseModeString = myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptEnterpriseMode);
                if (EnterpriseModeString == TpmCryptConfigfile_TpmCryptEnterpriseMode_TRUE)
                {
                    debug << "Running in enterprise mode!" << endl;
                    EnterpriseMode = true;
                }
                else
                {
                    debug << "Running in private mode!" << endl;
                    EnterpriseMode = false;
                }
                
#ifdef DEBUG
		if ( debugOption.getValue() )
		{
			cout << "Enabling debug output!" << endl;
			Debugger::getInstance().enableDebugging( true );
		}
#endif

                myServerSocket = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptServerSocketIdentifier));
                debug << "Server socket: " << myServerSocket << endl;
                myClientSocket = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptClientSocketIdentifier));
                debug << "Client socket: " << myClientSocket << endl;
                mySocketPass = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptSocketPass));
                debug << "Server-Client pass socket: " << mySocketPass << endl;

                if (( mountOption.getValue()))
                {
                    if ((userOption.hasValue()) && (volumeOption.hasValue()) && (pathOption.hasValue()))
                    {

                        if (attachOption.hasValue())
                        {
                            string newDevicePath = attachOption.getValue();
                            cout << "Attach option has been specified. Updating database for volume '" << volumeOption.getValue() << "' before proceeding." << endl;
                            // Create Volume database storage object
                            TpmCryptStorage myVolumeStorage( "VolumeDB", myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_VolumeDBIdentifier) );
                            if (!TpmCryptFindVolume(myVolumeStorage, volumeOption.getValue()))
                                throw UnknownVolume("Volume '" + volumeOption.getValue() + "' not found.");
                            // load volume
                            TpmCryptVolume myVolume(volumeOption.getValue(), myVolumeStorage);

                            // verifying supplied device name
                            if (!(fileExists(newDevicePath)))
                                throw InvalidFilename("Could not attach volume '" +  volumeOption.getValue() + "' to '" + newDevicePath + "'!");
                            if (myVolume.getMyVolumePath() == newDevicePath)
                            {
                                cout << "Volume '" << volumeOption.getValue() << "' already attached to: '" << myVolume.getMyVolumePath() << "'." << endl;
                            }
                            else
                            {
                                cout << "Volume '" << volumeOption.getValue() << "' was attached to: '" << myVolume.getMyVolumePath() << "'." << endl;
                                myVolume.updateDevicePath(newDevicePath);
                                cout << "Volume '" << volumeOption.getValue() << "' is now attached to: '" << myVolume.getMyVolumePath() << "'." << endl;
                            }
                        }
                        MountHelper( mountOption.getValue(), userOption.getValue(), volumeOption.getValue(), removeDelimiter(pathOption.getValue()) );
                        return SUCCESS;
                    }
                    else
                    {
                        cout << "Missing Parameter: To mount you must use the parameter -u for the user, -v for the volume to mount and -p for the mount destination." << endl;
                    }
                }
                else if (( unmountOption.getValue()))
                {
                    if ((pathOption.hasValue()))
                    {
                        MountHelper( !unmountOption.getValue(), userOption.getValue(), "", removeDelimiter(pathOption.getValue()) );
                        return SUCCESS;
                    }
                    else
                    {
                        cout << "Missing Parameter: To unmount you must use the parameter -u for the user that mounted the volume originaly and -p for the mount destination." << endl;
                    }
                }
                else
                {
                    showUsage(parser);
                    return ERROR;
                }
        }
	catch ( MissedOptionValue &e)
        {
                cout << "Error: Not enough arguments supplied! Please see '" << argv[0] << " --help' for usage information" << endl;
                return ERROR;
        }
	catch ( InvalidOptionValue &e)
        {
                cout << "Error: Invalid arguments supplied! Please see '" << argv[0] << " --help' for usage information" << endl;
                return ERROR;
        }
	catch ( ParseError &e)
        {
                cout << "Error: Invalid arguments supplied! Please see '" << argv[0] << " --help' for usage information" << endl;
                return ERROR;
        }
	catch ( std::exception &e ) {
                cout << "Error: " << e.what() << endl;
                unlink(myClientSocket.c_str());
                return ERROR;
        }

}


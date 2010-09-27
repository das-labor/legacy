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
#include <TpmCryptHelper.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptConfigfile.hxx>
#include <TpmCryptKeyprovider.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

const string helpOptionDesc	= "Shows the help string.\n";
#ifdef DEBUG
const string debugOptionDesc	= "Enables debugging output.\n";
#endif
const string foregroundOptionDesc	= "Runs the Keyprovider in the foreground/Don't send daemon into background\n";
const string verboseOptionDesc	= "Be verbose\n";
const string pruneOptionDesc	= "Delete all existing cache entries\n";
const string pathOptionDesc	= "Path to the configfile\n";
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
////////////////////////////////

int main(int argc, const char *argv[]) 
{
	try 
	{
		OptionList optionList;

#ifdef DEBUG
		Option<bool>    debugOption ( optionList, "debug", "d",  BaseOption::optional,   debugOptionDesc );
#endif
		Option<bool>	helpOption  ( optionList, "help",  "h",  BaseOption::single,     helpOptionDesc  );
                Option<bool>  foregroundOption      ( optionList, "foreground",   "f",  BaseOption::multi,      foregroundOptionDesc   );
                Option<bool>  pruneOption      ( optionList, "prune",   "p",  BaseOption::multi,      pruneOptionDesc   );
                Option<string>  configFileOption( optionList, "configfile",  "c",  BaseOption::optional,     configFileOptionDesc  );
                
		CmdLineParser parser( optionList );
		parser.parseCmdLine( argc, argv );

                if ( helpOption.getValue() )
		{
                        cout << "========================================================================"  << endl;
                        cout << "                        TpmCrypt Keyprovider" << string(VERSION)            << endl;
                        cout << "            (c) 2008-2009 by Sirrix AG security technologies"              << endl;
                        cout << "------------------------------------------------------------------------"  << endl;

			cout << parser.getBriefHelp( "TpmCrypt Keyprovider" ) << endl;
			cout << parser.getLongHelp();
			cout << "========================================================================" << endl;

			return SUCCESS;
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
#ifdef DEBUG
		if ( debugOption.getValue() )
		{
			cout << "Enabling debug output!" << endl;
			Debugger::getInstance().enableDebugging( true );
		}
#endif
                // get my directories
                myKeyDirectory = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_KeyPathIdentifier));
                debug << "Key Directory: " << myKeyDirectory << endl;
                myScriptDirectory = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptScriptPathIdentifier));
                debug << "Script Directory: " << myScriptDirectory << endl;
                myServerSocket = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptServerSocketIdentifier));
                debug << "Server socket: " << myServerSocket << endl;
                myClientSocket = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptClientSocketIdentifier));
                debug << "Client socket: " << myClientSocket << endl;
                mySocketPass = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptSocketPass));
                debug << "Server-Client pass socket: " << mySocketPass << endl;
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
                
                // Clearing database
                if (pruneOption.getValue())
                {
			debug << "Prune option was specified, clearing database..." << endl;
			TpmCryptStorage myDB( TpmCryptKeyproviderDB_Identifier, myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_KeyproviderDBIdentifier), true);
                }

                // if debug-option is set, start the daemon in foreground-mode
#ifdef DEBUG
		if ( debugOption.getValue() )
		{
                    cout << "Not going into background, since debug was enabled..." << endl;
                    TpmCryptKeyprovider(true, myTpmCryptConfig);
                    return SUCCESS;
		}
#endif
		// Start the daemon
                TpmCryptKeyprovider(foregroundOption.getValue(), myTpmCryptConfig);
                return SUCCESS;
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
                return ERROR;
        }

}

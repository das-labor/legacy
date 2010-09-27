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

using namespace std;
using namespace utils;
using namespace tpmcrypt;

const string helpOptionDesc	= "Shows the help string.\n";
const string verboseOptionDesc	= "Displays detailed user information.\n";
#ifdef DEBUG
const string debugOptionDesc	= "Enables debugging output.\n";
#endif
const string initOptionDesc	= "Initialize TpmCrypt.\n\tValid parameters are:\n\t\tcreateAdmin (ca)\n\t\tdeleteAdmin (da)\n\t\tlistAdmins (la)\n";
const string userOptionDesc	= "TpmCrypt user management.\n\tValid parameters are:\n\t\tcreateUser (cu)\n\t\timportUser (iu)\n\t\tdeleteUser (du)\n\t\tlistUsers (lu)\n\t\tchangeUserCredential (cuc)\n";
const string groupOptionDesc	= "TpmCrypt group management.\n\tValid parameters are:\n\t\tcreateGroup (cg)\n\t\tdeleteGroup (dg)\n\t\tlistGroups (lg)\n\t\t-----\n\t\taddSubjectToGroup (astg)\n\t\tdeleteSubjectFromGroup (dsfg)\n\t\tlistSubjectsInGroup (lsig)\n";
const string volumeOptionDesc	= "TpmCrypt volume management.\n\tValid parameters are:\n\t\tcreateVolume (cv)\n\t\timportVolume (iv)\n\t\tattachVolume (av)\n\t\tdeleteVolume (dv)\n\t\tlistVolumes (lv)\n\t\t-----\n\t\taddSubjectToVolume (astv)\n\t\tdeleteSubjectFromVolume (dsfv)\n\t\tlistSubjectsInVolume(lsiv)\n\t\t-----\n\t\taddGroupToVolume (agtv)\n\t\tdeleteGroupFromVolume(dgfv)\n\t\tlistGroupsInVolume(lgiv)\n\t\t-----\n\t\taddSSSToVolume (assstv)\n\t\tdeleteSSSFromVolume (dsssfv)\n\t\tlistSSSInVolume (lsssiv)\n";
const string platformOptionDesc	= "TpmCrypt platform management.\n\tValid parameters are:\n\t\tcreatePlatform (cp)\n\t\timportPlatform (ip)\n\t\tdeletePlatform (dp)\n\t\tlistPlatforms (lp)\n\t\texportPlatform (ep)\n";
const string tokenOptionDesc	= "TpmCrypt token management.\n\tValid parameters are:\n\t\tcreateToken (ct)\n\t\tdeleteToken (dt)\n\t\tlistToken (lt)\n";
const string configFileOptionDesc = "TpmCrypt configuration file.\n";
const string esdOptionDesc	= "TpmCrypt ESD management.\n\tValid parameters are:\n\t\tcreateESD (ce)\n\t\tshowESD (se)\n";

enum ReturnValues 
{
	SUCCESS = EXIT_SUCCESS,
	ERROR   = EXIT_FAILURE
};

////////////////////////////////
// EXTERN DECLARED VARIABLES!
string myKeyDirectory;
string myScriptDirectory;
string UUIDpath;
string logfile;
string RNGDevice;
bool EnterpriseMode;
bool printVerbose = false;
////////////////////////////////

int main(int argc, const char *argv[]) 
{
        cout << "========================================================================"<< endl;
	cout << "                     TpmCrypt Manager " << string(VERSION) << endl;
	cout << "            (c) 2008-2009 by Sirrix AG security technologies"<< endl;
	cout << "------------------------------------------------------------------------" << endl;
	try 
	{
		OptionList optionList;

#ifdef DEBUG
		Option<bool>    debugOption ( optionList, "debug", "d",  BaseOption::optional,   debugOptionDesc );
#endif
		Option<bool>	helpOption  ( optionList, "help",  "h",  BaseOption::single,     helpOptionDesc  );
		Option<bool>	verboseOption  ( optionList, "verbose",  "",  BaseOption::optional,     verboseOptionDesc  );
		Option<string>	configFileOption( optionList, "configfile",  "c",  BaseOption::optional,     configFileOptionDesc  );

                // add categories to OptionList
                Option<string>  initOption      ( optionList, "init",   "i",  BaseOption::multi,      initOptionDesc   );
                Option<string>  userOption      ( optionList, "user",   "u",  BaseOption::multi,      userOptionDesc   );
                Option<string>  groupOption     ( optionList, "group",  "g",  BaseOption::multi,      groupOptionDesc  );
                Option<string>  platformOption  ( optionList, "platform","p", BaseOption::multi,      platformOptionDesc );
                Option<string>  tokenOption     ( optionList, "token","t",   BaseOption::multi,      tokenOptionDesc );
                Option<string>  volumeOption    ( optionList, "volume", "v",  BaseOption::multi,      volumeOptionDesc );
                Option<string>  esdOption		( optionList, "esd", "e",  BaseOption::multi,      esdOptionDesc );

		CmdLineParser parser( optionList );
		parser.parseCmdLine( argc, argv );

                if (
                    (!(
                        initOption.hasValue() |
                        userOption.hasValue() |
                        groupOption.hasValue() |
                        platformOption.hasValue() |
                        tokenOption.hasValue() |
                        volumeOption.hasValue() |
                        esdOption.hasValue()
                    )) ||
                        ( helpOption.getValue() )
                   )
		{
			cout << parser.getBriefHelp( "TpmCrypt Manager" ) << endl;
			cout << parser.getLongHelp();
			cout << "========================================================================" << endl;

			return SUCCESS;
		}

#ifdef DEBUG
		if ( debugOption.getValue() ) 
		{
			cout << "Enabling debug output!" << endl;
			Debugger::getInstance().enableDebugging( true );
		}
#endif
                // enable verbose output for subjects
                if ( verboseOption.getValue() )
                    printVerbose = true;

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
                myScriptDirectory = removeDelimiter(myTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_TpmCryptScriptPathIdentifier));
                debug << "Script Directory: " << myScriptDirectory << endl;
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

                if ( initOption.hasValue() )
                {
                    TpmCryptInit( initOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }

		if ( userOption.hasValue() )
                {
                    TpmCryptUserManagement( userOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }

		if ( groupOption.hasValue() )
                {
                    TpmCryptGroupManagement( groupOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }

		if ( volumeOption.hasValue() )
                {
                    TpmCryptVolumeManagement( volumeOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }
        
        if ( esdOption.hasValue() )
                {
                    TpmCryptESDManagement( esdOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }         
                
		if ( platformOption.hasValue() )
                {
                    TpmCryptPlatformManagement( platformOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
                }
		if ( tokenOption.hasValue() )
                {
                    TpmCryptTokenManagement( tokenOption.getValue(), myTpmCryptConfig );
                    return SUCCESS;
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
	catch ( std::exception &e )
        {
                cout << "Error: " << e.what() << endl;
                return ERROR;
        }
        // explicitly close cryptolib
        return SUCCESS;
};

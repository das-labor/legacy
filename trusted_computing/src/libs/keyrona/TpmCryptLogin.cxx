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

/**
 * @file        TpmCryptLogin.cxx
 * @brief       Implementation of login methods.
 * @version     $Revision: 728 $
 * @date        Last modification at $Date: 2009-07-15 16:14:06 +0200 (Wed, 15 Jul 2009) $ by $Author: selhorst $
 */

// for shadow-file
#include <shadow.h>
// for crypt-routine
#include <crypt.h>
// to get uid
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <TpmCryptStorage.hxx>
#include <TpmCryptSubject.hxx>
#include <TpmCrypt.hxx>
#include <Debugging.hxx>
#include <TpmCryptExceptions.hxx>
#include <TpmCryptSearch.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// Value-Defintions of the different String values
#define RETURNKEY   10
#define BACKSPACE   127

//================================================================================
//
bool verifySelection()
{
    if (getStringFromUser("Are you sure? (type uppercase 'YES'): ") == "YES")
        return true;
    else
        return false;
}

//================================================================================
//
static string getString(string &myOutputText, bool passwordMode)
{
    termios before = getCurrentTerminalSettings();
    termios after  = getCurrentTerminalSettings();
    string userString;
    userString.clear();

    cout << myOutputText;
    // Disable echo for cin
    after.c_lflag &= (~ICANON); // Disable canonical mode, including line buffering
    after.c_lflag &= (~ECHO); // Don't echo characters on the screen (optional)
    tcsetattr (STDIN_FILENO, TCSANOW, &after); // Set the modified flags

    char temp=0;

    // wait till RETURN is pressed
    while (true)
    {
        cin.get(temp);

        switch(temp)
        {
            case BACKSPACE:
                    if(userString.length()>0)
                    {
                        userString = userString.substr(0, userString.length()-1);
                        cout << "\b \b";
                    }
                    
                    if(userString.length() == 0)
                    {
                        cout << "\r" << myOutputText;
                    }
                break;

            case RETURNKEY:
                // restore Terminal attributes
                restoreTerminalSettings(before);
                //  clear the remaining CR+LF
                cout << endl;
                return userString;

            default:
                userString += temp;
                if (passwordMode)
                    cout << "*";
                else
                    cout << temp;
                break;
        }
    }
};


//================================================================================
//
string getStringFromUser(string myOutputText)
{
    return getString(myOutputText,false);
};

//================================================================================
//
string getPassword(string myOutputText)
{
    return getString(myOutputText,true);
};
//================================================================================
//
termios getCurrentTerminalSettings()
{
    termios before;
    tcgetattr (STDIN_FILENO, &before); // fill 'before' with current termios values
    return before;
}

//================================================================================
//
void restoreTerminalSettings(termios &toRestore)
{
    tcsetattr (STDIN_FILENO, TCSANOW, &toRestore);
}

//================================================================================
//
string getPasswordVerified(string myOutputText)
{
    string pass1="a";
    string pass2="b";

    for (int i=TPMCRYPT_MAX_PASSWORD_TRIES; i>0; i--)
    {
        pass1 = getPassword(myOutputText);
        pass2 = getPassword("Please verify password: ");
        if (pass1 == pass2)
            return pass1;
        else
            cout << "Passwords didn't match" << endl;
    }
    throw InvalidPassword("Maximum password tries exceeded, giving up!");
};

//================================================================================
//
bool loginSuperUser()
{
    // Explicitly overwrite username and password variable!
    struct spwd *spw;
    int myUID = getuid();

    debug << "TpmCryptLogin: My userID = " << myUID << endl;

    cout << "The function you requested requires superuser authentication." << endl;
    if (myUID != 0)
        throw InvalidUID("TpmCrypt is not executed with UID 0. Please restart as user root.");

    if (! TPMCRYPT_ALWAYS_REQUIRE_LOGIN)
        return true;
    
    string  username=getStringFromUser("Please enter your superuser name (e.g., root): ");
    string  password=getPassword("Please enter password for user '" + username + "': ");

    if ((spw = getspnam(username.c_str())) == NULL)
        throw UnknownUser("unknown user '" + username + "'.");

    string  encryptedPassword = spw->sp_pwdp;
    string  calculatedPassword = crypt(password.c_str(),spw->sp_pwdp);

    if (encryptedPassword != calculatedPassword)
        throw AuthenticationError("authentication for user '" + username + "' failed!");

    return true;
};

//================================================================================
//
TpmCryptSubject* getCurrentAdmin(TpmCryptStorage &mySubjectStorage)
{
    // loading admin
    vector<string> myAdmins = TpmCryptFindAllAdmins(mySubjectStorage);
    if (!myAdmins.size())
        throw NoAdminAvailable("No admins found, please add an administrator first!");

    string existingAdmin;
    if (myAdmins.size() == 1)
    {
        existingAdmin = myAdmins[0];
        cout << "Only one administrator available, selecting admin '" << existingAdmin << "'" << endl;
    }
    else
    {
        existingAdmin = selectFromStringVector(myAdmins, "Number:\t\tAdmin:", "Please select an administrator to continue:");
    }

    if (!TpmCryptFindAdmin(mySubjectStorage, existingAdmin))
        throw UnknownSubject("Subject '" + existingAdmin + "' is not an admin.");

    TpmCryptSubject *myExistingAdmin = new TpmCryptSubject(existingAdmin, mySubjectStorage);
    return myExistingAdmin;
};

//================================================================================
//
bool loginAdmin(TpmCryptStorage &mySubjectStorage)
{
    int myUID = getuid();
    debug << "TpmCryptLogin: My userID = " << myUID << endl;

    // loading admin
    vector<string> myAdmins = TpmCryptFindAllAdmins(mySubjectStorage);
    if (!myAdmins.size())
        throw NoAdminAvailable("No admins found, please add an administrator first!");

    if (myUID == 0)
    {
        cout << "Running with superuser privileges, no admin authentication required" << endl;
        return true;
    }

    // load admin
    TpmCryptSubject *myAdmin = getCurrentAdmin(mySubjectStorage);
    string myPassword = getPassword("Please enter the according password for admin '" + myAdmin->getMySubjectName() + "' (" + myAdmin->getMySubjectIDString() + "): ");
    return myAdmin->verifyPassword(myPassword);
};


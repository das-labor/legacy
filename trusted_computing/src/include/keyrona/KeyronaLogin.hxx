// Copyright (C) 2008, 2009 by Sirrix AG security technologies
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
 * @file        KeyronaLogin.hxx
 * @brief       Implementation of login methods.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONALOGIN_HXX
#define	_KEYRONALOGIN_HXX

#include <KeyronaStorage.hxx>

/*!
 *  @brief gets the current terminal settings
 *  @return termios, contains file descriptor for the current terminal
 */
termios getCurrentTerminalSettings();

/*!
 *  @brief restores the terminal, to avoid broken terminals
 *  @param &toRestore, termios, contains the current setting
 */
void restoreTerminalSettings(termios &toRestore);

/*!
 *  @brief reads an input string from the user
 *  @param myOutputText, string, contains a message for the user
 *  @return, string, contains the input of the user
 */
std::string getStringFromUser(std::string myOutputText);

/*!
 *  @brief reads an input string from the user, the string is replaced through stars
 *  @param myOutputText, string, contains a message for the user
 *  @return, string, contains the input of the user
 */
std::string getPassword(std::string myOutputText);

/*!
 *  @brief reads an input string from the user twice and verifies, if the first input equals the second input
 *  @param myOutputText, string, contains a message for the user
 *  @return, string, contains one input of the user
 */
std::string getPasswordVerified(std::string myOutputText);

/*!
 *  @brief login method for the root super user
 *  @return determines whether or not the login was successful 
 */
bool loginSuperUser();

/*!
 *  @brief login method for an Keyrona administrator
 *  @param &mySubjectStorage, KeyronaStorage, the database with the admin credentials
 *  @return determines whether or not the login was successful 
 */
bool loginAdmin(KeyronaStorage &mySubjectStorage);

/*!
 *  @brief asks the user about the correctness of the selection ( upper case YES )
 *  @return determines how the user responded
 */
bool verifySelection();

#endif	/* _KEYRONALOGIN_HXX */


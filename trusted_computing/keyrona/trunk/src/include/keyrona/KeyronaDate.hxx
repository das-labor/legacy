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
 * @file        KeyronaDate.hxx
 * @brief       Implementation of the Keyrona date class.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONADATE_HXX
#define	_KEYRONADATE_HXX

#include <Keyrona.hxx>

namespace keyrona
{
    class KeyronaDate
    {
        public:
            /*!
             *  @brief get current system time
             */
            KeyronaDate();

            /*!
             *  @brief generate an invalid date
             */
            KeyronaDate(bool invalid);
            
            /*!
             *  @brief get date directly by parameters
             *  @param year, UInt32, contains the year of the date
             *  @param month, UInt32, contains the month of the date
             *  @param day, UInt32, contains the day of the date
             *  @param hour, UInt32, contains the hour of the date
             *  @param minute, UInt32, contains the minute of the date
             *  @param second, UInt32, contains the second of the date
             */
            KeyronaDate(UInt32 year, UInt32 month, UInt32 day, UInt32 hour, UInt32 minute, UInt32 second);
            
            /*!
             *  @brief get date directly by vector
             *  @param date, vector of UInt32, contains the date
             */      
            KeyronaDate(vector<UInt32> date);
            
            /*!
             *  @brief destructor
             */
            ~KeyronaDate();
            
            /*!
             *  @brief checks if a date is valid
             *  @return boolean, determines whether or not the date is valid
             */ 
            bool isValid() { return valid; };
            
            /*!
             *  @brief returns the date as vector of UInt32
             *  @return vector of UInt32, contains the date
             */ 
            vector<UInt32> getDate();
            
            /*!
             *  @brief returns the date in string format
             *  @return string, contains the date
             */ 
            string getDateString();
            
            /*!
             *  @brief sets the date to a given date
             *  @param year, UInt32, contains the year of the date
             *  @param month, UInt32, contains the month of the date
             *  @param day, UInt32, contains the day of the date
             *  @param hour, UInt32, contains the hour of the date
             *  @param minute, UInt32, contains the minute of the date
             *  @param second, UInt32, contains the second of the date
             *  @return boolean determines whether or not the operation was successful
             */ 
            bool setDate(UInt32 year, UInt32 month, UInt32 day, UInt32 hour, UInt32 minute, UInt32 second);
            
            /*!
             *  @brief sets the date to a given date
             *  @param date, vector of UInt32, contains the date
             *  @return boolean determines whether or not the operation was successful
             */ 
            bool setDate(vector<UInt32> date);
            
            /*!
             *  @brief prints the date to the screen
             */
            void printDate();

        private:
            /*!
             *  @brief checks if the date is valid
             *  @return boolean determines whether or not the operation was successful
             */
            bool checkDate();

            vector<UInt32> myDate;
            bool          valid;
    };
};

#endif	/* _KEYRONADATE_HXX */


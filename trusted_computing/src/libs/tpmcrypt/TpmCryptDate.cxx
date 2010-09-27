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
 * @file        TpmCryptDate.hxx
 * @brief       Implementation of the TpmCrypt date class.
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#include <TpmCryptDate.hxx>

//================================================================================
//
// invalid date...
TpmCryptDate::TpmCryptDate(bool invalid) :
myDate (NULL),
valid (false)
{};

//================================================================================
//
// retrieving current system time
TpmCryptDate::TpmCryptDate() :
myDate (NULL),
valid (false)
{
    time_t myTime;
    tm *myCurrentSystemTime;
    myTime = time(0);
    myCurrentSystemTime = localtime(&myTime);

    setDate(myCurrentSystemTime->tm_year + 1900,
            myCurrentSystemTime->tm_mon + 1,
            myCurrentSystemTime->tm_mday,
            myCurrentSystemTime->tm_hour,
            myCurrentSystemTime->tm_min,
            myCurrentSystemTime->tm_sec);
};

//================================================================================
//
TpmCryptDate::TpmCryptDate(UInt32 year, UInt32 month, UInt32 day, UInt32 hour, UInt32 minute, UInt32 second) :
myDate (NULL),
valid (false)
{
    setDate(year, month, day, hour, minute, second);
};

//================================================================================
//
TpmCryptDate::TpmCryptDate(vector<UInt32> date) :
myDate (NULL),
valid (false)
{
    setDate(date);
};

//================================================================================
//
TpmCryptDate::~TpmCryptDate()
{
    //empty
};

//================================================================================
//
bool TpmCryptDate::setDate(UInt32 year, UInt32 month, UInt32 day, UInt32 hour, UInt32 minute, UInt32 second)
{
    valid = false;
    myDate.push_back(year);
    myDate.push_back(month);
    myDate.push_back(day);
    myDate.push_back(hour);
    myDate.push_back(minute);
    myDate.push_back(second);
    return checkDate();
};

//================================================================================
//
bool TpmCryptDate::setDate(vector<UInt32> date)
{
    valid = false;
    myDate = date;
    return checkDate();
};

//================================================================================
//
vector<UInt32> TpmCryptDate::getDate()
{
    if (!valid)
        myDate.clear();
    return myDate;
};

//================================================================================
//
bool TpmCryptDate::checkDate()
{
    if (myDate.size() < 6)
        throw InvalidEntryValue("TpmCryptDate: Not enough entries.");

    UInt32         temp;
    vector<UInt32> tempDate = myDate;
    // get and check seconds
    temp = tempDate.back();
    tempDate.pop_back();
    if (!(temp < 60))
        throw InvalidEntryValue("TpmCryptDate: Seconds were bad.");

    // get and check minutes
    temp = tempDate.back();
    tempDate.pop_back();
    if (!(temp < 60))
        throw InvalidEntryValue("TpmCryptDate: Minutes were bad.");

    // get and check hours
    temp = tempDate.back();
    tempDate.pop_back();
    if (!(temp < 24))
        throw InvalidEntryValue("TpmCryptDate: Hours were bad.");

    // get and check days
    temp = tempDate.back();
    tempDate.pop_back();
    if (!((temp > 0) & (temp < 32)))
        throw InvalidEntryValue("TpmCryptDate: Days were bad.");

    // get and check months
    temp = tempDate.back();
    tempDate.pop_back();
    if (!((temp > 0) & (temp < 13)))
        throw InvalidEntryValue("TpmCryptDate: Months were bad.");

    // get and check year
    temp = tempDate.back();
    tempDate.pop_back();
    if (!((temp > MIN_YEAR) & (temp < MAX_YEAR)))
        throw InvalidEntryValue("TpmCryptDate: Years were bad.");

    valid = true;
    return valid;
};

//================================================================================
//
string TpmCryptDate::getDateString()
{
    if (!valid)
        return "";

    vector<UInt32> tempDate = myDate;
    UInt32         seconds;
    UInt32         minutes;
    UInt32         hours;
    UInt32         days;
    UInt32         month;
    UInt32         year;

    seconds = tempDate.back();
    tempDate.pop_back();

    minutes = tempDate.back();
    tempDate.pop_back();

    hours = tempDate.back();
    tempDate.pop_back();

    days = tempDate.back();
    tempDate.pop_back();

    month = tempDate.back();
    tempDate.pop_back();

    year = tempDate.back();
    tempDate.pop_back();

    char currentDate[25];
    memset(currentDate, 0, 25);
    sprintf(currentDate, "%02d.%02d.%04d, %02d:%02d:%02d", days, month, year, hours, minutes, seconds);
    return currentDate;
};
//================================================================================
//
void TpmCryptDate::printDate()
{
    if (!valid)
        return;

    string date = getDateString();
    cout << date << endl;
};

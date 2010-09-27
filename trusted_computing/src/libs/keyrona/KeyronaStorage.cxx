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
 * @file        KeyronaStorage.cxx
 * @brief       Implementation of the Keyrona storage service.
 * @version     $Revision: 549 $
 * @date        Last modification at $Date: 2009-05-07 10:27:30 +0200 (Thu, 07 May 2009) $ by $Author: selhorst $
 */

#include <KeyronaStorage.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

//================================================================================
//
KeyronaStorage::KeyronaStorage( const string &StorageName, const string &StorageFile ):
    myStorageName (StorageName),
    myStorageFile (StorageFile),
    mySection (""),
    mySubSection (""),
    myDB (NULL),
    mySections (NULL),
    SectionIterator (NULL),
    mySectionPositions (NULL),
    SectionPositionsIterator (NULL),
    mySubSections (NULL),
    SubSectionIterator (NULL),
    mySubSectionPositions (NULL),
    SubSectionPositionsIterator (NULL),
    myEntries (NULL),
    EntryIterator (NULL),
    myEntryPositions (NULL),
    EntryPositionsIterator (NULL)
{
    if (myStorageFile.empty())
        throw NoFilename("KeyronaStorage: No valid database filename submitted! Please check your configuration file!");

    openFile();
    queryDBforSections();
};

//================================================================================
//
KeyronaStorage::KeyronaStorage( const string &StorageName, const string &StorageFile, bool clear):
    myStorageName (StorageName),
    myStorageFile (StorageFile),
    mySection (""),
    mySubSection (""),
    myDB (NULL),
    mySections (NULL),
    SectionIterator (NULL),
    mySectionPositions (NULL),
    SectionPositionsIterator (NULL),
    mySubSections (NULL),
    SubSectionIterator (NULL),
    mySubSectionPositions (NULL),
    SubSectionPositionsIterator (NULL),
    myEntries (NULL),
    EntryIterator (NULL),
    myEntryPositions (NULL),
    EntryPositionsIterator (NULL)
{
    if (myStorageFile.empty())
        throw NoFilename("KeyronaStorage: No valid database filename submitted! Please check your configuration file!");

    openFile();
    if (clear)
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage: clearing database!" << endl;
#endif
        truncateFile(0);
    }
    else
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage: not clearing database!" << endl;
#endif

    queryDBforSections();
};


//================================================================================
//
KeyronaStorage::~KeyronaStorage()
{
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|Destructor()]: Closing database '" + myStorageFile + "'." << endl;
#endif
    if (myDB.is_open())
        myDB.close();
};

//================================================================================
//
bool KeyronaStorage::openFile()
{
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|openFile()]: Opening database '" + myStorageFile + "'." << endl;
#endif

    myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::ate);

    if (!myDB.is_open())
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|openFile()]: : Database file '" + myStorageFile + "' does not exist." << endl;
        debug << "KeyronaStorage[" + myStorageName + "|openFile()]: : Creating database file..." << endl;
#endif
        myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::app);
        if (!myDB.is_open())
            throw NoOpenDB("KeyronaStorage: Database '" + myStorageFile + "' could not be opened!\nAre path and filename correct and do you have enough access rights?");
        myDB.close();
        myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::ate);
    }

    if (!myDB.is_open())
        throw NoOpenDB("KeyronaStorage: Database '" + myStorageFile + "' could not be opened!\nAre path and filename correct and do you have enough access rights?");

    int myUID = getuid();
    if (!myUID)
        assignFilePermission(myStorageFile, DBFILEPERMISSION);

    return true;
};

//================================================================================
//
bool KeyronaStorage::truncateFile( UInt32 truncatePosition )
{

    UInt8 * memblock;
    UInt32 myDBSize;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|truncateFile()]: : Truncating file '" + myStorageFile + "' at position: " << truncatePosition << endl;
#endif

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();

    if (myDBSize < truncatePosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "]: Database error during truncation...");

    memblock = new UInt8[truncatePosition];
    memset (memblock, 0, truncatePosition);

    // read the file
    myDB.clear();
    myDB.seekg(0, ios::beg);
    myDB.read(reinterpret_cast <char*>(memblock), truncatePosition);
    myDB.close();

    // open it empty...
    myDB.open(myStorageFile.c_str(), ios::out |  ios::trunc);
    myDB.write(reinterpret_cast <char*>(memblock), truncatePosition);
    myDB.close();

    delete[] memblock;

    return openFile();
};

//================================================================================
//
void KeyronaStorage::selectSection(const string &section)
{
#ifdef KEYRONA_STORAGE_DEBUG
    if (!mySection.empty())
        debug << "KeyronaStorage[" + myStorageName + "|selectSection()]: My current section is: " << mySection << endl;
    else
        debug << "KeyronaStorage[" + myStorageName + "|selectSection()]: Currently no section selected!" << endl;
#endif

    if (!findSection(section))
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|selectSection()]: Creating new section '" + section + "'." << endl;
#endif
        addSection(section);
        if (!findSection(section))
            throw SectionNotFound("KeyronaStorage[" + myStorageName + "|selectSection()]: Section '" + section + "' has not been added correctly.");
    };
    
    mySection = section;
    mySubSection.clear();
    mySubSections.clear();
    mySubSectionPositions.clear();
    myEntries.clear();
    myEntryPositions.clear();

#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|selectSection()]: Section '" << mySection << "' selected!" << endl;
#endif
    queryDBforSubSections();

};

//================================================================================
//
void KeyronaStorage::selectSubSection(const string &subSection)
{
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|selectSubSection()]: No Section has been selected. Section has to be set prior to calls to selectSubSection().");

#ifdef KEYRONA_STORAGE_DEBUG
    if (!mySubSection.empty())
        debug << "KeyronaStorage[" + myStorageName + "|selectSubSection()]: My current subsection is: " << mySubSection << endl;
    else
        debug << "KeyronaStorage[" + myStorageName + "|selectSubSection()]: Currently no subsection selected!" << endl;
#endif

    if (!findSubSection(subSection))
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|selectSubSection()]: Creating new subsection '" + subSection + "'." << endl;
#endif
        addSubSection(subSection);
        if (!findSubSection(subSection))
            throw SubSectionNotFound("KeyronaStorage[" + myStorageName + "|selectSubSection()]: Subsection '" + subSection + "' has not been added correctly.");
    };

    mySubSection = subSection;
    myEntries.clear();
    myEntryPositions.clear();

#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|selectSubSection()]: Subsection '" << mySubSection << "' selected!" << endl;
#endif
    queryDBforEntries();
};

//================================================================================
//
void KeyronaStorage::queryDBforSections(void)
{
    string line;
    UInt32 myCurrentPosition;

    // clear my section and its according position vector
    mySections.clear();
    mySectionPositions.clear();

    // rewind file to the beginning
    myDB.clear();
    myDB.seekg(0, ios::beg);
    while (!myDB.eof())
    {
        // parse file for the first occurance of <Section>
        while (line.compare(0, KeyronaDatabase_SectionIdentifier_Start.length(), KeyronaDatabase_SectionIdentifier_Start))
        {
            myCurrentPosition = myDB.tellg();
            getline(myDB, line);
            // get my current position for later reuse
            if (myDB.eof())
                break;
        }

        // find the beginning of a section
        if (!line.compare(0, KeyronaDatabase_SectionIdentifier_Start.length(), KeyronaDatabase_SectionIdentifier_Start))
        {
            // the next line should have our SectionName
            getline(myDB, line);
            if (!line.compare(0, KeyronaDatabase_SectionNameIdentifier.length(), KeyronaDatabase_SectionNameIdentifier))
            {
                mySections.push_back(extractQuotationMarks(line));
                mySectionPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidSectionName("KeyronaStorage[" + myStorageName + "|queryDBforSections()]: Section does not have a valid SectionName.");

            // Now, since we found the a valid SectionName, get all lines until section closes
            while (line.compare(0, KeyronaDatabase_SectionIdentifier_Stop.length(), KeyronaDatabase_SectionIdentifier_Stop))
            {
                // if EOF reached while section still open
                if (myDB.eof())
                    throw SectionDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforSections()]: Section '" + mySections.back() + "' does not close!.");
                
                // if section still open and a new section opens
                if (!line.compare(0, KeyronaDatabase_SectionIdentifier_Start.length(), KeyronaDatabase_SectionIdentifier_Start))
                    throw SectionDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforSections()]: Section '" + mySections.back() + "' does not close!.");

                getline(myDB, line);
            }
        }
    }
#ifdef KEYRONA_STORAGE_DEBUG
    // Now print out the found sections
    if (mySections.size())
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforSections()]: Identified " << mySections.size() << " sections:" << endl;
        SectionIterator = mySections.begin();
        SectionPositionsIterator = mySectionPositions.begin();
        while ( SectionIterator != mySections.end())
        {
            debug << "KeyronaStorage[" + myStorageName + "|queryDBforSections()]: " << *(SectionIterator) << " @ " << *(SectionPositionsIterator) << endl;
            SectionIterator++;
            SectionPositionsIterator++;
        }
    }
    else
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforSections()]: No sections identified!" << endl;
    }
#endif
};

//================================================================================
//
void KeyronaStorage::queryDBforSubSections(void)
{
    string line;
    UInt32 myCurrentPosition;

    // clear my section and its according position vector
    mySubSections.clear();
    mySubSectionPositions.clear();

    // seek to the beginning of my section
    myDB.clear();
    myCurrentPosition = *(SectionPositionsIterator);
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: Seeking to position " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition,ios::beg);

    // read until section closes
    while (line.compare(0, KeyronaDatabase_SectionIdentifier_Stop.length(), KeyronaDatabase_SectionIdentifier_Stop))
    {
        // get my current position for later reuse
        myCurrentPosition = myDB.tellg();
        getline(myDB, line);
        // find the beginning of a subsection
        if (!line.compare(0, KeyronaDatabase_SubSectionIdentifier_Start.length(), KeyronaDatabase_SubSectionIdentifier_Start))
        {
            // the next line should have our SubSectionName
            getline(myDB, line);
            if (!line.compare(0, KeyronaDatabase_SubSectionNameIdentifier.length(), KeyronaDatabase_SubSectionNameIdentifier))
            {
                mySubSections.push_back(extractQuotationMarks(line));
                mySubSectionPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidSubSectionName("KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: Subsection does not have a valid SubsectionName.");

            // Now, since we found the a valid SubSectionName, get all lines until section closes
            while (line.compare(0, KeyronaDatabase_SubSectionIdentifier_Stop.length(), KeyronaDatabase_SubSectionIdentifier_Stop))
            {
                // if EOF reached while subsection still open
                if (myDB.eof())
                    throw SubSectionDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: Subsection '" + mySubSections.back() + "' does not close!.");
                // if subsection still open and a new subsection opens
                if (!line.compare(0, KeyronaDatabase_SubSectionIdentifier_Start.length(), KeyronaDatabase_SubSectionIdentifier_Start))
                    throw SubSectionDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: SubSection '" + mySubSections.back() + "' does not close!.");

                getline(myDB, line);
            }
        }

    }
#ifdef KEYRONA_STORAGE_DEBUG
    // Now print out the found subsections
    if (mySubSections.size())
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: Identified " << mySubSections.size() << " subsections:" << endl;
        SubSectionIterator = mySubSections.begin();
        SubSectionPositionsIterator = mySubSectionPositions.begin();
        while ( SubSectionIterator != mySubSections.end())
        {
            debug << "KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: " << *(SubSectionIterator) << " @ " << *(SubSectionPositionsIterator) << endl;
            SubSectionIterator++;
            SubSectionPositionsIterator++;
        }
    }
    else
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforSubSections()]: No subsections identified!" << endl;
    }
#endif
};

//================================================================================
//
void KeyronaStorage::queryDBforEntries(void)
{
    string line;
    UInt32 myCurrentPosition;

    // clear my entries and its according position vector
    myEntries.clear();
    myEntryPositions.clear();

    // seek to the beginning of my section
    myDB.clear();
    myCurrentPosition = *(SubSectionPositionsIterator);
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Seeking to position " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition,ios::beg);

    // read until subsection closes
    while (line.compare(0, KeyronaDatabase_SubSectionIdentifier_Stop.length(), KeyronaDatabase_SubSectionIdentifier_Stop))
    {
        // get my current position for later reuse
        myCurrentPosition = myDB.tellg();
        getline(myDB, line);
        // find the beginning of an entry
        if (!line.compare(0, KeyronaDatabase_EntryIdentifier_Start.length(), KeyronaDatabase_EntryIdentifier_Start))
        {
            // the next line should have our EntryName
            getline(myDB, line);
            if (!line.compare(0, KeyronaDatabase_EntryNameIdentifier.length(), KeyronaDatabase_EntryNameIdentifier))
            {
                myEntries.push_back(extractQuotationMarks(line));
                myEntryPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Entry does not have a valid EntryName.");

            // Now, since we found the a valid EntryName, get all lines until Entry closes
            while (line.compare(0, KeyronaDatabase_EntryIdentifier_Stop.length(), KeyronaDatabase_EntryIdentifier_Stop))
            {
                // if EOF reached while subsection still open
                if (myDB.eof())
                    throw EntryDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");
                // if entry still open and a new subsection opens
                if (!line.compare(0, KeyronaDatabase_SubSectionIdentifier_Start.length(), KeyronaDatabase_SubSectionIdentifier_Start))
                    throw EntryDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");
                // if entry still open and a new entry opens
                if (!line.compare(0, KeyronaDatabase_EntryIdentifier_Start.length(), KeyronaDatabase_EntryIdentifier_Start))
                    throw EntryDoesNotClose("KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");

                getline(myDB, line);
            }
        }
    }
#ifdef KEYRONA_STORAGE_DEBUG
    // Now print out the found entries
    if (myEntries.size())
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: Identified " << myEntries.size() << " entries:" << endl;
        EntryIterator = myEntries.begin();
        EntryPositionsIterator = myEntryPositions.begin();
        while ( EntryPositionsIterator != myEntryPositions.end())
        {
            debug << "KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: " << *(EntryIterator) << " @ " << *(EntryPositionsIterator) << endl;
            EntryIterator++;
            EntryPositionsIterator++;
        }
    }
    else
    {
        debug << "KeyronaStorage[" + myStorageName + "|queryDBforEntries()]: No entries identified!" << endl;
    }
#endif
};

//================================================================================
//
bool KeyronaStorage::findSection(const string &section)
{
    if (section.empty())
        throw InvalidSectionName("KeyronaStorage[" + myStorageName + "|findSection()]: Invalid section name.");
    queryDBforSections();

    SectionIterator = mySections.begin();
    SectionPositionsIterator = mySectionPositions.begin();
    int entry = 0;

    while ( SectionIterator != mySections.end())
    {
        entry++;
        if (section == *(SectionIterator))
        {
#ifdef KEYRONA_STORAGE_DEBUG
            debug << "KeyronaStorage[" + myStorageName + "|findSection()]: Section '" + section + "' found @ " << *(SectionPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        SectionIterator++;
        SectionPositionsIterator++;
    }
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|findSection()]: Section '" + section + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool KeyronaStorage::findSubSection(const string &subsection)
{
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|findSubSection()]: No Section has been selected.");
    if (subsection.empty())
        throw InvalidSubSectionName("KeyronaStorage[" + myStorageName + "|findSubSection()]: Invalid subsection name.");
    queryDBforSubSections();

    SubSectionIterator = mySubSections.begin();
    SubSectionPositionsIterator = mySubSectionPositions.begin();
    int entry = 0;

    while ( SubSectionIterator != mySubSections.end())
    {
        entry++;
        if (subsection == *(SubSectionIterator))
        {
#ifdef KEYRONA_STORAGE_DEBUG
            debug << "KeyronaStorage[" + myStorageName + "|findSubSection()]: Subsection '" + subsection + "' found @ " << *(SubSectionPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        SubSectionIterator++;
        SubSectionPositionsIterator++;
    }
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|findSubSection()]: Subsection '" + subsection + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool KeyronaStorage::findEntry(const string &entryname)
{
    if (entryname.empty())
        throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|findEntry()]: Invalid entry name.");
    queryDBforEntries();

    EntryIterator = myEntries.begin();
    EntryPositionsIterator = myEntryPositions.begin();
    int entry = 0;

    while ( EntryIterator != myEntries.end())
    {
        entry++;
        if (entryname == *(EntryIterator))
        {
#ifdef KEYRONA_STORAGE_DEBUG
            debug << "KeyronaStorage[" + myStorageName + "|findEntry()]: Entry '" + entryname + "' found @ " << *(EntryPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        EntryIterator++;
        EntryPositionsIterator++;
    }
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|findEntry()]: Entry '" + entryname + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool KeyronaStorage::addSection(const string &section)
{
    UInt32 myCurrentPosition;
    string line;

    // Seeking to last <Section>-entry
    if (mySections.size())
    {
        myCurrentPosition = mySectionPositions[(mySections.size()-1)];
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|addSection()]: Previously created sections exist, adding section below pos: " << myCurrentPosition << endl;
#endif
        myDB.clear();
        myDB.seekg(myCurrentPosition, ios::beg);

        // parse file for the last occurance of </Section>
        while (line.compare(0, KeyronaDatabase_SectionIdentifier_Stop.length(), KeyronaDatabase_SectionIdentifier_Stop))
        {
            getline(myDB, line);
            myCurrentPosition = myDB.tellg();
        }
    }
    else
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|addSection()]: No existing sections, adding section on top..." << endl;
#endif
        myCurrentPosition = 0;
    }
        myDB.clear();
        myDB.seekg(myCurrentPosition, ios::beg);
        myDB.seekp(myCurrentPosition, ios::beg);

        // Adding section
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|addSection()]: Adding section '" + section + "' to position: " << myDB.tellg() << endl;
#endif
        myDB << KeyronaDatabase_SectionIdentifier_Start << endl;
        myDB << KeyronaDatabase_SectionNameIdentifier << "=\"" << section << "\"" << endl;
        myDB << KeyronaDatabase_SectionIdentifier_Stop << endl;

        myDB.clear();
        myDB.seekg(0, ios::end);
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|addSection()]: Current DB filesize: " << myDB.tellg() << endl;
#endif
        queryDBforSections();
        return true;
};

//================================================================================
//
bool KeyronaStorage::deleteSection(const string &section)
{
    UInt32 myCurrentPosition;
    UInt32 myDeletionPosition;
    UInt32 myDBSize;
    UInt32 bytesToCopy;
    string line;
    UInt8 * memblock;

    // check if subsection exists, if not, we're fine...
    if (!findSection(section))
        return true;

    if (section == mySection)
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Deleting currently selected section." << endl;
#endif
        mySection.clear();
        mySubSection.clear();
    }

    // get my current subsection position
    myCurrentPosition = *(SectionPositionsIterator);
    myDeletionPosition = *(SectionPositionsIterator);

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <Section>...
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SectionIdentifier_Start.length(), KeyronaDatabase_SectionIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|deleteSection()]: Inconsistent Database: Expected " + KeyronaDatabase_SectionIdentifier_Start);

    // now read everything until </Section>
    while (line.compare(0, KeyronaDatabase_SectionIdentifier_Stop.length(), KeyronaDatabase_SectionIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Current write position: " << myDB.tellp() << endl;
#endif
    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting section
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Deleting section '" + section + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Deleting finished..." << endl;
#endif

    // Truncate file ...
    myCurrentPosition = myDB.tellg();
    truncateFile(myCurrentPosition);

    // Finally, re-new our pointers and vectors
    queryDBforSections();
    return true;
};

//================================================================================
//
bool KeyronaStorage::addSubSection(const string &subsection)
{
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|addSubSection()]: No Section has been selected.");

    UInt32 myCurrentPosition;
    UInt32 myDBSize;
    UInt32 bytesToCopy;
    string line;
    UInt8 * memblock;

    // get my current section position
    myCurrentPosition = *(SectionPositionsIterator);

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to insertion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip two lines for <Section> and SectionName...
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SectionIdentifier_Start.length(), KeyronaDatabase_SectionIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: Expected " + KeyronaDatabase_SectionIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SectionNameIdentifier.length(), KeyronaDatabase_SectionNameIdentifier))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: Expected " + KeyronaDatabase_SectionNameIdentifier);

    myCurrentPosition = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to insertion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // Inserting new section
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Adding subsection '" + subsection + "' to position: " << myDB.tellg() << endl;
#endif
    myDB << KeyronaDatabase_SubSectionIdentifier_Start << endl;
    myDB << KeyronaDatabase_SubSectionNameIdentifier << "=\"" << subsection << "\"" << endl;
    myDB << KeyronaDatabase_SubSectionIdentifier_Stop << endl;

    // Copying saved data back into file
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|addSubSection()]: Writing " << bytesToCopy << " bytes back to DB..." << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

    // Finally, re-new our pointers and vectors
    queryDBforSubSections();
    return true;
};

//================================================================================
//
bool KeyronaStorage::deleteSubSection(const string &subsection)
{
    UInt32 myCurrentPosition;
    UInt32 myDeletionPosition;
    UInt32 myDBSize;
    UInt32 bytesToCopy;
    string line;
    UInt8 * memblock;

    // check if subsection exists, if not, we're fine...
    if (!findSubSection(subsection))
        return true;

    if (subsection == mySubSection)
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|deleteSection()]: Deleting currently selected subsection." << endl;
#endif
        mySubSection.clear();
    }

    // get my current subsection position
    myCurrentPosition = *(SubSectionPositionsIterator);
    myDeletionPosition = *(SubSectionPositionsIterator);

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <SubSection>...
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SubSectionIdentifier_Start.length(), KeyronaDatabase_SubSectionIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Inconsistent Database: Expected " + KeyronaDatabase_SubSectionIdentifier_Start);

    // now read everything until </SubSection>
    while (line.compare(0, KeyronaDatabase_SubSectionIdentifier_Stop.length(), KeyronaDatabase_SubSectionIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting subsection
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Deleting subsection '" + subsection + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteSubSection()]: Deleting finished..." << endl;
#endif

    // Truncate file ...
    myCurrentPosition = myDB.tellg();
    truncateFile(myCurrentPosition);

    // Finally, re-new our pointers and vectors
    queryDBforSubSections();
    return true;
};

//================================================================================
//
bool KeyronaStorage::setEntry(const string &entry, const string &value)
{
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|setEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("KeyronaStorage[" + myStorageName + "|setEntry()]: No Subsection has been selected.");
    if (value.empty())
        throw InvalidEntryValue("KeyronaStorage[" + myStorageName + "|setEntry()]: Value for entry '" + entry + "' is invalid.");

    UInt32 myCurrentPosition;
    UInt32 myDBSize;
    UInt32 bytesToCopy;
    string line;
    UInt8 * memblock;

    // do we have such an entry already?
    // if so, check whether it is current or delete it and store it new
    if (findEntry(entry))
    {
        if (getEntry(entry) == value)
            return true;
        else
            deleteEntry(entry);
    }

    // get my current subsection position
    myCurrentPosition = *(SubSectionPositionsIterator);

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Current DB filesize: " << myDBSize << endl;
#endif
    // seek to insertion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip two lines for <SubSection> and SubSectionName...
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SubSectionIdentifier_Start.length(), KeyronaDatabase_SubSectionIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: Expected " + KeyronaDatabase_SubSectionIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_SubSectionNameIdentifier.length(), KeyronaDatabase_SubSectionNameIdentifier))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: Expected " + KeyronaDatabase_SubSectionNameIdentifier);

    myCurrentPosition = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Current read position: " << myDB.tellg() << endl;
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to insertion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // Inserting new section
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Adding Entry '" + entry + "' to position: " << myDB.tellg() << endl;
#endif
    myDB << KeyronaDatabase_EntryIdentifier_Start << endl;
    myDB << KeyronaDatabase_EntryNameIdentifier << "=\"" << entry << "\"" << endl;
    myDB << value << endl;
    myDB << KeyronaDatabase_EntryIdentifier_Stop << endl;

    // Copying saved data back into file
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|setEntry()]: Writing " << bytesToCopy << " bytes back to DB..." << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

    // Finally, re-new our pointers and vectors
    queryDBforEntries();
    return true;
};

//================================================================================
//
string KeyronaStorage::getEntry(const string &entryname)
{
    if (entryname.empty())
        throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|getEntry()]: Entry was empty.");
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|getEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("KeyronaStorage[" + myStorageName + "|getEntry()]: No Subsection has been selected.");

    UInt32 myCurrentPosition;
    string line;

    // check, if entry is available
    if (!findEntry(entryname))
        return "";

    // found entry, now get the value
    myCurrentPosition = *(EntryPositionsIterator);

    // seek to entry position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|getEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);

    // yes, we have an entry for <entryname>
    // I need to skip two lines for <Entry> and EntryName
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_EntryIdentifier_Start.length(), KeyronaDatabase_EntryIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|getEntry()]: Inconsistent Database: Expected " + KeyronaDatabase_EntryIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_EntryNameIdentifier.length(), KeyronaDatabase_EntryNameIdentifier))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|getEntry()]: Inconsistent Database: Expected " + KeyronaDatabase_EntryNameIdentifier);

    // Finally, get the value line
    getline(myDB, line);
    return line;
};

//================================================================================
//
bool KeyronaStorage::deleteEntry(const string &entry)
{
    if (entry.empty())
        throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|deleteEntry()]: Entry was empty.");
    if (mySection.empty())
        throw SectionNotSelected("KeyronaStorage[" + myStorageName + "|deleteEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("KeyronaStorage[" + myStorageName + "|deleteEntry()]: No Subsection has been selected.");

    UInt32 myCurrentPosition;
    UInt32 myDeletionPosition;
    UInt32 myDBSize;
    UInt32 bytesToCopy;
    string line;
    UInt8 * memblock;

    // check if subsection exists, if not, we're fine...
    if (!findEntry(entry))
        return true;

    // get my current entry position
    myCurrentPosition = *(EntryPositionsIterator);
    myDeletionPosition = *(EntryPositionsIterator);

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <Entry>...
    getline(myDB, line);
    if (line.compare(0, KeyronaDatabase_EntryIdentifier_Start.length(), KeyronaDatabase_EntryIdentifier_Start))
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|deleteEntry()]: Inconsistent Database: Expected " + KeyronaDatabase_EntryIdentifier_Start);

    // now read everything until </Entry>
    while (line.compare(0, KeyronaDatabase_EntryIdentifier_Stop.length(), KeyronaDatabase_EntryIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Current read position: " << myDB.tellg() << endl;
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Current write position: " << myDB.tellp() << endl;
#endif
    
    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("KeyronaStorage[" + myStorageName + "|deleteEntry()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif
    
    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting subsection
#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Deleting entry '" + entry + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef KEYRONA_STORAGE_DEBUG
    debug << "KeyronaStorage[" + myStorageName + "|deleteEntry()]: Deleting finished..." << endl;
#endif

    // Truncate file ...
    myCurrentPosition = myDB.tellg();
    truncateFile(myCurrentPosition);

    // Finally, re-new our pointers and vectors
    queryDBforEntries();
    return true;
};

//================================================================================
//
vector<string> KeyronaStorage::getAvailableSections()
{
    return mySections;
};

//================================================================================
//
vector<string> KeyronaStorage::getAvailableSubSections()
{
    return mySubSections;
};

//================================================================================
//
vector<string> KeyronaStorage::getAllEntries()
{
    return myEntries;
};

//================================================================================
//
vector<StringPair> KeyronaStorage::getAllEntriesWithValues()
{
    vector<StringPair> allMyEntries;

    if (mySection.empty())
        throw InvalidSectionName("KeyronaStorage|getAllEntries(): No section selected!");
    if (mySubSection.empty())
        throw InvalidSubSectionName("KeyronaStorage|getAllEntries(): No subsection selected!");

    // iterate through all sections
    vector<string>::const_iterator EntryIterator;
    // put iterator on beginning of sections
    EntryIterator = myEntries.begin();

    vector<StringPair> myEntriesWithValues;
    while ( EntryIterator != myEntries.end())
    {
        string currentEntry = *(EntryIterator);
        string currentValue = getEntry(currentEntry);
        StringPair currentEntryWithValue (currentEntry, currentValue);
        myEntriesWithValues.push_back(currentEntryWithValue);
        EntryIterator++;
    }
    return myEntriesWithValues;
};

//================================================================================
//
vector<StringPair> KeyronaStorage::findAllEntries(const string &entry)
{
    if (entry.empty())
        throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|findAllEntries()]: Entry was empty.");

    // create StringPair-Vector object and clear it
    vector<StringPair> allMyEntries;
    allMyEntries.clear();
    
    // get all Sections
    vector<string> Sections = getAvailableSections();
    if (Sections.empty())
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: No sections available..." << endl;
#endif
        return allMyEntries;
    }

    // iterate through all sections
    vector<string>::const_iterator SectionIterator;
    // put iterator on beginning of sections
    SectionIterator = Sections.begin();

    while ( SectionIterator != Sections.end())
    {
        // select a section
        selectSection(*SectionIterator);

        // get all subsections
        vector<string> SubSections = getAvailableSubSections();
        if (SubSections.empty())
        {
#ifdef KEYRONA_STORAGE_DEBUG
            debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: No subsections available in section '" << (*SectionIterator) << "'." << endl;
#endif
            SectionIterator++;
            continue;
        }
        
        // iterate through all sections
        vector<string>::const_iterator SubSectionIterator;
        // put iterator on beginning of sections
        SubSectionIterator = SubSections.begin();

        while ( SubSectionIterator != SubSections.end())
        {
            // select a subsection
            selectSubSection(*SubSectionIterator);
            if (findEntry(entry))
            {
#ifdef KEYRONA_STORAGE_DEBUG
                debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: Entry '" << entry << "' found in section '" << *(SectionIterator) << "', subsection '" << *(SubSectionIterator) << "'." << endl;
#endif
                StringPair entryFound (*SectionIterator, *SubSectionIterator);
                allMyEntries.push_back(entryFound);
            }
            SubSectionIterator++;
        }
        SectionIterator++;
    }
    return allMyEntries;
};

//================================================================================
//
vector<StringPair> KeyronaStorage::findAllEntries(const string &entry, const string &value)
{
    if (entry.empty())
        throw InvalidEntryName("KeyronaStorage[" + myStorageName + "|findAllEntries()]: Entry was empty.");

    if (value.empty())
        throw InvalidEntryValue("KeyronaStorage[" + myStorageName + "|findAllEntries()]: Value was empty.");

    // create StringPair-Vector object and clear it
    vector<StringPair> allMyEntries;
    allMyEntries.clear();

    // get all Sections
    vector<string> Sections = getAvailableSections();
    if (Sections.empty())
    {
#ifdef KEYRONA_STORAGE_DEBUG
        debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: No sections available..." << endl;
#endif
        return allMyEntries;
    }

    // iterate through all sections
    vector<string>::const_iterator SectionIterator;
    // put iterator on beginning of sections
    SectionIterator = Sections.begin();

    while ( SectionIterator != Sections.end())
    {
        // select a section
        selectSection(*SectionIterator);

        // get all subsections
        vector<string> SubSections = getAvailableSubSections();
        if (SubSections.empty())
        {
#ifdef KEYRONA_STORAGE_DEBUG
            debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: No subsections available in section '" << (*SectionIterator) << "'." << endl;
#endif
            SectionIterator++;
            continue;
        }

        // iterate through all sections
        vector<string>::const_iterator SubSectionIterator;
        // put iterator on beginning of sections
        SubSectionIterator = SubSections.begin();

        while ( SubSectionIterator != SubSections.end())
        {
            // select a subsection
            selectSubSection(*SubSectionIterator);
            if (findEntry(entry))
            {
                if (value == getEntry(entry))
                {
#ifdef KEYRONA_STORAGE_DEBUG
                    debug << "KeyronaStorage[" + myStorageName + "|findAllEntries()]: Entry '" << entry << "' with value '" << value << "' found in section '" << *(SectionIterator) << "', subsection '" << *(SubSectionIterator) << "'." << endl;
#endif
                    StringPair entryFound (*SectionIterator, *SubSectionIterator);
                    allMyEntries.push_back(entryFound);
                }
            }
            SubSectionIterator++;
        }
        SectionIterator++;
    }
    return allMyEntries;
};

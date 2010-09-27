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
 * @file        TpmCryptStorage.cxx
 * @brief       Implementation of the TpmCrypt storage service.
 * @version     $Revision: 549 $
 * @date        Last modification at $Date: 2009-05-07 10:27:30 +0200 (Thu, 07 May 2009) $ by $Author: selhorst $
 */

#include <TpmCryptStorage.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

//================================================================================
//
TpmCryptStorage::TpmCryptStorage( const string &StorageName, const string &StorageFile ):
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
        throw NoFilename("TpmCryptStorage: No valid database filename submitted! Please check your configuration file!");

    openFile();
    queryDBforSections();
};

//================================================================================
//
TpmCryptStorage::TpmCryptStorage( const string &StorageName, const string &StorageFile, bool clear):
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
        throw NoFilename("TpmCryptStorage: No valid database filename submitted! Please check your configuration file!");

    openFile();
    if (clear)
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage: clearing database!" << endl;
#endif
        truncateFile(0);
    }
    else
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage: not clearing database!" << endl;
#endif

    queryDBforSections();
};


//================================================================================
//
TpmCryptStorage::~TpmCryptStorage()
{
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|Destructor()]: Closing database '" + myStorageFile + "'." << endl;
#endif
    if (myDB.is_open())
        myDB.close();
};

//================================================================================
//
bool TpmCryptStorage::openFile()
{
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|openFile()]: Opening database '" + myStorageFile + "'." << endl;
#endif

    myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::ate);

    if (!myDB.is_open())
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|openFile()]: : Database file '" + myStorageFile + "' does not exist." << endl;
        debug << "TpmCryptStorage[" + myStorageName + "|openFile()]: : Creating database file..." << endl;
#endif
        myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::app);
        if (!myDB.is_open())
            throw NoOpenDB("TpmCryptStorage: Database '" + myStorageFile + "' could not be opened!\nAre path and filename correct and do you have enough access rights?");
        myDB.close();
        myDB.open(myStorageFile.c_str(), ios::in | ios::out |  ios::ate);
    }

    if (!myDB.is_open())
        throw NoOpenDB("TpmCryptStorage: Database '" + myStorageFile + "' could not be opened!\nAre path and filename correct and do you have enough access rights?");

    int myUID = getuid();
    if (!myUID)
        assignFilePermission(myStorageFile, DBFILEPERMISSION);

    return true;
};

//================================================================================
//
bool TpmCryptStorage::truncateFile( UInt32 truncatePosition )
{

    UInt8 * memblock;
    UInt32 myDBSize;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|truncateFile()]: : Truncating file '" + myStorageFile + "' at position: " << truncatePosition << endl;
#endif

    // get my filesize
    myDB.clear();
    myDB.seekg(0, ios::end);
    myDBSize = myDB.tellg();

    if (myDBSize < truncatePosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "]: Database error during truncation...");

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
void TpmCryptStorage::selectSection(const string &section)
{
#ifdef TPMCRYPT_STORAGE_DEBUG
    if (!mySection.empty())
        debug << "TpmCryptStorage[" + myStorageName + "|selectSection()]: My current section is: " << mySection << endl;
    else
        debug << "TpmCryptStorage[" + myStorageName + "|selectSection()]: Currently no section selected!" << endl;
#endif

    if (!findSection(section))
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|selectSection()]: Creating new section '" + section + "'." << endl;
#endif
        addSection(section);
        if (!findSection(section))
            throw SectionNotFound("TpmCryptStorage[" + myStorageName + "|selectSection()]: Section '" + section + "' has not been added correctly.");
    };
    
    mySection = section;
    mySubSection.clear();
    mySubSections.clear();
    mySubSectionPositions.clear();
    myEntries.clear();
    myEntryPositions.clear();

#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|selectSection()]: Section '" << mySection << "' selected!" << endl;
#endif
    queryDBforSubSections();

};

//================================================================================
//
void TpmCryptStorage::selectSubSection(const string &subSection)
{
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|selectSubSection()]: No Section has been selected. Section has to be set prior to calls to selectSubSection().");

#ifdef TPMCRYPT_STORAGE_DEBUG
    if (!mySubSection.empty())
        debug << "TpmCryptStorage[" + myStorageName + "|selectSubSection()]: My current subsection is: " << mySubSection << endl;
    else
        debug << "TpmCryptStorage[" + myStorageName + "|selectSubSection()]: Currently no subsection selected!" << endl;
#endif

    if (!findSubSection(subSection))
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|selectSubSection()]: Creating new subsection '" + subSection + "'." << endl;
#endif
        addSubSection(subSection);
        if (!findSubSection(subSection))
            throw SubSectionNotFound("TpmCryptStorage[" + myStorageName + "|selectSubSection()]: Subsection '" + subSection + "' has not been added correctly.");
    };

    mySubSection = subSection;
    myEntries.clear();
    myEntryPositions.clear();

#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|selectSubSection()]: Subsection '" << mySubSection << "' selected!" << endl;
#endif
    queryDBforEntries();
};

//================================================================================
//
void TpmCryptStorage::queryDBforSections(void)
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
        while (line.compare(0, TpmCryptDatabase_SectionIdentifier_Start.length(), TpmCryptDatabase_SectionIdentifier_Start))
        {
            myCurrentPosition = myDB.tellg();
            getline(myDB, line);
            // get my current position for later reuse
            if (myDB.eof())
                break;
        }

        // find the beginning of a section
        if (!line.compare(0, TpmCryptDatabase_SectionIdentifier_Start.length(), TpmCryptDatabase_SectionIdentifier_Start))
        {
            // the next line should have our SectionName
            getline(myDB, line);
            if (!line.compare(0, TpmCryptDatabase_SectionNameIdentifier.length(), TpmCryptDatabase_SectionNameIdentifier))
            {
                mySections.push_back(extractQuotationMarks(line));
                mySectionPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidSectionName("TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: Section does not have a valid SectionName.");

            // Now, since we found the a valid SectionName, get all lines until section closes
            while (line.compare(0, TpmCryptDatabase_SectionIdentifier_Stop.length(), TpmCryptDatabase_SectionIdentifier_Stop))
            {
                // if EOF reached while section still open
                if (myDB.eof())
                    throw SectionDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: Section '" + mySections.back() + "' does not close!.");
                
                // if section still open and a new section opens
                if (!line.compare(0, TpmCryptDatabase_SectionIdentifier_Start.length(), TpmCryptDatabase_SectionIdentifier_Start))
                    throw SectionDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: Section '" + mySections.back() + "' does not close!.");

                getline(myDB, line);
            }
        }
    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    // Now print out the found sections
    if (mySections.size())
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: Identified " << mySections.size() << " sections:" << endl;
        SectionIterator = mySections.begin();
        SectionPositionsIterator = mySectionPositions.begin();
        while ( SectionIterator != mySections.end())
        {
            debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: " << *(SectionIterator) << " @ " << *(SectionPositionsIterator) << endl;
            SectionIterator++;
            SectionPositionsIterator++;
        }
    }
    else
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSections()]: No sections identified!" << endl;
    }
#endif
};

//================================================================================
//
void TpmCryptStorage::queryDBforSubSections(void)
{
    string line;
    UInt32 myCurrentPosition;

    // clear my section and its according position vector
    mySubSections.clear();
    mySubSectionPositions.clear();

    // seek to the beginning of my section
    myDB.clear();
    myCurrentPosition = *(SectionPositionsIterator);
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: Seeking to position " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition,ios::beg);

    // read until section closes
    while (line.compare(0, TpmCryptDatabase_SectionIdentifier_Stop.length(), TpmCryptDatabase_SectionIdentifier_Stop))
    {
        // get my current position for later reuse
        myCurrentPosition = myDB.tellg();
        getline(myDB, line);
        // find the beginning of a subsection
        if (!line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Start.length(), TpmCryptDatabase_SubSectionIdentifier_Start))
        {
            // the next line should have our SubSectionName
            getline(myDB, line);
            if (!line.compare(0, TpmCryptDatabase_SubSectionNameIdentifier.length(), TpmCryptDatabase_SubSectionNameIdentifier))
            {
                mySubSections.push_back(extractQuotationMarks(line));
                mySubSectionPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidSubSectionName("TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: Subsection does not have a valid SubsectionName.");

            // Now, since we found the a valid SubSectionName, get all lines until section closes
            while (line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Stop.length(), TpmCryptDatabase_SubSectionIdentifier_Stop))
            {
                // if EOF reached while subsection still open
                if (myDB.eof())
                    throw SubSectionDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: Subsection '" + mySubSections.back() + "' does not close!.");
                // if subsection still open and a new subsection opens
                if (!line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Start.length(), TpmCryptDatabase_SubSectionIdentifier_Start))
                    throw SubSectionDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: SubSection '" + mySubSections.back() + "' does not close!.");

                getline(myDB, line);
            }
        }

    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    // Now print out the found subsections
    if (mySubSections.size())
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: Identified " << mySubSections.size() << " subsections:" << endl;
        SubSectionIterator = mySubSections.begin();
        SubSectionPositionsIterator = mySubSectionPositions.begin();
        while ( SubSectionIterator != mySubSections.end())
        {
            debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: " << *(SubSectionIterator) << " @ " << *(SubSectionPositionsIterator) << endl;
            SubSectionIterator++;
            SubSectionPositionsIterator++;
        }
    }
    else
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforSubSections()]: No subsections identified!" << endl;
    }
#endif
};

//================================================================================
//
void TpmCryptStorage::queryDBforEntries(void)
{
    string line;
    UInt32 myCurrentPosition;

    // clear my entries and its according position vector
    myEntries.clear();
    myEntryPositions.clear();

    // seek to the beginning of my section
    myDB.clear();
    myCurrentPosition = *(SubSectionPositionsIterator);
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Seeking to position " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition,ios::beg);

    // read until subsection closes
    while (line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Stop.length(), TpmCryptDatabase_SubSectionIdentifier_Stop))
    {
        // get my current position for later reuse
        myCurrentPosition = myDB.tellg();
        getline(myDB, line);
        // find the beginning of an entry
        if (!line.compare(0, TpmCryptDatabase_EntryIdentifier_Start.length(), TpmCryptDatabase_EntryIdentifier_Start))
        {
            // the next line should have our EntryName
            getline(myDB, line);
            if (!line.compare(0, TpmCryptDatabase_EntryNameIdentifier.length(), TpmCryptDatabase_EntryNameIdentifier))
            {
                myEntries.push_back(extractQuotationMarks(line));
                myEntryPositions.push_back(myCurrentPosition);
            }
            else
                throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Entry does not have a valid EntryName.");

            // Now, since we found the a valid EntryName, get all lines until Entry closes
            while (line.compare(0, TpmCryptDatabase_EntryIdentifier_Stop.length(), TpmCryptDatabase_EntryIdentifier_Stop))
            {
                // if EOF reached while subsection still open
                if (myDB.eof())
                    throw EntryDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");
                // if entry still open and a new subsection opens
                if (!line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Start.length(), TpmCryptDatabase_SubSectionIdentifier_Start))
                    throw EntryDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");
                // if entry still open and a new entry opens
                if (!line.compare(0, TpmCryptDatabase_EntryIdentifier_Start.length(), TpmCryptDatabase_EntryIdentifier_Start))
                    throw EntryDoesNotClose("TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Entry '" + myEntries.back() + "' does not close!.");

                getline(myDB, line);
            }
        }
    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    // Now print out the found entries
    if (myEntries.size())
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: Identified " << myEntries.size() << " entries:" << endl;
        EntryIterator = myEntries.begin();
        EntryPositionsIterator = myEntryPositions.begin();
        while ( EntryPositionsIterator != myEntryPositions.end())
        {
            debug << "TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: " << *(EntryIterator) << " @ " << *(EntryPositionsIterator) << endl;
            EntryIterator++;
            EntryPositionsIterator++;
        }
    }
    else
    {
        debug << "TpmCryptStorage[" + myStorageName + "|queryDBforEntries()]: No entries identified!" << endl;
    }
#endif
};

//================================================================================
//
bool TpmCryptStorage::findSection(const string &section)
{
    if (section.empty())
        throw InvalidSectionName("TpmCryptStorage[" + myStorageName + "|findSection()]: Invalid section name.");
    queryDBforSections();

    SectionIterator = mySections.begin();
    SectionPositionsIterator = mySectionPositions.begin();
    int entry = 0;

    while ( SectionIterator != mySections.end())
    {
        entry++;
        if (section == *(SectionIterator))
        {
#ifdef TPMCRYPT_STORAGE_DEBUG
            debug << "TpmCryptStorage[" + myStorageName + "|findSection()]: Section '" + section + "' found @ " << *(SectionPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        SectionIterator++;
        SectionPositionsIterator++;
    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|findSection()]: Section '" + section + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool TpmCryptStorage::findSubSection(const string &subsection)
{
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|findSubSection()]: No Section has been selected.");
    if (subsection.empty())
        throw InvalidSubSectionName("TpmCryptStorage[" + myStorageName + "|findSubSection()]: Invalid subsection name.");
    queryDBforSubSections();

    SubSectionIterator = mySubSections.begin();
    SubSectionPositionsIterator = mySubSectionPositions.begin();
    int entry = 0;

    while ( SubSectionIterator != mySubSections.end())
    {
        entry++;
        if (subsection == *(SubSectionIterator))
        {
#ifdef TPMCRYPT_STORAGE_DEBUG
            debug << "TpmCryptStorage[" + myStorageName + "|findSubSection()]: Subsection '" + subsection + "' found @ " << *(SubSectionPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        SubSectionIterator++;
        SubSectionPositionsIterator++;
    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|findSubSection()]: Subsection '" + subsection + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool TpmCryptStorage::findEntry(const string &entryname)
{
    if (entryname.empty())
        throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|findEntry()]: Invalid entry name.");
    queryDBforEntries();

    EntryIterator = myEntries.begin();
    EntryPositionsIterator = myEntryPositions.begin();
    int entry = 0;

    while ( EntryIterator != myEntries.end())
    {
        entry++;
        if (entryname == *(EntryIterator))
        {
#ifdef TPMCRYPT_STORAGE_DEBUG
            debug << "TpmCryptStorage[" + myStorageName + "|findEntry()]: Entry '" + entryname + "' found @ " << *(EntryPositionsIterator) << " (entry no: " << entry << ")!" << endl;
#endif
            return true;
        }
        EntryIterator++;
        EntryPositionsIterator++;
    }
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|findEntry()]: Entry '" + entryname + "' not found!" << endl;
#endif
    return false;
};

//================================================================================
//
bool TpmCryptStorage::addSection(const string &section)
{
    UInt32 myCurrentPosition;
    string line;

    // Seeking to last <Section>-entry
    if (mySections.size())
    {
        myCurrentPosition = mySectionPositions[(mySections.size()-1)];
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|addSection()]: Previously created sections exist, adding section below pos: " << myCurrentPosition << endl;
#endif
        myDB.clear();
        myDB.seekg(myCurrentPosition, ios::beg);

        // parse file for the last occurance of </Section>
        while (line.compare(0, TpmCryptDatabase_SectionIdentifier_Stop.length(), TpmCryptDatabase_SectionIdentifier_Stop))
        {
            getline(myDB, line);
            myCurrentPosition = myDB.tellg();
        }
    }
    else
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|addSection()]: No existing sections, adding section on top..." << endl;
#endif
        myCurrentPosition = 0;
    }
        myDB.clear();
        myDB.seekg(myCurrentPosition, ios::beg);
        myDB.seekp(myCurrentPosition, ios::beg);

        // Adding section
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|addSection()]: Adding section '" + section + "' to position: " << myDB.tellg() << endl;
#endif
        myDB << TpmCryptDatabase_SectionIdentifier_Start << endl;
        myDB << TpmCryptDatabase_SectionNameIdentifier << "=\"" << section << "\"" << endl;
        myDB << TpmCryptDatabase_SectionIdentifier_Stop << endl;

        myDB.clear();
        myDB.seekg(0, ios::end);
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|addSection()]: Current DB filesize: " << myDB.tellg() << endl;
#endif
        queryDBforSections();
        return true;
};

//================================================================================
//
bool TpmCryptStorage::deleteSection(const string &section)
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
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Deleting currently selected section." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <Section>...
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SectionIdentifier_Start.length(), TpmCryptDatabase_SectionIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|deleteSection()]: Inconsistent Database: Expected " + TpmCryptDatabase_SectionIdentifier_Start);

    // now read everything until </Section>
    while (line.compare(0, TpmCryptDatabase_SectionIdentifier_Stop.length(), TpmCryptDatabase_SectionIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Current write position: " << myDB.tellp() << endl;
#endif
    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting section
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Deleting section '" + section + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Deleting finished..." << endl;
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
bool TpmCryptStorage::addSubSection(const string &subsection)
{
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|addSubSection()]: No Section has been selected.");

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
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to insertion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip two lines for <Section> and SectionName...
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SectionIdentifier_Start.length(), TpmCryptDatabase_SectionIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: Expected " + TpmCryptDatabase_SectionIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SectionNameIdentifier.length(), TpmCryptDatabase_SectionNameIdentifier))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: Expected " + TpmCryptDatabase_SectionNameIdentifier);

    myCurrentPosition = myDB.tellg();
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|addSubSection()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to insertion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // Inserting new section
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Adding subsection '" + subsection + "' to position: " << myDB.tellg() << endl;
#endif
    myDB << TpmCryptDatabase_SubSectionIdentifier_Start << endl;
    myDB << TpmCryptDatabase_SubSectionNameIdentifier << "=\"" << subsection << "\"" << endl;
    myDB << TpmCryptDatabase_SubSectionIdentifier_Stop << endl;

    // Copying saved data back into file
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|addSubSection()]: Writing " << bytesToCopy << " bytes back to DB..." << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

    // Finally, re-new our pointers and vectors
    queryDBforSubSections();
    return true;
};

//================================================================================
//
bool TpmCryptStorage::deleteSubSection(const string &subsection)
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
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|deleteSection()]: Deleting currently selected subsection." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <SubSection>...
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Start.length(), TpmCryptDatabase_SubSectionIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Inconsistent Database: Expected " + TpmCryptDatabase_SubSectionIdentifier_Start);

    // now read everything until </SubSection>
    while (line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Stop.length(), TpmCryptDatabase_SubSectionIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Current read position: " << myDB.tellg() << endl;
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting subsection
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Deleting subsection '" + subsection + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteSubSection()]: Deleting finished..." << endl;
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
bool TpmCryptStorage::setEntry(const string &entry, const string &value)
{
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|setEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("TpmCryptStorage[" + myStorageName + "|setEntry()]: No Subsection has been selected.");
    if (value.empty())
        throw InvalidEntryValue("TpmCryptStorage[" + myStorageName + "|setEntry()]: Value for entry '" + entry + "' is invalid.");

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
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Current DB filesize: " << myDBSize << endl;
#endif
    // seek to insertion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip two lines for <SubSection> and SubSectionName...
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SubSectionIdentifier_Start.length(), TpmCryptDatabase_SubSectionIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: Expected " + TpmCryptDatabase_SubSectionIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_SubSectionNameIdentifier.length(), TpmCryptDatabase_SubSectionNameIdentifier))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: Expected " + TpmCryptDatabase_SubSectionNameIdentifier);

    myCurrentPosition = myDB.tellg();
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Current read position: " << myDB.tellg() << endl;
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Current write position: " << myDB.tellp() << endl;
#endif

    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|setEntry()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif

    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to insertion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // Inserting new section
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Adding Entry '" + entry + "' to position: " << myDB.tellg() << endl;
#endif
    myDB << TpmCryptDatabase_EntryIdentifier_Start << endl;
    myDB << TpmCryptDatabase_EntryNameIdentifier << "=\"" << entry << "\"" << endl;
    myDB << value << endl;
    myDB << TpmCryptDatabase_EntryIdentifier_Stop << endl;

    // Copying saved data back into file
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|setEntry()]: Writing " << bytesToCopy << " bytes back to DB..." << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

    // Finally, re-new our pointers and vectors
    queryDBforEntries();
    return true;
};

//================================================================================
//
string TpmCryptStorage::getEntry(const string &entryname)
{
    if (entryname.empty())
        throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|getEntry()]: Entry was empty.");
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|getEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("TpmCryptStorage[" + myStorageName + "|getEntry()]: No Subsection has been selected.");

    UInt32 myCurrentPosition;
    string line;

    // check, if entry is available
    if (!findEntry(entryname))
        return "";

    // found entry, now get the value
    myCurrentPosition = *(EntryPositionsIterator);

    // seek to entry position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|getEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);

    // yes, we have an entry for <entryname>
    // I need to skip two lines for <Entry> and EntryName
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_EntryIdentifier_Start.length(), TpmCryptDatabase_EntryIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|getEntry()]: Inconsistent Database: Expected " + TpmCryptDatabase_EntryIdentifier_Start);

    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_EntryNameIdentifier.length(), TpmCryptDatabase_EntryNameIdentifier))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|getEntry()]: Inconsistent Database: Expected " + TpmCryptDatabase_EntryNameIdentifier);

    // Finally, get the value line
    getline(myDB, line);
    return line;
};

//================================================================================
//
bool TpmCryptStorage::deleteEntry(const string &entry)
{
    if (entry.empty())
        throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Entry was empty.");
    if (mySection.empty())
        throw SectionNotSelected("TpmCryptStorage[" + myStorageName + "|deleteEntry()]: No Section has been selected.");
    if (mySubSection.empty())
        throw SubSectionNotSelected("TpmCryptStorage[" + myStorageName + "|deleteEntry()]: No Subsection has been selected.");

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
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Current DB filesize: " << myDBSize << endl;
#endif

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Seeking to position: " << myCurrentPosition << endl;
#endif
    myDB.seekg(myCurrentPosition, ios::beg);
    myDB.seekp(myCurrentPosition, ios::beg);

    // I need to skip one line for <Entry>...
    getline(myDB, line);
    if (line.compare(0, TpmCryptDatabase_EntryIdentifier_Start.length(), TpmCryptDatabase_EntryIdentifier_Start))
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Inconsistent Database: Expected " + TpmCryptDatabase_EntryIdentifier_Start);

    // now read everything until </Entry>
    while (line.compare(0, TpmCryptDatabase_EntryIdentifier_Stop.length(), TpmCryptDatabase_EntryIdentifier_Stop))
        getline(myDB, line);

    // finished reading, where am I now?
    myCurrentPosition = myDB.tellg();
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Current read position: " << myDB.tellg() << endl;
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Current write position: " << myDB.tellp() << endl;
#endif
    
    // Read everything from this point into memory
    if (myDBSize < myCurrentPosition)
        throw InconsistentDatabase("TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Inconsistent Database: DB size too small");

    bytesToCopy = myDBSize - myCurrentPosition;
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Reading " << bytesToCopy << " bytes into memory..." << endl;
#endif
    
    memblock = new UInt8[bytesToCopy];
    memset (memblock, 0, bytesToCopy);

    myDB.read(reinterpret_cast <char*>(memblock), bytesToCopy);

    // seek to deletion position
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Seeking to position: " << myDeletionPosition << endl;
#endif
    myDB.seekg(myDeletionPosition, ios::beg);
    myDB.seekp(myDeletionPosition, ios::beg);

    // Deleting subsection
#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Deleting entry '" + entry + "' at position: " << myDB.tellg() << endl;
#endif
    myDB.write(reinterpret_cast <char*>(memblock), bytesToCopy);
    delete[] memblock;

#ifdef TPMCRYPT_STORAGE_DEBUG
    debug << "TpmCryptStorage[" + myStorageName + "|deleteEntry()]: Deleting finished..." << endl;
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
vector<string> TpmCryptStorage::getAvailableSections()
{
    return mySections;
};

//================================================================================
//
vector<string> TpmCryptStorage::getAvailableSubSections()
{
    return mySubSections;
};

//================================================================================
//
vector<string> TpmCryptStorage::getAllEntries()
{
    return myEntries;
};

//================================================================================
//
vector<StringPair> TpmCryptStorage::getAllEntriesWithValues()
{
    vector<StringPair> allMyEntries;

    if (mySection.empty())
        throw InvalidSectionName("TpmCryptStorage|getAllEntries(): No section selected!");
    if (mySubSection.empty())
        throw InvalidSubSectionName("TpmCryptStorage|getAllEntries(): No subsection selected!");

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
vector<StringPair> TpmCryptStorage::findAllEntries(const string &entry)
{
    if (entry.empty())
        throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|findAllEntries()]: Entry was empty.");

    // create StringPair-Vector object and clear it
    vector<StringPair> allMyEntries;
    allMyEntries.clear();
    
    // get all Sections
    vector<string> Sections = getAvailableSections();
    if (Sections.empty())
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: No sections available..." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
            debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: No subsections available in section '" << (*SectionIterator) << "'." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
                debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: Entry '" << entry << "' found in section '" << *(SectionIterator) << "', subsection '" << *(SubSectionIterator) << "'." << endl;
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
vector<StringPair> TpmCryptStorage::findAllEntries(const string &entry, const string &value)
{
    if (entry.empty())
        throw InvalidEntryName("TpmCryptStorage[" + myStorageName + "|findAllEntries()]: Entry was empty.");

    if (value.empty())
        throw InvalidEntryValue("TpmCryptStorage[" + myStorageName + "|findAllEntries()]: Value was empty.");

    // create StringPair-Vector object and clear it
    vector<StringPair> allMyEntries;
    allMyEntries.clear();

    // get all Sections
    vector<string> Sections = getAvailableSections();
    if (Sections.empty())
    {
#ifdef TPMCRYPT_STORAGE_DEBUG
        debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: No sections available..." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
            debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: No subsections available in section '" << (*SectionIterator) << "'." << endl;
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
#ifdef TPMCRYPT_STORAGE_DEBUG
                    debug << "TpmCryptStorage[" + myStorageName + "|findAllEntries()]: Entry '" << entry << "' with value '" << value << "' found in section '" << *(SectionIterator) << "', subsection '" << *(SubSectionIterator) << "'." << endl;
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

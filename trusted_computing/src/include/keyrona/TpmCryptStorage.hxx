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
 * @file        TpmCryptStorage.hxx
 * @brief       Implementation of the TpmCrypt storage service.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _TPMCRYPTSTORAGE_HXX
#define _TPMCRYPTSTORAGE_HXX

#include <TpmCrypt.hxx>

// Database entries
const std::string TpmCryptDatabase_SectionIdentifier_Start       =	"<Section>";
const std::string TpmCryptDatabase_SectionIdentifier_Stop        =	"</Section>";
const std::string TpmCryptDatabase_SectionNameIdentifier         =       "SectionName";
const std::string TpmCryptDatabase_SubSectionIdentifier_Start    =	"<SubSection>";
const std::string TpmCryptDatabase_SubSectionIdentifier_Stop     =	"</SubSection>";
const std::string TpmCryptDatabase_SubSectionNameIdentifier      =       "SubSectionName";
const std::string TpmCryptDatabase_EntryIdentifier_Start         =	"<Entry>";
const std::string TpmCryptDatabase_EntryIdentifier_Stop          =	"</Entry>";
const std::string TpmCryptDatabase_EntryNameIdentifier           =	"EntryName";
const std::string TpmCryptDatabase_ValueIdentifier               =	"Value";

using namespace std;
using namespace utils;

namespace keyrona
{
    class TpmCryptStorage
    {
        public:
	    /*!
             *  @brief constructor for a TpmCryptStorage object
	     *  @param &StorageName, const string, contains the name of the storage
	     * 	@param &StorageFile, const string, path to the database
	     *	@param clear, bool, determines whether or not the database should be reset
	     */
            TpmCryptStorage( const string &StorageName, const string &StorageFile );
            TpmCryptStorage( const string &StorageName, const string &StorageFile, bool clear);

            ~TpmCryptStorage();


	    /*!
             *  @brief selects a section in the database
	     *  @param &section, const string, contains name of the section to be selected
	     */
            void selectSection(const string &section);

            /*!
             *  @brief finds a section in the database
	     *  @param &section, const string, contains name of the section to be found
	     *	@return bool, determines whether or not the section exists
	     */
	    bool findSection(const string &section);

            /*!
             *  @brief deletes a section from the database
	     *  @param &section, const string, contains name of the section to be deleted
	     *	@return bool, determines whether or not the deletion was successful
	     */
	    bool deleteSection(const string &section);


	    /*!
             *  @brief selects a subsection in the database
	     *  @param &subSection, const string, contains name of the subsection to be selected
	     */
            void selectSubSection(const string &subSection);

            /*!
             *  @brief finds a subsection in the database
	     *  @param &subSection, const string, contains name of the subsection to be found
	     *	@return bool, determines whether or not the subsection exists
	     */	    
            bool findSubSection(const string &subsection);

            /*!
             *  @brief deletes a subsection from the database
	     *  @param &subSection, const string, contains name of the subsection to be deleted
	     *	@return bool, determines whether or not the deletion was successful
	     */
            bool deleteSubSection(const string &subsection);

            /*!
             *  @brief sets an entry in the database to a specified value
	     *  @param &entry, const string, contains name of the entry to be changed
	     *  @param &value, const string, contains value to be set
	     *	@return bool, determines whether or not the operation was successful
	     */
            bool setEntry(const string &entry, const string &value);
	    	    
            /*!
             *  @brief finds an entry in the database
	     *  @param &entry, const string, contains name of the entry
	     *	@return bool, determines whether or not the entry is found
	     */
            bool findEntry(const string &entry);

	    /*!
             *  @brief gets the value for an entry
	     *  @param &entry, const string, contains name of the entry
	     *	@return string, contains the value of the entry
	     */	    
            string getEntry(const string &entry);

	    /*!
             *  @brief deletes an entry from the database
	     *  @param &entry, const string, contains name of the entry to be deleted
	     *	@return bool, determines whether or not the deletion was successful
	     */
            bool deleteEntry(const string &entry);

	    /*!
             *  @brief returns the available sections from the storage database
	     *	@return vector of string, contains the available sections
	     */
            vector<string> getAvailableSections();

            /*!
             *  @brief returns the available subsections from the storage database
	     *	@return vector of string, contains the available subsections
	     */
	    vector<string> getAvailableSubSections();

	    /*!
             *  @brief returns the available entries
	     *	@return vector of string, contains the available entries
	     */
            vector<string> getAllEntries();

	    /*!
             *  @brief returns the available entries and their corresponding values
	     *	@return vector of StringPair, contains the available entries and their corresponding values
	     */
            vector<StringPair> getAllEntriesWithValues();

	    /*!
             *  @brief finds all entries with the same name in a database
	     *  @param &entry, const string, contains name of the entry to be found
	     *	@return vector of StringPair, contains the matching entries and their corresponding values
	     */
            vector<StringPair> findAllEntries(const string &entry);

            /*!
             *  @brief finds all entries with the same name in a database with a certain value
	     *  @param &entry, const string, contains name of the entry to be found
	     *  @param &value, const string, contains the value of the entry to be found
	     *	@return vector of StringPair, contains the matching entries and their corresponding values
	     */
	    vector<StringPair> findAllEntries(const string &entry, const string &value);

        private:
	    
	    /*!
             *  @brief opens a database file
	     *	@return boolean, determines whether or not the operation was successful
	     */
            bool openFile();

            /*!
             *  @brief truncates a file at a specified position
	     * 	@param truncatePosition, UInt32, position where the file should be truncated
	     *	@return boolean, determines whether or not the operation was successful
	     */
	    bool truncateFile( UInt32 truncatePosition );

            /*!
             *  @brief checks for sections in a database
	     */
	    void queryDBforSections(void);

            /*!
             *  @brief checks for subsections in a database
	     */
	    void queryDBforSubSections(void);

            /*!
             *  @brief checks for entries in a database
	     */
	    void queryDBforEntries(void);

            /*!
             *  @brief adds a section to a database
	     *	@param &section, string, contains the name of the section to be added
	     */
	    bool addSection(const string &section);

            /*!
             *  @brief adds a subsection to a database
	     *	@param &subsection, string, contains the name of the subsection to be created
	     */
	    bool addSubSection(const string &subsection);

            string  myStorageName;
            string  myStorageFile;
            string  mySection;
            string  mySubSection;
            fstream myDB;

            vector<string> mySections;
            vector<string>::const_iterator SectionIterator;
            vector<UInt32> mySectionPositions;
            vector<UInt32>::const_iterator SectionPositionsIterator;

            vector<string> mySubSections;
            vector<string>::const_iterator SubSectionIterator;
            vector<UInt32> mySubSectionPositions;
            vector<UInt32>::const_iterator SubSectionPositionsIterator;

            vector<string> myEntries;
            vector<string>::const_iterator EntryIterator;
            vector<UInt32> myEntryPositions;
            vector<UInt32>::const_iterator EntryPositionsIterator;

        /*!
         *  @brief  disabled copy constructor
         */
        TpmCryptStorage( const TpmCryptStorage& );

        /*!
         *  @brief  disabled assignment operator
         */
        TpmCryptStorage& operator=( const TpmCryptStorage& );

        };
};

#endif

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
 * @file        KeyronaHelper.cxx
 * @brief       Implementation of some Keyrona helper methods
 * @version     $Revision: 803 $
 * @date        Last modification at $Date: 2009-08-26 11:14:28 +0200 (Wed, 26 Aug 2009) $ by $Author: selhorst $
 */
#include <KeyronaHelper.hxx>
#include <KeyronaLogin.hxx>
#include <KeyronaExceptions.hxx>
#include <KeyronaDate.hxx>
#include <KeyronaTPM.hxx>
#include <algorithm>
#include <mntent.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>

using namespace std;
using namespace utils;

//================================================================================
//
string extractQuotationMarks(string line)
{
    int start_pos, end_pos, length;
    start_pos = line.find_first_of("\"");
    end_pos = line.find_last_of("\"");
    length = line.length() - start_pos - 2; // line length - start position - 2 quotation marks
    return line.substr(start_pos+1, length);
};

//================================================================================
//
// simple method to print char* as hexstring
void printCharPointer(char *toPrint, int length)
{
   ostringstream myStream;
   for (int i=0; i<length; i++)
   {
       unsigned int temp = toPrint[i];
       myStream << hex << setfill('0') << setw (2) << temp << dec << " ";
   }
   myStream << flush;
   cout << myStream.str() << endl;
};

//================================================================================
//
// simple method to print UInt32 vectors
void printUInt32Vector(vector<UInt32> *vectorOfUInt32)
{
    vector<UInt32>::const_iterator Iterator;
    if (vectorOfUInt32->size())
    {
        Iterator=vectorOfUInt32->begin();
        while ( Iterator != vectorOfUInt32->end())
        {
            cout << "\t" << *(Iterator)  << endl;
            Iterator++;
        }
    }
};

//================================================================================
//
// simple method to print UInt8 vectors
void printUInt8Vector(vector<UInt8> *vectorOfUInt8)
{
    ostringstream myStream;
    vector<UInt8>::const_iterator Iterator;
    Iterator=vectorOfUInt8->begin();
    while ( Iterator != vectorOfUInt8->end())
    {
        UInt8 tempUInt8 = *(Iterator);
        unsigned int temp = (unsigned int) tempUInt8;
        myStream << hex << setfill('0') << setw (2) << temp << dec << " ";
        Iterator++;
   }
   myStream << flush;
   cout << myStream.str() << endl;
};

//================================================================================
//
// simple method to print string vectors
void printStringVector(vector<string> *vectorOfStrings)
{
    vector<string>::const_iterator Iterator;
    if (vectorOfStrings->size())
    {
        Iterator=vectorOfStrings->begin();
        while ( Iterator != vectorOfStrings->end())
        {
            cout << "\t" << *(Iterator)  << endl;
            Iterator++;
        }
    }
};

//================================================================================
//
// simple method to print string vectors
void printStringPairVector(vector<utils::StringPair> *vectorOfStringPairs)
{
    vector<utils::StringPair>::const_iterator Iterator;
    if (vectorOfStringPairs->size())
    {
        Iterator=vectorOfStringPairs->begin();
        while ( Iterator != vectorOfStringPairs->end())
        {
            cout << "\t" << Iterator->first << "\t" << Iterator->second << endl;
            Iterator++;
        }
    }
};


//================================================================================
//
string convertUInt32toString( UInt32 SubjectID )
{
    stringstream mySIDString;
    mySIDString << SubjectID;
    return mySIDString.str();
};

//================================================================================
//
UInt32 convertStringtoUInt32( string myString )
{
    UInt32 myUInt32;
    istringstream myStream(myString);
    if (myStream >> myUInt32)
        return myUInt32;
    else
        return 0;
};

//================================================================================
//
bool verifyFilename(string &filename, bool write)
{
    if (! fileExists(filename))
        return false;
    if (write)
        return fileWritable(filename);
    else
        return fileReadable(filename);
};

//================================================================================
//
bool fileExists(string &filename)
{
    struct stat buf;
    int i = stat ( filename.c_str(), &buf );
     /* File found */
    if ( i == 0 )
    {
        debug << "File '" << filename << "' exists!" << endl;
        return true;
    }
    debug << "File '" << filename << "' does NOT exist!" << endl;
    return false;

};

//================================================================================
//
bool fileWritable(string &filename)
{
    struct stat buf;
    int i = stat ( filename.c_str(), &buf );
    /* File found */
    if ( i == 0 )
    {
        // are we root?
        if (getuid() == 0)
            return true;

        // does this file belong to us?
        if (buf.st_uid == getuid())
        {
            debug << "File '" << filename << "' belongs to you '" << buf.st_uid << "'!" << endl;
            if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IWUSR))
            {
                debug << "File '" << filename << "' is writable by you!" << endl;
                return true;
            }
        }

        // is this file writable by others?
        if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IWOTH))
        {
            debug << "File '" << filename << "' is writable by others!" << endl;
            return true;
        }

        // are we in a group with write-access?
        struct group *fileGroup = getgrgid(buf.st_gid);
        struct passwd *currentUser = getpwuid(getuid());
        string currentUsername = currentUser->pw_name;

        if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IWGRP))
        {
            debug << "File '" << filename << "' belongs to GID '" << buf.st_gid << "' (" << fileGroup->gr_name << ")!" << endl;
            debug << "Checking if user '" << currentUsername << "' belongs to this group:" << endl;

            for (int lp = 1; NULL != *(fileGroup->gr_mem); lp++, (fileGroup->gr_mem)++)
            {
                string currentGroupMember = *(fileGroup->gr_mem);
                if (currentUsername == currentGroupMember)
                {
                    debug << "You are in group '" << fileGroup->gr_name << "', so file is writable!" << endl;
                    return true;
                }
            }
        }
    }
    debug << "File '" << filename << "' is NOT writable!" << endl;
    return false;
};

//================================================================================
//
bool fileReadable(string &filename)
{
    struct stat buf;
    int i = stat ( filename.c_str(), &buf );
    /* File found */
    if ( i == 0 )
    {
        // are we root?
        if (getuid() == 0)
            return true;

        // does this file belong to us?
        if (buf.st_uid == getuid())
        {
            debug << "File '" << filename << "' belongs to you '" << buf.st_uid << "'!" << endl;
            if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IRUSR))
            {
                debug << "File '" << filename << "' is readable by you!" << endl;
                return true;
            }
        }

        // is this file readable by others?
        if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IROTH))
        {
            debug << "File '" << filename << "' is readable by others!" << endl;
            return true;
        }

        // are we in a group with read-access?
        struct group *fileGroup = getgrgid(buf.st_gid);
        struct passwd *currentUser = getpwuid(getuid());
        string currentUsername = currentUser->pw_name;

        if ((buf.st_mode & S_IFREG) && (buf.st_mode & S_IRGRP))
        {
            debug << "File '" << filename << "' belongs to GID '" << buf.st_gid << "' (" << fileGroup->gr_name << ")!" << endl;
            debug << "Checking if user '" << currentUsername << "' belongs to this group:" << endl;

            for (int lp = 1; NULL != *(fileGroup->gr_mem); lp++, (fileGroup->gr_mem)++)
            {
                string currentGroupMember = *(fileGroup->gr_mem);
                if (currentUsername == currentGroupMember)
                {
                    debug << "You are in group '" << fileGroup->gr_name << "', so file is readable!" << endl;
                    return true;
                }
            }
        }
    }
    debug << "File '" << filename << "' is NOT readable!" << endl;
    return false;
};

//================================================================================
//
bool dirExists(string &dirname)
{
    struct stat buf;
    int i = stat ( dirname.c_str(), &buf );
     /* Dir found */
    if ( i == 0 )
    {
        if (buf.st_mode & S_IFDIR)
        {
            debug << "Directory '" << dirname << "' exists!" << endl;
            return true;
        }
    }
    debug << "Directory '" << dirname << "' does NOT exist!" << endl;
    return false;
};

//================================================================================
//
bool dirWritable(string &dirname)
{
    struct stat buf;
    int i = stat ( dirname.c_str(), &buf );
    /* Dir found */
    if ( i == 0 )
    {
        // are we root?
        if (getuid() == 0)
            return true;

        // does this directory belong to us?
        if (buf.st_uid == getuid())
        {
            debug << "Directory '" << dirname << "' belongs to you '" << buf.st_uid << "'!" << endl;
            if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IWUSR))
            {
                debug << "Directory '" << dirname << "' is writable by you!" << endl;
                return true;
            }
        }

        // is this directory writable by others?
        if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IWOTH))
        {
            debug << "Directory '" << dirname << "' is writable by others!" << endl;
            return true;
        }

        // are we in a group with write-access?
        struct group *dirGroup = getgrgid(buf.st_gid);
        struct passwd *currentUser = getpwuid(getuid());
        string currentUsername = currentUser->pw_name;

        if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IWGRP))
        {
            debug << "Directory '" << dirname << "' belongs to GID '" << buf.st_gid << "' (" << dirGroup->gr_name << ")!" << endl;
            debug << "Checking if user '" << currentUsername << "' belongs to this group:" << endl;

            for (int lp = 1; NULL != *(dirGroup->gr_mem); lp++, (dirGroup->gr_mem)++)
            {
                printf("Group member %d is: %s\n", lp, *(dirGroup->gr_mem));
                string currentGroupMember = *(dirGroup->gr_mem);
                if (currentUsername == currentGroupMember)
                {
                    debug << "You are in group '" << dirGroup->gr_name << "', so directory is writable!" << endl;
                    return true;
                }
            }
        }
    }
    debug << "Directory '" << dirname << "' is NOT writable!" << endl;
    return false;
};

//================================================================================
//
bool dirReadable(string &dirname)
{
    struct stat buf;
    int i = stat ( dirname.c_str(), &buf );
    /* Dir found */
    if ( i == 0 )
    {
        // are we root?
        if (getuid() == 0)
            return true;

        // does this directory belong to us?
        if (buf.st_uid == getuid())
        {
            debug << "Directory '" << dirname << "' belongs to you '" << buf.st_uid << "'!" << endl;
            if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IRUSR))
            {
                debug << "Directory '" << dirname << "' is readable by you!" << endl;
                return true;
            }
        }

        // is this directory readable by others?
        if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IROTH))
        {
            debug << "Directory '" << dirname << "' is writable by others!" << endl;
            return true;
        }

        // are we in a group with read-access?
        struct group *dirGroup = getgrgid(buf.st_gid);
        struct passwd *currentUser = getpwuid(getuid());
        string currentUsername = currentUser->pw_name;

        if ((buf.st_mode & S_IFDIR) && (buf.st_mode & S_IRGRP))
        {
            debug << "Directory '" << dirname << "' belongs to GID '" << buf.st_gid << "' (" << dirGroup->gr_name << ")!" << endl;
            debug << "Checking if user '" << currentUsername << "' belongs to this group:" << endl;

            for (int lp = 1; NULL != *(dirGroup->gr_mem); lp++, (dirGroup->gr_mem)++)
            {
                printf("Group member %d is: %s\n", lp, *(dirGroup->gr_mem));
                string currentGroupMember = *(dirGroup->gr_mem);
                if (currentUsername == currentGroupMember)
                {
                    debug << "You are in group '" << dirGroup->gr_name << "', so directory is readable!" << endl;
                    return true;
                }
            }
        }
    }
    debug << "Directory '" << dirname << "' is NOT readable!" << endl;
    return false;
};

//================================================================================
//
void storeByteVectorInFile(string &filename, ByteVector &toStore)
{
    fstream myFile;
    myFile.open(filename.c_str(), ios::out );
    if (!myFile.is_open())
        throw InvalidFilename("storeByteVectorInFile(): Can't write to file '" + filename + "'!");

    myFile.clear();
    myFile.seekg(0, ios::beg);
    myFile.write(reinterpret_cast <char*>(toStore.toCArray()), toStore.size());
    myFile.close();
};

//================================================================================
//
ByteVector loadByteVectorFromFile(string &filename)
{
    UInt8 * memblock;
    fstream myFile;

    myFile.open(filename.c_str(), ios::in);
    if (!myFile.is_open())
        throw InvalidFilename("loadByteVectorFromFile(): Can't read from file '" + filename + "'!");

    myFile.seekg(0, ios::end);
    UInt32 length = myFile.tellg();
    memblock = new UInt8[length];
    memset (memblock, 0, length);
    
    myFile.clear();
    myFile.seekg(0, ios::beg);
    myFile.read(reinterpret_cast <char*>(memblock), length);
    myFile.close();

    ByteVector loadedData(memblock, length);
    delete[] memblock;
    return loadedData;
};


//================================================================================
//
bool convertUInt8VectorToChar(vector<UInt8> *toConvert, char *dest, int &length)
{
    vector<UInt8>::const_iterator Iterator;
    if (toConvert->size())
    {
        length=toConvert->size();
        Iterator=toConvert->begin();
        while ( Iterator != toConvert->end())
        {
            *dest = *(Iterator);
            dest++;
            Iterator++;
        }
        return true;
    }
    else
    {
        return false;
    }
};

//================================================================================
//
bool convertIntVectorToChar(vector<int> *toConvert, char *dest, int &length)
{
    vector<int>::const_iterator Iterator;
    if (toConvert->size())
    {
        length=toConvert->size();
        Iterator=toConvert->begin();
        while ( Iterator != toConvert->end())
        {
            *dest = *(Iterator);
            dest++;
            Iterator++;
        }
        return true;
    }
    else
    {
        return false;
    }
};

//================================================================================
//
bool convertCharToUInt8Vector(vector<UInt8> *toConvert, char *src, int length)
{
    if (length < 0)
        return false;
    toConvert->clear();
    for (int i=0; i<length; i++)
    {
        toConvert->push_back(*src);
        src++;
    }
    return true;
};

//================================================================================
//
vector<UInt8> convertStringToUInt8Vector(const string &toConvert)
{
    vector<UInt8> myString;
    int length = toConvert.length();
    for (int i=0; i<length; i++)
        myString.push_back(toConvert[i]);
    return myString;
};

//================================================================================
//
vector<int> convertStringToIntVector(const string &toConvert)
{
    vector<int> myString;
    int length = toConvert.length();
    for (int i=0; i<length; i++)
        myString.push_back(toConvert[i]);
    return myString;
};

//================================================================================
//
ByteVector convertStringToByteVector(const string &toConvert)
{
	ByteVector myString(toConvert);
	
	return myString;
};

//================================================================================
//
string EncodeUInt8VectorToBASE64(vector<UInt8> *toConvert)
{
    int length = toConvert->size();
    if (!length)
        return "";

    char myMemRegion[length];
    if  (!convertUInt8VectorToChar(toConvert, myMemRegion, length))
        return "";
    return base64_encode(reinterpret_cast<const unsigned char*>(myMemRegion), length);
};

//================================================================================
//
vector<UInt8> DecodeBASE64StringToUInt8Vector(const string &toConvert)
{
    string decoded = base64_decode(toConvert);
    return convertStringToUInt8Vector(decoded);
};

//================================================================================
//
string EncodeByteVectorToBASE64(ByteVector &toConvert)
{
    int length = toConvert.size();
    if (!length)
        return "";

    return base64_encode(reinterpret_cast<const unsigned char*>(toConvert.toCArray()), length);
};

//================================================================================
//
ByteVector DecodeBASE64StringToByteVector(const string &toConvert)
{
    string decoded = base64_decode(toConvert);
    ByteVector result(decoded);
    return result;
};

//================================================================================
//
string removeDelimiter(string myEntry)
{
    if (myEntry.empty())
        throw runtime_error("removeDelimiter: string was empty!");

    // remove trailing slash
    if (!myEntry.compare(myEntry.length()-1,1,KeyronaPathSeparator))
        return myEntry.substr(0,myEntry.length()-1);
    else
        return myEntry;
};

//================================================================================
//
string convertUInt8VectorToString(vector<UInt8> *toConvert)
{
    int length = toConvert->size();
    if (!length)
        return "";
    char myString[length];
    convertUInt8VectorToChar(toConvert, myString, length);
    return string(myString, length);
};

//================================================================================
//
string convertIntVectorToString(vector<int> *toConvert)
{
    int length = toConvert->size();
    if (!length)
        return "";
    char myString[length];
    convertIntVectorToChar(toConvert, myString, length);
    return string(myString, length);
};

void findAvailableFilesystems(string fs, vector<string>* AvailableFilesystems)
{
    string mkfs;

    mkfs = "/sbin/mkfs." + fs;
    if (fileExists(mkfs))
    {
        debug << "Found '" << fs << "' at '" << mkfs << "'." << endl;
        AvailableFilesystems->push_back(fs);
    }

    mkfs.clear();
    mkfs = "/usr/sbin/mkfs." + fs;
    if (fileExists(mkfs))
    {
        debug << "Found '" << fs << "' at '" << mkfs << "'." << endl;
        AvailableFilesystems->push_back(fs);
    }
};

//================================================================================
//
string getUUID(string volume, string &UUIDpath)
{
    DIR *deviceDirectory = opendir(UUIDpath.c_str());
    string search;
    int i,j;
    char buf[BUFFERSIZE], real[BUFFERSIZE];
    memset(buf, 0, BUFFERSIZE);
    memset(real, 0, BUFFERSIZE);

    debug << "Searching match for " << volume << endl;

    if(deviceDirectory)
    {
        struct dirent *ent;
        while((ent = readdir(deviceDirectory)) != NULL)
        {
            search = UUIDpath + KeyronaPathSeparator;
            search += ent->d_name;
            debug << "Searching " << search << endl;
            ssize_t len = readlink(search.c_str(), buf, sizeof(buf)-1);
            debug << "Result is " << buf << endl;
            if (len != -1) {
            buf[len] = '\0';
            }
            else {
            /* handle error condition */
            }
            search = buf;
            i = search.rfind("/");
            j = search.length()-1;
            debug << "Possible match is " << buf << " with the UUID "<< ent->d_name << endl;
            if ( i > 0){
                if( volume.compare(("/dev"+search.substr(i,j))) == 0){
                    return ent->d_name;
                }
            }
        }
        closedir(deviceDirectory);
    }
    else
    {
        cout << "Error opening directory '" << UUIDpath << "'!" << endl;
    }
    return volume;
};

//================================================================================
//
vector<StringPair> getAvailableVolumes(string &UUIDpath)
{
    DIR *deviceDirectory = opendir(UUIDpath.c_str());
    string search;
    vector<StringPair> myAvailableVolumes;
    int i,j;
    char buf[BUFFERSIZE], real[BUFFERSIZE];
    memset(buf, 0, BUFFERSIZE);
    memset(real, 0, BUFFERSIZE);

    debug << "Searching for Volumes " << endl;

    if(deviceDirectory)
    {
        struct dirent *ent;
        while((ent = readdir(deviceDirectory)) != NULL)
        {
            search = UUIDpath + KeyronaPathSeparator;
            search += ent->d_name;
            debug << "Searching " << search << endl;
            ssize_t len = readlink(search.c_str(), buf, sizeof(buf)-1);
            debug << "Result is " << buf << endl;
            if (len != -1) {
            buf[len] = '\0';
            }
            else {
            /* handle error condition */
            }
            search = buf;
            i = search.rfind("/");
            j = search.length()-1;
            debug << "Found volume " << buf << " with the UUID "<< ent->d_name << endl;
            if ( i > 0)
            {
                StringPair myVolume("/dev"+search.substr(i,j), ent->d_name);
                myAvailableVolumes.push_back(myVolume);
            }
        }
        closedir(deviceDirectory);
    }
    else
    {
        cout << "Error opening directory '" << UUIDpath << "'!" << endl;
    }
    return myAvailableVolumes;
};

//================================================================================
//
string selectFromStringPairVector(vector<StringPair> &AvailablePairs, string whatToSelect, string messageToUser)
{
    if (!AvailablePairs.size())
        throw InvalidSelection("selectFromStringPairVector: No data available!");

    vector<utils::StringPair>::const_iterator Iterator;
    UInt32 selection = 1;

    cout << whatToSelect << endl;
    sort(AvailablePairs.begin(), AvailablePairs.end());

    Iterator=AvailablePairs.begin();
    while ( Iterator != AvailablePairs.end())
    {
        cout << "    ";
        cout.width(2);
        cout.fill(' ');
        cout << selection;
        cout.width();
        cout.fill();
        cout << ") \t" << Iterator->first << "\t" << Iterator->second << endl;
        selection++;
        Iterator++;
    }
    cout << endl;
    string UserInput = getStringFromUser(messageToUser + " ");
    if (UserInput.empty())
        throw InvalidSelection("selectFromStringPairVector: Invalid selection!");

    selection = convertStringtoUInt32(UserInput);
    if ((!selection) | (selection > AvailablePairs.size()))
    {
        debug << "selectFromStringPairVector(): No number selected, returning input '" << UserInput << "'" << endl;
        return UserInput;
    }
    else
    {
        Iterator = AvailablePairs.begin() + (selection - 1);
        return Iterator->second;
    }
};

//================================================================================
//
string selectFromStringVector(vector<string> &AvailableStrings, string whatToSelect, string messageToUser)
{
    if (!AvailableStrings.size())
        throw InvalidSelection("selectFromStringVector: No data available!");

    vector<string>::const_iterator Iterator;
    UInt32 selection = 1;

    sort(AvailableStrings.begin(), AvailableStrings.end());
    cout << whatToSelect << endl;
    Iterator=AvailableStrings.begin();
    while ( Iterator != AvailableStrings.end())
    {
        cout << "    ";
        cout.width(2);
        cout.fill(' ');
        cout << selection;
        cout.width();
        cout.fill();
        cout << ") \t" << *(Iterator) << endl;
        selection++;
        Iterator++;
    }
    cout << endl;
    string UserInput = getStringFromUser(messageToUser + " ");
    cout << endl;
    if (UserInput.empty())
        throw InvalidSelection("selectFromStringPairVector: Invalid selection!");
    selection = convertStringtoUInt32(UserInput);
    if ((!selection) | (selection > AvailableStrings.size()))
    {
        debug << "selectFromStringVector(): No number selected, returning input '" << UserInput << "'" << endl;
        return UserInput;
    }
    else
    {
        Iterator = AvailableStrings.begin() + (selection - 1);
        return *(Iterator);
    }
};

//================================================================================
// the code for random string generation was modified from:
// http://hatayquelua.wordpress.com/2007/08/24/make-random-string-by-c/
//
string generateRandomString(int iLength)
{
    string strReturn;
    ByteVector randomBytes = getRandomBytes(iLength);
    for( int i = 0 ; i < iLength ; ++i )
    {
        UInt8 iNumber = randomBytes[i] % 122;
        if( 48 > iNumber )
            iNumber += 48;
        if( ( 57 < iNumber ) && ( 65 > iNumber ) )
            iNumber += 7;
        if( ( 90 < iNumber ) && ( 97 > iNumber ) )
            iNumber += 6;
        strReturn += (char)iNumber;
    }
    return strReturn;
};

//================================================================================
//
void assignFilePermission(string &filename, UInt32 permission)
{
    if (filename.empty())
        throw NoFilename("Invalid filename submitted!");
    if (!permission)
        throw InvalidFilePermissions("Invalid file permissions!");
			// assigning group and file permission
			string myCommand = "/bin/chgrp -f " + KEYRONA_LINUX_SYSTEM_GROUP + " \"" + filename + "\"";
			debug << "Executing: " << myCommand << endl;
			if (system(myCommand.c_str()))
				throw RuntimeError("Keyrona::assignFilePermission(): Could not assign group '" + KEYRONA_LINUX_SYSTEM_GROUP + "' to '" + filename + "'");

			myCommand = "/bin/chmod -f " + convertUInt32toString(permission) + " \"" + filename + "\"";
			debug << "Executing: " << myCommand << endl;
			if (system(myCommand.c_str()))
			throw RuntimeError("Keyrona::assignFilePermission(): Could not assign permission '" + convertUInt32toString(permission) + "' to '" + filename + "'");

};

//================================================================================
//
void renameFile(string currentFilename, string newFilename)
{
    if (currentFilename.empty())
        throw NoFilename("Keyrona::renameFile(): Invalid filename submitted!");
    if (newFilename.empty())
        throw NoFilename("Keyrona::renameFile(): Invalid filename submitted!");
    if (!verifyFilename(currentFilename, false))
        throw FileNotExists("Keyrona::renameFile(): File '" + currentFilename + "' does not exist!");
    if (verifyFilename(newFilename, false))
        throw FileExists("Keyrona::renameFile(): File '" + newFilename + "' already exists!");

    string myCommand = "/bin/mv " + moveCommandOption + " \"" + currentFilename + "\" \"" + newFilename + "\"";
    debug << "Executing: " << myCommand << endl;
    if (system(myCommand.c_str()))
        throw RuntimeError("Keyrona::renameFile(): Could not rename file '" + currentFilename + "' to '" + newFilename + "'");
}

//================================================================================
//
void removeFile(string currentFilename)
{
    if (currentFilename.empty())
        throw NoFilename("Keyrona::removeFile(): Invalid filename submitted!");

    if (!verifyFilename(currentFilename, false))
        throw FileNotExists("Keyrona::removeFile(): File '" + currentFilename + "' does not exist!");

    if (!fileWritable(currentFilename))
        throw FileNotWritable("Keyrona::removeFile(): File '" + currentFilename + "' is not writable!");

    string myCommand = "/bin/rm \"" + currentFilename + "\"";
    debug << "Executing: " << myCommand << endl;
    if (system(myCommand.c_str()))
        throw RuntimeError("Keyrona::removeFile(): Could not remove file '" + currentFilename + "'!");
    if (verifyFilename(currentFilename, false))
        throw FileExists("Keyrona::removeFile(): File '" + currentFilename + "' still exists!");
}

//================================================================================
//
void replaceCharInString(string *myString, UInt8 from, UInt8 to)
{
    UInt8 validChars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                         'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                         '0','1','2','3','4','5','6','7','8','9','_','-','.' };
    if (myString->empty())
        throw RuntimeError("replaceCharInString(): Empty string received!");
    UInt8 i;
    bool isValid = false;
    for (i=0; i<sizeof(validChars); i++)
    {
        if (validChars[i] == to)
            isValid = true;
    }

    if (! isValid)
        throw RuntimeError("replaceCharInString(): Replace-Character was invalid!");

    for (i=0; i< myString->length(); i++)
    {
        if ((*myString)[i] == from)
            (*myString)[i] = to;
    }
};

//================================================================================
//
void removeStringFromStringVector(vector<string> *vectorOfStrings, string toRemove)
{
    vector<string> backup;
    while (vectorOfStrings->size())
    {
        // get an entry
        string temp = vectorOfStrings->back();
        vectorOfStrings->pop_back();
        // push back entry into backup
        if (temp == toRemove)
        {
            debug << "removeStringFromStringVector(): Removing entry '" << toRemove << "'" << endl;
        }
        else
        {
            backup.push_back(temp);
        }
    }
    vectorOfStrings->clear();
    while (backup.size())
    {
        vectorOfStrings->push_back ( backup.back() );
        backup.pop_back();
    }
};

//================================================================================
//
ByteVector convertUInt8Vector2ByteVector(vector<UInt8> &toConvert)
{
    vector<UInt8>::const_iterator Iterator;
    if (toConvert.size())
    {
        ByteVector result(toConvert.size());
        Iterator=toConvert.begin();
        UInt32 counter=0;
        while ( Iterator != toConvert.end())
        {
            result[counter] = *(Iterator);
            Iterator++;
            counter++;
        }
        return result;
    }
    else
    {
        // empty ByteVector
        ByteVector result;
        return result;
    }
};

//================================================================================
//
vector<UInt8> convertByteVector2UInt8Vector(ByteVector &toConvert)
{
    vector<UInt8> result;
    UInt32 counter=0;
    for (counter=0; counter < toConvert.size(); counter++)
        result.push_back(toConvert[counter]);
    return result;
};

//================================================================================
//
string convertByteVector2String(ByteVector &toConvert)
{
	string result( (const char*) toConvert.toCArray(), toConvert.size() );
	return result;
};

//================================================================================
//
void printToSyslog(string message)
{
    KeyronaDate currentDate;
    FILE* keyrona_logfile;
    keyrona_logfile = fopen(logfile.c_str(), "a+");
    if (keyrona_logfile)
        fprintf(keyrona_logfile, "%s: %s\n", currentDate.getDateString().c_str(), message.c_str());
    else
        fprintf(stderr, "KEYRONA: Can't write to logfile!\n");
    fclose(keyrona_logfile);
};

//================================================================================
// code borrowed from: http://program-nix.blogspot.com/2008/08/c-language-check-filesystem-is-mounted.html
//
bool isDeviceMounted(string &device)
{
    FILE * mtab = NULL;
    struct mntent * part = NULL;

    int is_mounted = 0;
    if ( ( mtab = setmntent ("/etc/mtab", "r") ) != NULL)
    {
        while ( ( part = getmntent ( mtab) ) != NULL)
        {
            if ( ( part->mnt_fsname != NULL ) && ( strcmp ( part->mnt_fsname, device.c_str() ) ) == 0 )
            {
                debug << "Device '" << device << "' is mounted!" << endl;
                is_mounted = 1;
            }
        }
        endmntent (mtab);
    }
    if (is_mounted)
        return true;
    else
        return false;
};

//================================================================================
//
ByteVector getRandomBytes(UInt8 amountOfBytes)
{
    debug << "Retrieving " << convertUInt32toString(amountOfBytes) << " byte of random numbers: " << endl;

    try
    {
        KeyronaTPM myTPM;
        return myTPM.getRandom(amountOfBytes);
    }
    catch ( std::exception &e )
    {
        cout << "Could not get random bytes via TPM, using RNG-device '" << RNGDevice << "'!" << endl;

        try
        {
            // open rng-device
            ifstream myRNGSource(RNGDevice.c_str(),ios::in | ios::binary);
            if (! myRNGSource.is_open())
                myRNGSource.open(RNGDevice.c_str());

            // get random data from device
            ByteVector randomBytes(amountOfBytes);
            for (int i=0; i<amountOfBytes; i++)
            {
                UInt8 rngByte;
                myRNGSource >> rngByte;
                randomBytes[i] = rngByte;
            }

            return randomBytes;
        }
        catch ( std::exception &e )
        {
            // Initialize RNG
            srand(time(NULL));
            cout << "Could not get random bytes via TPM, using C++ software-based RNG rand()!" << endl;
            ByteVector randomBytes(amountOfBytes);
            for (int i=0; i<amountOfBytes; i++)
            {
                UInt8 randomTemp = rand();
                randomBytes[i] = randomTemp;
            }
            return randomBytes;
        }
    }
}

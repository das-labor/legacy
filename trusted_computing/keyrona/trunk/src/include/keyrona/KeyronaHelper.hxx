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
 * @file        KeyronaHelper.hxx
 * @brief       Implementation of Keyrona helper methods
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONAHELPER_HXX
#define _KEYRONAHELPER_HXX

#include <Keyrona.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;


/*!
 *  @brief simple helping method to extract quotation marks
 *  @param line, string, contains the original string
 *  @return, string, contains the string without quotation marks
 */
std::string extractQuotationMarks(std::string line);

/*!
 *  @brief simple method to print string vectors
 *  @param *vectorOfStrings, vector of string, strings that should be printed to the screen
 */
void printStringVector(std::vector<std::string> *vectorOfStrings);

/*!
 *  @brief simple method to print string pair vectors
 *  @param *vectorOfStringPairs, vector of StringPair, strings pairs that should be printed to the screen
 */
void printStringPairVector(std::vector<utils::StringPair> *vectorOfStringPairs);

/*!
 *  @brief simple method to print char pointer
 *  @param *toPrint, char, char vector that should be printed
 *  @param length, int, length of the char vector
 */
void printCharPointer(char *toPrint, int length);

/*!
 *  @brief simple method to print UInt8 vectors
 *  @param *vectorOfUInt8, vector of UInt8, vector that should be printed
 */
void printUInt8Vector(vector<UInt8> *vectorOfUInt8);

/*!
 *  @brief simple method to print UInt32 vectors
 *  @param *vectorOfUInt32, vector of UInt32, vector that should be printed
 */
void printUInt32Vector(vector<UInt32> *vectorOfUInt32);

/*!
 *  @brief simple method to convert UInt32 to string
 *  @param SubjectID, UInt32, vector to be converted
 *  @return, string, converted UInt32 vector
 */
string convertUInt32toString( UInt32 SubjectID );

/*!
 *  @brief simple method to convert string to UInt32 
 *  @param SubjectID, string, string to be converted
 *  @return, UInt32, converted string
 */
UInt32 convertStringtoUInt32 (string SubjectIDString );

/*!
 *  @brief checks if a filename is valid 
 *  @param &filename, string, contains the name of the file to be checked
 *  @param, write, boolean, determines whether or not the file should be checked for access rights to write
 *  @return, boolen, determines whether or not the filename is valid
 */
bool verifyFilename(string &filename, bool write);

/*!
 *  @brief checks if a file exists
 *  @param &filename, string, contains the name of the file to be checked
 *  @return, boolen, determines whether or not the file exists
 */
bool fileExists(string &filename);

/*!
 *  @brief checks if a file is writable
 *  @param &filename, string, contains the name of the file to be checked
 *  @return, boolen, determines whether or not the file is writable
 */
bool fileWritable(string &filename);

/*!
 *  @brief checks if a file is readable
 *  @param &filename, string, contains the name of the file to be checked
 *  @return, boolen, determines whether or not the file is readable
 */
bool fileReadable(string &filename);

/*!
 *  @brief checks if a directory exists
 *  @param &dirname, string, contains the name of the directory to be checked
 *  @return, boolen, determines whether or not the directory exists
 */
bool dirExists(string &dirname);

/*!
 *  @brief checks if a directory is writable
 *  @param &dirname, string, contains the name of the directory to be checked
 *  @return, boolen, determines whether or not the directory is writable
 */
bool dirWritable(string &dirname);

/*!
 *  @brief checks if a directory is readable
 *  @param &dirname, string, contains the name of the directory to be checked
 *  @return, boolen, determines whether or not the directory writable
 */
bool dirReadable(string &dirname);

/*!
 *  @brief checks if a device is mounted
 *  @param &device, string, contains the name of the device to be checked
 *  @return, boolen, determines whether or not the device is mounted
 */
bool isDeviceMounted(string &device);

/*!
 *  @brief writes a byte vector to a file
 *  @param &filename, string, contains the name of the file written to
 *  @param &toStore, ByteVector, the byte vector that should be written to a file
 */
void storeByteVectorInFile(string &filename, ByteVector &toStore);

/*!
 *  @brief reads a byte vector from a file
 *  @param &filename, string, contains the name of the file read from
 *  @return ByteVector, contains the loaded vector from the file
 */
ByteVector loadByteVectorFromFile(string &filename);

/*!
 *  @brief simple method to convert UInt8 vector to char 
 *  @param *toConvert, vector of UInt8, vector to be converted
 *  @param, *dest, char, char vector that contains the converted UInt8 vector
 *  @param, &length, int, length of the new char vector
 *  @return, boolean, determines wheter or not the operation was successful
 */
bool convertUInt8VectorToChar(vector<UInt8> *toConvert, char *dest, int &length);

/*!
 *  @brief simple method to convert UInt8 vector to string 
 *  @param *toConvert, vector of UInt8, vector to be converted
 *  @return, string, contains the converted vector
 */
string convertUInt8VectorToString(vector<UInt8> *toConvert);

/*!
 *  @brief simple method to convert char to UInt8 vector 
 *  @param *toConvert, vector of UInt8, will cotain the converted char
 *  @param, *dest, char, char vector that should be converted to UInt8 vector
 *  @param, &length, int, length of the char vector
 *  @return, boolean, determines wheter or not the operation was successful
 */
bool convertCharToUInt8Vector(vector<UInt8> *toConvert, char *src, int length);

/*!
 *  @brief simple method to convert string to UInt8 vector 
 *  @param &toConvert, string, contains the string to be converted
 *  @return, vector of UInt8, contains the converted string
 */
vector<UInt8> convertStringToUInt8Vector(const string &toConvert);

/*!
 *  @brief encodes vector of UInt8 to Base64 string
 *  @param *toConvert, vector of UInt8, contains the vector to be encoded
 *  @return, string, contains the encoded vector
 */
string EncodeUInt8VectorToBASE64(vector<UInt8> *toConvert);

/*!
 *  @brief decodes Base64 string to vector of UInt8 
 *  @param *toConvert, string, contains the string to be decoded
 *  @return, vector of UInt8, contains the decoded string
 */
vector<UInt8> DecodeBASE64StringToUInt8Vector(const string &toConvert);

/*!
 *  @brief decodes Base64 string to ByteVector 
 *  @param *toConvert, string, contains the string to be decoded
 *  @return, ByteVector, contains the decoded string
 */
ByteVector DecodeBASE64StringToByteVector(const string &toConvert);

/*!
 *  @brief encodes ByteVector to Base64 string
 *  @param *toConvert, ByteVector, contains the vector to be encoded
 *  @return, string, contains the encoded vector
 */
string EncodeByteVectorToBASE64(ByteVector &toConvert);

/*!
 *  @brief removes delimiter from a string 
 *  @param myEntry, string, contains the original string
 *  @return, string, contains the string without delimiter
 */
string removeDelimiter(string myEntry);

/*!
 *  @brief resolves volume to its UUID
 *  @param volume, string, contains the name of the volume
 *  @param &UUIDpath, string, contains the path to the folder with the volume -> UUID mapping
 *  @return, string, contains the UUID of the volume
 */
string getUUID(string volume, string &UUIDpath);

/*!
 *  @brief returns all available volume
 *  @param &UUIDpath, string, contains the path to the folder with the volume -> UUID mapping
 *  @return, vector of StringPair, contains all available volumes 
 */
vector<StringPair> getAvailableVolumes(string &UUIDpath);

/*!
 *  @brief selects a string from a string pair vector
 *  @param &AvailablePairs, vector of StringPair, contains the string pairs to select from
 *  @param whatToSelect, string, identifies the string that should be selected
 *  @param messageToUser, string, contains a message to the user
 *  @return, string, contains the selected string
 */
string selectFromStringPairVector(vector<StringPair> &AvailablePairs, string whatToSelect, string messageToUser);

/*!
 *  @brief selects a string from a string  vector
 *  @param &AvailablePairs, vector of string, contains the string pairs to select from
 *  @param whatToSelect, string, identifies the string that should be selected
 *  @param messageToUser, string, contains a message to the user
 *  @return, string, contains the selected string
 */
string selectFromStringVector(vector<string> &AvailableStrings, string whatToSelect, string messageToUser);

/*!
 *  @brief creates a random string
 *  @param iLength, int, defines the length of the random string
 *  @return, string, contains the random string
 */
string generateRandomString(int iLength);

/*!
 *  @brief assign the access permission of a file
 *  @param &filename, string, specifies the file
 *  @param permission, UInt32, defines the new access permission of the file
 */
void assignFilePermission(string &filename, UInt32 permission);

/*!
 *  @brief renames a file
 *  @param currentFilename, string, specifies the current file name
 *  @param newFilename, string, specifies the new file name
 */
void renameFile(string currentFilename, string newFilename);

/*!
 *  @brief removes a file
 *  @param currentFilename, string, specifies the current file name
 */
void removeFile(string currentFilename);

/*!
 *  @brief replaces a char in a string
 *  @param *myString, string, pointer to the string to be changed
 *  @param from, UInt8, ASCII code of the character to be changed
 *  @param to, UInt8, ASCII code of the character, the old character is changed into
 */
void replaceCharInString(string *myString, UInt8 from, UInt8 to);

/*!
 *  @brief removes a string from a string vector
 *  @param *vectorOfStrings, vector of string, pointer to the string vector to be changed
 *  @param toRemove, string, string that should be removed
 */
void removeStringFromStringVector(vector<string> *vectorOfStrings, string toRemove);

/*!
 *  @brief converts a UInt8 vector to a byte vector
 *  @param &toConvert, vector of UInt8, points to the vector to be converted
 *  @return ByteVector, contains the converted vector 
 */
ByteVector convertUInt8Vector2ByteVector(vector<UInt8> &toConvert);

/*!
 *  @brief converts a byte vector to a vector of UInt8
 *  @param &toConvert, ByteVector, points to the vector to be converted
 *  @return vector of UInt8, contains the converted vector 
 */
vector<UInt8> convertByteVector2UInt8Vector(ByteVector &toConvert);

/*!
 *  @brief prints a message to the system log
 *  @param message, string, contains the message printed to the system log
 */
void printToSyslog(string message);

/*!
 *  @brief gets a vector of random bytes
 *  @param amountOfBytes, UInt8, specifies the number of bytes returned
 *  @return ByteVector, the vector with random bytes
 */
ByteVector getRandomBytes(UInt8 amountOfBytes);

#endif

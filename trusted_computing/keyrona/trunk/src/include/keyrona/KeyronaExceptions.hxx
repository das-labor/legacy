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
 * @file        KeyronaExceptions.hxx
 * @brief       Command line base Option 
 * @version     $Revision: 720 $
 * @date        Last modification at $Date: 2009-07-15 11:23:15 +0200 (Wed, 15 Jul 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONAEXCEPTIONS__HXX
#define _KEYRONAEXCEPTIONS__HXX

#include <stdexcept>

namespace keyrona
{
// ===========================
// Keyrona Manager Exceptions:
// ===========================
    /**
     * @brief Exception that is thrown when something is not implemented
     */
    class NotImplemented : public std::runtime_error {
    public:
      NotImplemented( const std::string &what ) :
		std::runtime_error( "Not implemented: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when authentication fails...
     */
    class AuthenticationError : public std::runtime_error {
    public:
      AuthenticationError( const std::string &what ) :
		std::runtime_error( "AuthenticationError: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when admin is not found...
     */
    class NoAdminAvailable : public std::runtime_error {
    public:
      NoAdminAvailable( const std::string &what ) :
		std::runtime_error( "NoAdminAvailable: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when no password magic is available...
     */
    class NoPasswordMagic : public std::runtime_error {
    public:
      NoPasswordMagic( const std::string &what ) :
		std::runtime_error( "NoPasswordMagicAvailable: " + what ) {};
    };
    /**
     * @brief Exception that is thrown when user is not found...
     */
    class UnknownUser : public std::runtime_error {
    public:
      UnknownUser( const std::string &what ) :
		std::runtime_error( "UnknownUser: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when subject is not found...
     */
    class UnknownSubject : public std::runtime_error {
    public:
      UnknownSubject( const std::string &what ) :
		std::runtime_error( "UnknownSubject: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when group is not found...
     */
    class UnknownGroup : public std::runtime_error {
    public:
      UnknownGroup( const std::string &what ) :
		std::runtime_error( "UnknownGroup: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when volume is not found...
     */
    class UnknownVolume : public std::runtime_error {
    public:
      UnknownVolume( const std::string &what ) :
		std::runtime_error( "UnknownVolume: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when no volume is available
     */
    class NoVolume : public std::runtime_error {
    public:
      NoVolume( const std::string &what ) :
		std::runtime_error( "NoVolume: " + what ) {};
    };

    /**
     * @brief Exception that is thrown when a volume already exists
     */
    class VolumeExists : public std::runtime_error {
    public:
      VolumeExists( const std::string &what ) :
		std::runtime_error( "VolumeExists: " + what ) {};
    };
    /**
     * @brief Exception that is thrown when invalid groupID supplied
     */
    class InvalidGroup : public std::runtime_error {
    public:
      InvalidGroup( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid username supplied
     */
    class InvalidUsername : public std::runtime_error {
    public:
      InvalidUsername( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid platform supplied
     */
    class InvalidPlatform : public std::runtime_error {
    public:
      InvalidPlatform( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid token supplied
     */
    class InvalidToken : public std::runtime_error {
    public:
      InvalidToken( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid UUID supplied
     */
    class InvalidUUID : public std::runtime_error {
    public:
      InvalidUUID( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid encryption system supplied
     */
    class InvalidEncSystem : public std::runtime_error {
    public:
      InvalidEncSystem( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when invalid key supplied
     */
    class InvalidKey : public std::runtime_error {
    public:
      InvalidKey( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when no TPM or invalid TPM state
     */
    class InvalidTPM : public std::runtime_error {
    public:
      InvalidTPM( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    
   /**
     * @brief Exception that is thrown when invalid subject type supplied
     */
    class InvalidSubjectType : public std::runtime_error {
    public:
      InvalidSubjectType( const std::string &what ) :
		std::runtime_error( what ) {};
    };


    /**
     * @brief Exception that is thrown when invalid subject ID supplied
     */
    class InvalidSubjectID : public std::runtime_error {
    public:
      InvalidSubjectID( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid email address supplied
     */
    class InvalidEMail : public std::runtime_error {
    public:
      InvalidEMail( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid country code supplied
     */
    class InvalidCountrycode : public std::runtime_error {
    public:
      InvalidCountrycode( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid organisation supplied
     */
    class InvalidOrganisation : public std::runtime_error {
    public:
      InvalidOrganisation( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid organisation unit supplied
     */
    class InvalidOrganisationUnit : public std::runtime_error {
    public:
      InvalidOrganisationUnit( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid state supplied
     */
    class InvalidState : public std::runtime_error {
    public:
      InvalidState( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when invalid locality supplied
     */
    class InvalidLocality : public std::runtime_error {
    public:
      InvalidLocality( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when executed with wrong userID...
     */
    class InvalidUID : public std::runtime_error {
    public:
      InvalidUID( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when user something could not be created...
     */
    class NotCreated : public std::runtime_error {
    public:
      NotCreated( const std::string &what ) :
		std::runtime_error( what ) {};
    };


// ===========================
// Keyrona Storage Exceptions:
// ===========================

    /**
     * @brief Exception that is thrown when a DB file is not opened
     */
    class NoOpenDB : public std::runtime_error {
    public:
      NoOpenDB( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a filename was empty
     */
    class NoFilename : public std::runtime_error {
    public:
      NoFilename( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a filename was invalid
     */
    class InvalidFilename : public std::runtime_error {
    public:
      InvalidFilename( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when file permissions were invalid
     */
    class InvalidFilePermissions : public std::runtime_error {
    public:
      InvalidFilePermissions( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a file exists
     */
    class FileExists : public std::runtime_error {
    public:
      FileExists( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a file exists
     */
    class FileNotExists : public std::runtime_error {
    public:
      FileNotExists( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a file is not writable
     */
    class FileNotWritable : public std::runtime_error {
    public:
      FileNotWritable( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a directory is not writable
     */
    class DirNotExist : public std::runtime_error {
    public:
      DirNotExist( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a directory is not writable
     */
    class DirNotWritable : public std::runtime_error {
    public:
      DirNotWritable( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a directory is not readable
     */
    class DirNotReadable : public std::runtime_error {
    public:
      DirNotReadable( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when a selection was invalid
     */
    class InvalidSelection : public std::runtime_error {
    public:
      InvalidSelection( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when an entry could not be found
     */
    class EntryNotFound : public std::runtime_error {
    public:
      EntryNotFound( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a section is accessed but not selected
     */
    class SectionNotSelected : public std::runtime_error {
    public:
      SectionNotSelected( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a subsection is accessed but not selected
     */
    class SubSectionNotSelected : public std::runtime_error {
    public:
      SubSectionNotSelected( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when SectionName is invalid
     */
    class InvalidSectionName : public std::runtime_error {
    public:
      InvalidSectionName( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when SubSectionName is invalid
     */
    class InvalidSubSectionName : public std::runtime_error {
    public:
      InvalidSubSectionName( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when EntryName is invalid
     */
    class InvalidEntryName : public std::runtime_error {
    public:
      InvalidEntryName( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a value is empty
     */
    class InvalidEntryValue : public std::runtime_error {
    public:
      InvalidEntryValue( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a section is not closed
     */
    class SectionDoesNotClose : public std::runtime_error {
    public:
      SectionDoesNotClose( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a section is not closed
     */
    class SubSectionDoesNotClose : public std::runtime_error {
    public:
      SubSectionDoesNotClose( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when an entry is not closed
     */
    class EntryDoesNotClose : public std::runtime_error {
    public:
      EntryDoesNotClose( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a section is not found although recently added
     */
    class SectionNotFound : public std::runtime_error {
    public:
      SectionNotFound( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when a subsection is not found although recently added
     */
    class SubSectionNotFound : public std::runtime_error {
    public:
      SubSectionNotFound( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when no section is found
     */
    class NoSectionFound : public std::runtime_error {
    public:
      NoSectionFound( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when no subsection is found
     */
    class NoSubSectionFound : public std::runtime_error {
    public:
      NoSubSectionFound( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when suddenly my DB is inconsistent
     */
    class InconsistentDatabase : public std::runtime_error {
    public:
      InconsistentDatabase( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    
     /**
     * @brief Exception that is thrown when getnextGroupID reached the end of the list
     */
    class ReachedEndOfList : public std::runtime_error {
    public:
      ReachedEndOfList( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    
     /**
     * @brief Exception that is thrown when socket() failed
     */
    class SocketCreationError : public std::runtime_error {
    public:
      SocketCreationError( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when bind() failed
     */
    class SocketBindError : public std::runtime_error {
    public:
      SocketBindError( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when accept() failed
     */
    class SocketAcceptError : public std::runtime_error {
    public:
      SocketAcceptError( const std::string &what ) :
		std::runtime_error( what ) {};
    };
    /**
     * @brief Exception that is thrown when connect() failed
     */
    class SocketConnectError : public std::runtime_error {
    public:
      SocketConnectError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

// ===========================
// Keyrona Crypto Exceptions:
// ===========================

    /**
     * @brief Exception that is thrown when cryptlib fails
     */
    class Libcrypt : public std::runtime_error {
    public:
      Libcrypt( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when encryption fails
     */
    class EncryptionFailed : public std::runtime_error {
    public:
      EncryptionFailed( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when decryption fails
     */
    class DecryptionFailed : public std::runtime_error {
    public:
      DecryptionFailed( const std::string &what ) :
		std::runtime_error( what ) {};
    };

    /**
     * @brief Exception that is thrown when no password supplied
     */
    class InvalidPassword : public std::runtime_error {
    public:
      InvalidPassword( const std::string &what ) :
		std::runtime_error( what ) {};
    };

// ===============================
// Keyrona Keyprovider Exceptions:
// ===============================

     /**
     * @brief Exception that is thrown when fork() failed
     */
    class ForkError : public std::runtime_error {
    public:
      ForkError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when something fails during runtime
     */
    class RuntimeError : public std::runtime_error {
    public:
      RuntimeError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when something fails during runtime
     */
    class NoKeyAvailable : public std::runtime_error {
    public:
      NoKeyAvailable( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a device is already mounted
     */
    class AlreadyMounted : public std::runtime_error {
    public:
      AlreadyMounted( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a device is not mounted
     */
    class NotMounted : public std::runtime_error {
    public:
      NotMounted( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a device could not be unmounted
     */
    class UnmountError : public std::runtime_error {
    public:
      UnmountError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when a device could not be mounted
     */
    class MountError : public std::runtime_error {
    public:
      MountError( const std::string &what ) :
		std::runtime_error( what ) {};
    };
// ===============================
// Keyrona Secret Sharing Scheme:
// ===============================
     /**
     * @brief Exception that is thrown when participants < n
     */
    class NotEnoughParticipants : public std::runtime_error {
    public:
      NotEnoughParticipants( const std::string &what ) :
		std::runtime_error( what ) {};
    };
     /**
     * @brief Exception that is thrown when invalid SSSID supplied
     */
    class InvalidSSSID : public std::runtime_error {
    public:
      InvalidSSSID( const std::string &what ) :
		std::runtime_error( what ) {};
    };
}
// ===============================
// Keyrona Messaging:
// ===============================
     /**
     * @brief Exception that is thrown when participants < n
     */
    class NoMessage : public std::runtime_error {
    public:
      NoMessage( const std::string &what ) :
		std::runtime_error( what ) {};
    };

// ===============================
// Keyrona TPM TrouSerS:
// ===============================
     /**
     * @brief Exception that is thrown when connection to tcsd failed
     */
    class NoTCSD : public std::runtime_error {
    public:
      NoTCSD( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when context can not be created
     */
    class ContextError : public std::runtime_error {
    public:
      ContextError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief Exception that is thrown when context can not be connected
     */
    class TPMConnectError : public std::runtime_error {
    public:
      TPMConnectError( const std::string &what ) :
		std::runtime_error( what ) {};
    };

     /**
     * @brief General TSS Exception
     */
    class TSSError : public std::runtime_error {
    public:
      TSSError( const std::string &what ) :
		std::runtime_error( what ) {};
    };
#endif  /* _KEYRONAEXCEPTIONS__HXX */

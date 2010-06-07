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
 * @file        KeyronaKeyprovider.cxx
 * @brief       Implementation of the Keyrona Keyprovider Service.
 * @version     $Revision: 721 $
 * @date        Last modification at $Date: 2009-07-15 12:03:49 +0200 (Wed, 15 Jul 2009) $ by $Author: selhorst $
 */

#include <KeyronaKeyprovider.hxx>
#include <KeyronaSearch.hxx>
#include <KeyronaSSS.hxx>
#include <algorithm>
#include <vector>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string>

using namespace std;
using namespace keyrona;
using namespace utils;

//================================================================================
//
KeyronaKeyprovider::KeyronaKeyprovider(bool foreground, KeyronaConfigfile &myKeyronaConfig)
{
    pid_t pid;
    if (foreground)
    {
        cout << "Starting the daemon in foreground..." << endl;
        startKeyprovider(myKeyronaConfig);
    }
    else
    {
        pid = fork();
        if (pid == -1)
            throw ForkError("Couldn't daemonize, fork failed!");

        if (pid == 0)
        {
            // this is the child process
            setsid(); // no controling terminal
            startKeyprovider(myKeyronaConfig);
            exit(0); // Terminating the parent process
        }
    }
    cout << "done" << endl;
}

//================================================================================
//
void KeyronaKeyprovider::startKeyprovider(KeyronaConfigfile &myKeyronaConfig)
{

   int sockfd, newsockfd, servlen, clilen, n;
   pid_t pid, w;
   struct sockaddr_un  cli_addr, serv_addr;
   char buf[BUFFERSIZE];
   vector<string> tokens;

   (void) signal(SIGINT, KeyronaKeyprovider::quitDaemon);
   (void) signal(SIGTERM, KeyronaKeyprovider::quitDaemon);

   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
       throw SocketCreationError("Creating socket failed!");
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strcpy(serv_addr.sun_path,myServerSocket.c_str());
   servlen=strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
       throw SocketBindError("Binding Server socket failed");
   // setting the socket permission to 760, to allow the access of keyrona_mount
   changepermission();
   // now the daemon is ready to get input

   while(true)
   {
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept( sockfd,(struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
        if (newsockfd < 0) 
            throw SocketAcceptError("Accepting socket failed!");

        pid = fork();

        if (pid == -1)
            throw ForkError("Couldn't fork for the token, fork failed!");

        if (pid == 0)
        {
            
            pid = fork();
            if (pid == 0)
            {
                // this is the child process
                close(sockfd);
                setsid(); // no controlling terminal

                bzero(buf,BUFFERSIZE);
                n=read(newsockfd,buf,BUFFERSIZE);
                tokens.clear();
                Tokenize(buf, tokens);
                string umount = tokens.back();
                tokens.pop_back();
                close(newsockfd);
                umount == "1" ? mount(tokens, myKeyronaConfig) : unmount(tokens, myKeyronaConfig);
                exit(0); // Terminating the child process
            }
            else
            {
                exit(0);
            }
        }
        else
        {
            // this is the parent process
            int status;
            do
            {
                // wait for the child to exit
                w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                if (w == -1)
                    throw ForkError("waitpid() failed!");
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}

//================================================================================
//
string KeyronaKeyprovider::createMountName(vector<string> inputVector)
{
    sort(inputVector.begin(), inputVector.end());
    string mountName = inputVector.back();
    inputVector.pop_back();
    while (!inputVector.empty())
    {
        mountName += "," + inputVector.back();
        inputVector.pop_back();
    }
    return mountName;
}

//================================================================================
//
void KeyronaKeyprovider::Tokenize(const string& str, vector<string>& tokens)
{
    string delimiters = ",";
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

//================================================================================
//
void KeyronaKeyprovider::quitDaemon(int sig)
{
   unlink(myServerSocket.c_str());
   unlink(mySocketPass.c_str());
   exit(0);
}

//================================================================================
//
string KeyronaKeyprovider::getGroupPassword(KeyronaGroup *myGroup, KeyronaStorage &mySubjectStorage)
{
    vector<UInt32> subjectsInGroup = myGroup->getMySubjects();
    if (!subjectsInGroup.size())
    {
        throw InvalidGroup("No subjects in group '" + myGroup->getMyGroupID() +"'");
    }

    string username = clientCommunication(KEYRONA_ISC_GET_STRING_MESSAGE,"Please enter a username of group '" + myGroup->getMyGroupID() + "': ");
    if (username.empty())
        throw InvalidUsername("No valid username supplied!");

    KeyronaSubject *mySubject = new KeyronaSubject(username, mySubjectStorage);

    // verify the entered user
    if (! myGroup->isSubjectInGroup(mySubject))
        throw InvalidUsername("Subject '" + username + "' is not in group '" + myGroup->getMyGroupID() + "'!");

    string subjectPassword = getSubjectPassword(mySubject);
    return (myGroup->getDecryptedGroupKey(mySubject, subjectPassword));
};

//================================================================================
//
string KeyronaKeyprovider::getSubjectPassword(KeyronaSubject *mySubject)
{
    string myPassword;

    // check if token is available
    if (mySubject->getMySubjectType() == SUBJECTTYPE_TOKEN)
    {
        string keyToken = mySubject->getMySubjectKeyfile();
        if (! verifyFilename(keyToken, false))
        {
            clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, "Token '" + mySubject->getMySubjectName() + "' not available under '" + keyToken + "'!\nSince Token is not accessable, the volume can not be decrypted.");
            exit(-1);
        }

        // check if token uses standard password
        try
        {
            myPassword = KEYRONA_TOKEN_STANDARD_PASSWD;
            mySubject->verifyPassword(myPassword);
        }
        catch ( std::exception &e )
        {
            myPassword = "";
        }
    }

    if (myPassword.empty())
    {
        try
        {
            myPassword = clientCommunication(KEYRONA_ISC_PASS_MESSAGE,"Please enter passphrase for user '" + mySubject->getMySubjectName() + "': ");
        }
        catch ( SocketAcceptError &e )
        {
                debug << "Client Communication failed, abording" << endl;
                exit(-1);
        }

        try
        {
            mySubject->verifyPassword(myPassword);
        }
        catch ( std::exception &e )
        {
                clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, "Invalid password for user '" + mySubject->getMySubjectName() + "'!");
                exit(-1);
        }
    }
    // send user messages
    sendMessagesForUser(mySubject);
    return myPassword;
};

//================================================================================
//
void KeyronaKeyprovider::mountBySubject(KeyronaConfigfile &myConfigfile, KeyronaVolume &myVolume, KeyronaStorage &mySubjectStorage, vector<string> &parameter, string &path, string &volume, string &mountName )
{
    string myVolumePath = myVolume.getMyVolumePath();
    UInt8 myCryptoSystem = myVolume.getMyVolumeType();
    string username = parameter.back();
    KeyronaTPM myTPM;

    // get datatounbind and myPasswort
    vector<ByteVector> myDataToUnbind;
    KeyronaSubject mySubject(username, mySubjectStorage);
    string myPassword = getSubjectPassword(&mySubject);
    // get key
	string myKey = myVolume.getDecryptedVolumeKey(&mySubject, myPassword);
    // mount
    KeyronaKeyproviderMount(myCryptoSystem, myVolumePath, path, myKey, myConfigfile, username, volume);
    clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"-> success!");
    exit(0);
};

//================================================================================
//
void KeyronaKeyprovider::mountByGroup(KeyronaConfigfile &myConfigfile, KeyronaVolume &myVolume, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, vector<string> &parameter, string &path, string &volume, string &mountName )
{
    string myVolumePath = myVolume.getMyVolumePath();
    UInt8 myCryptoSystem = myVolume.getMyVolumeType();
    string groupname = parameter.back();

    if (! KeyronaFindGroup(myGroupStorage, groupname))
    {
        throw InvalidGroup("Group '" + groupname + "' does not exist!");
    }

    // let's browse the groups
    vector<string> myVolumeGroups = myVolume.getGroups();
    if (!myVolumeGroups.size())
    {
        throw NoKeyAvailable("No key available to mount volume '" + myVolume.getMyVolumeUUID()  + "' by group '" + groupname + "'");
    }

    vector<string>::const_iterator Iterator;
    Iterator=myVolumeGroups.begin();
    while ( Iterator != myVolumeGroups.end())
    {
        if ((*Iterator) == groupname)
        {
            // load group and get group password
            KeyronaGroup *myGroup = new KeyronaGroup(*(Iterator), myGroupStorage, mySubjectStorage);
            string myPassword = getGroupPassword(myGroup, mySubjectStorage);
            // get key
            string myVolumeKey = myVolume.getDecryptedVolumeKey(myGroup, myPassword);
            // mount
            KeyronaKeyproviderMount(myCryptoSystem, myVolumePath, path, myVolumeKey, myConfigfile, mountName, volume);
            clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"-> success!");
            delete myGroup;
            exit(0);
        }
        else
        {
            Iterator++;
        }
    }
    throw NoKeyAvailable("No key available to mount volume '" + myVolume.getMyVolumeUUID()  + "' by group '" + groupname + "'");
};

//================================================================================
//
void KeyronaKeyprovider::mountBySSS(KeyronaConfigfile &myConfigfile, KeyronaVolume &myVolume, KeyronaStorage &mySSSStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, vector<string> &parameter, string &path, string &volume, string &mountName )
{
    // --------------------------------------
    // FIND THE SSS
    // --------------------------------------
    string myVolumePath = myVolume.getMyVolumePath();
    UInt8 myCryptoSystem = myVolume.getMyVolumeType();

    vector<string> mySSSs = myVolume.getAvailableSSS();
    vector<string>::const_iterator SSSIterator;

    // test, if we have any SSS
    bool SSSfound = false;
    if (!mySSSs.size())
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"Could not find any Secret-Sharing-Scheme for volume " + volume + "!");
        exit(-1);
    }

    // try to identify the correct SSS by given users
    SSSIterator=mySSSs.begin();
    // -> browse all SSSs
    while ( SSSIterator != mySSSs.end())
    {
        KeyronaSSS mySSS(mySSSStorage, *(SSSIterator));
        vector<string> subjectsInSSS = mySSS.getSubjectsInSSS();
        UInt32 n = mySSS.getMinimumParticipants();
        vector<string> userBackup = parameter;

        if (n > userBackup.size())
        {
            SSSIterator++;
            continue;
        }

        vector<string>::const_iterator SubjectIterator;
        SubjectIterator=subjectsInSSS.begin();

        // -> browse all subjects in SSS
        while (( SubjectIterator != subjectsInSSS.end()) && (n))
        {
            userBackup = parameter;
            // -> browse given parameter users
            while (userBackup.size())
            {
                string currentUser = userBackup.back();
                userBackup.pop_back();

                // yippie, found a given user in our current SSS
                if (currentUser == *(SubjectIterator))
                {
                    n--;
                    break;
                }
            }
            SubjectIterator++;
        }
        // if n is zero, we found an SSS that contains all submitted param users
        if (!n)
        {
            SSSfound=true;
            break;
        }
        SSSIterator++;
    }

    // SSS found?
    if (!SSSfound)
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"Could not find any valid Secret-Sharing-Scheme with the supplied users!");
        exit(-1);
    }

    // --------------------------------------
    // OK, now load the subjects and get their passwords
    // --------------------------------------
    string SSSID = *(SSSIterator);
    debug << "Found valid SSS " << SSSID << " for given users" << endl;
    vector<KeyronaSubject*> Participants;
    vector<string> subjectPasswords;
    KeyronaGroup *myGroup = NULL;
    string myGroupPassword;
    bool platformIncluded = false;
    string usernames;
    
    while (parameter.size())
    {
        string currentSubjectName = parameter.back();
        usernames += currentSubjectName;
        if (parameter.size() > 1)
            usernames += ",";
        parameter.pop_back();

        // check if the current participant is a group
        if (currentSubjectName.rfind("Group_") != string::npos)
        {
            myGroup = new KeyronaGroup(currentSubjectName, myGroupStorage, mySubjectStorage);
            myGroupPassword = getGroupPassword(myGroup, mySubjectStorage);
        }
        else
        {
            if (!KeyronaFindSubject(mySubjectStorage, currentSubjectName))
            {
                clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"Subject '" + currentSubjectName + "' not found!");
                exit(-1);
            }
            KeyronaSubject *currentSubject = new KeyronaSubject(currentSubjectName, mySubjectStorage);

            Participants.push_back ( currentSubject );

            if ((currentSubject->getMySubjectType() == SUBJECTTYPE_USER) || (currentSubject->getMySubjectType() == SUBJECTTYPE_TOKEN))
            {
                string myPassword = getSubjectPassword(currentSubject);
                subjectPasswords.push_back(myPassword);
            }
            else
            {
                platformIncluded = true;
                subjectPasswords.push_back("");
            }
        }
    }

    // load the SSS
    KeyronaSSS mySSS(mySSSStorage, SSSID);
    string myVolumeKey;

    // create nice output string with all information
    string n = convertUInt32toString(mySSS.getMinimumParticipants());
    string m = convertUInt32toString(mySSS.getSubjectsInSSS().size());

    // now send an info to the user(s)
    clientCommunication(KEYRONA_ISC_MESG_MESSAGE, "Now decrypting secret-sharing-scheme (n/m = " + n + "/" + m + ") with " + usernames);
    if (platformIncluded)
        clientCommunication(KEYRONA_ISC_MESG_MESSAGE,"Note: A Platform is included in this SSS. The decryption process might require some additional time due to TPM access.");

    // and decrypt the SSS
    debug << "Retrieving key from SSS" << endl;
    if (myGroup)
    {
        myVolumeKey = mySSS.retrieveKey(Participants, subjectPasswords, myGroup, myGroupPassword);
    }
    else
    {
        myVolumeKey = mySSS.retrieveKey(Participants, subjectPasswords);
    }

    // Finally, try the mount
    KeyronaKeyproviderMount(myCryptoSystem, myVolumePath, path, myVolumeKey, myConfigfile, mountName, volume);
    clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"-> success!");
    for (int i=Participants.size(); i>0; i--)
    {
        delete Participants[i-1];
    }
    if (myGroup)
    {
        delete myGroup;
    }
    exit(0);
};

//================================================================================
//
void KeyronaKeyprovider::mount(vector<string> parameter, KeyronaConfigfile &myConfigfile)
{
    // get parameter
    string path = parameter.back();
    parameter.pop_back();
    string volume = parameter.back();
    parameter.pop_back();
    string mountName = parameter.at(0);

    // setup databases
    try
    {
        // Create Volume database storage object
        KeyronaStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_VolumeDBIdentifier) );
        // Create User database storage object
        KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );
        // Create Group database storage object
        KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );
        // Create SSS database storage object
        KeyronaStorage mySSSStorage( "SSSDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SSSDBIdentifier) );

        KeyronaVolume myVolume(volume, myVolumeStorage);

        //////////////////////////////////////////////////////////////
        // use SSS?
        if (parameter.size() > 1)
        {
            mountName = createMountName(parameter);
            debug << "More than one user supplied, assuming Secret-Sharing-Scheme for " << mountName << endl;
            mountBySSS(myConfigfile, myVolume, mySSSStorage, mySubjectStorage, myGroupStorage, parameter, path, volume, mountName );
        }

        // check, whether we shall mount by group
        if (parameter.back().rfind("Group_") != string::npos)
        {
            mountByGroup(myConfigfile, myVolume, mySubjectStorage, myGroupStorage, parameter, path, volume, mountName );
        }
        // else try to mount via user directly
        else
        {
            mountBySubject(myConfigfile, myVolume, mySubjectStorage, parameter, path, volume, mountName );
        }
    }
    catch ( DecryptionFailed &e )
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, "No key available to mount volume '" + volume + "' with the given user(s).");
        exit(-1);
    }
    catch ( NotMounted &e)
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, e.what());
        exit(-1);
    }
    catch ( AlreadyMounted &e)
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, e.what());
        exit(-1);
    }
    catch ( UnmountError &e)
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, e.what());
        exit(-1);
    }
    catch ( NoKeyAvailable &e)
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, e.what());
        exit(-1);
    }
    catch ( MountError &e)
    {
        clientCommunication(KEYRONA_ISC_MESG_MESSAGE, e.what());
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, "\nNote:\tPlease verify, that your volume '" + volume + "' is really connected to its volume path.\n\tIf the device location has changed (e.g., for portable devices), you can attach the device to the new location via\n\t'keyrona_mount [...] --attach /dev/sdXY' or via\n\t'keyrona_manager -v av'");
        exit(-1);
    }
    catch ( std::exception &e )
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE, e.what());
        exit(-1);
    }
};

//================================================================================
//
void KeyronaKeyprovider::unmount(vector<string> parameter, KeyronaConfigfile &myConfigfile)
{
    string path = parameter.back();
    parameter.pop_back();
    string mountName = parameter.at(0);
    if (parameter.size() > 1)
    {
        mountName = createMountName(parameter);
    }
    
    try
    {
        debug << "Now trying to umount " << path << " mounted by " << mountName << endl;
        KeyronaKeyproviderMount(path, myConfigfile, mountName);
    }
    catch ( std::exception &e )
    {
        clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,e.what());
        exit(-1);
    }
    clientCommunication(KEYRONA_ISC_QUIT_MESSAGE,"-> success!");
    exit(0);
}

//================================================================================
//
void KeyronaKeyprovider::changepermission()
{
    if ( chmod( myServerSocket.c_str(), SOCKETPERMISSION)<0)
        cout << "Couldn't change socket permission" << endl;
}

//================================================================================
//
string KeyronaKeyprovider::clientCommunication(string event, string message)
{
    int sock, sockfd, newsock, servlen, n;
    struct sockaddr_un  serv_addr, clntaddr;
    char buf[BUFFERSIZE];
    memset(buf,0,BUFFERSIZE);
    (void) signal(SIGINT, KeyronaKeyprovider::quitDaemon);
    (void) signal(SIGTERM, KeyronaKeyprovider::quitDaemon);
   
    // Server Socket //////////////////////////////////////////////////
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) 
       throw SocketCreationError("Creating server socket failed!");
    struct timeval timeout;
    timeout.tv_sec = CLIENT_TIMEOUT;
    timeout.tv_usec = 1;
    if ( (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) < 0 )
       throw SocketCreationError("Creating server socket failed!");
    bzero(&clntaddr, sizeof(clntaddr));
    clntaddr.sun_family = AF_UNIX;
    strcpy(clntaddr.sun_path, mySocketPass.c_str());

    if (bind(sock, (struct sockaddr *)&clntaddr, sizeof(clntaddr)) < 0) 
       throw SocketBindError("Binding Client socket failed!");
    
    if ( chmod( mySocketPass.c_str(),  SOCKETPERMISSION)<0)
        cout << "Couldn't change socket permission" << endl;

    //Client Socket //////////////////////////////////////////////////
    string buffer = event + message;
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path,myClientSocket.c_str());
    servlen = strlen(serv_addr.sun_path) +  sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
       throw SocketCreationError("Creating socket failed!");
    if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
       throw SocketConnectError("Connection error!");
    debug << "Sending message to client: " << buffer << endl;
    int send;
    send = write(sockfd,buffer.c_str(),BUFFERSIZE);
    debug << send << "Bytes sent" << endl;
    close(sockfd);
    
    //Listening to the clients response////////////////////////////
    bzero(buf,BUFFERSIZE);
    if( event == KEYRONA_ISC_PASS_MESSAGE )
    {
        listen(sock,1);
        int clilen = sizeof(clntaddr);
        newsock = accept( sock,(struct sockaddr *)&clntaddr,(socklen_t *)&clilen);
        if (newsock < 0){
            debug << "TIMEOUT, closing sockets now" << endl;
            unlink(mySocketPass.c_str());
            if (close(sock) < 0 )
            debug << "TIMEOUT, socket closed" << endl;
            throw SocketAcceptError("Accepting failed!");
        }         
        debug << "Now reading the data" << endl;
        n=read(newsock,buf,BUFFERSIZE);
    }
    else if( event == KEYRONA_ISC_GET_STRING_MESSAGE )
    {
        listen(sock,1);
        int clilen = sizeof(clntaddr);
        newsock = accept( sock,(struct sockaddr *)&clntaddr,(socklen_t *)&clilen);
        if (newsock < 0){
            debug << "TIMEOUT, closing sockets now" << endl;
            unlink(mySocketPass.c_str());
            if (close(sock) < 0 )
            debug << "TIMEOUT, socket closed" << endl;
            throw SocketAcceptError("Accepting failed!");
        }
        debug << "Now reading the data" << endl;
        n=read(newsock,buf,BUFFERSIZE);
    }
    string help(buf);
    unlink(mySocketPass.c_str());
    close(sock);
    return help;
};

//================================================================================
//
void KeyronaKeyprovider::sendMessagesForUser(KeyronaSubject *mySubject)
{
    // password is verified, print current messages for user
    string username = mySubject->getMySubjectName();
    vector<StringPair> userMessages = mySubject->getCurrentMessagesForUser();
    if (userMessages.size())
    {
        clientCommunication(KEYRONA_ISC_MESG_MESSAGE, "========== KEYRONA SYSTEM MESSAGE ==========");
        clientCommunication(KEYRONA_ISC_MESG_MESSAGE, "To '" + username + "': you have the following new messages:");
        clientCommunication(KEYRONA_ISC_MESG_MESSAGE, "============================================");

        while (userMessages.size())
        {
            StringPair currentMessage = userMessages.back();
            userMessages.pop_back();
            clientCommunication(KEYRONA_ISC_MESG_MESSAGE, currentMessage.first);
            clientCommunication(KEYRONA_ISC_MESG_MESSAGE, currentMessage.second);
        }
    }
    mySubject->deleteUserMessages();
};

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
 * @file        KeyronaMountHelper.cxx
 * @brief       Keyrona Mount helper methods
 * @version     $Revision: 778 $
 * @date        Last modification at $Date: 2009-07-31 14:50:44 +0200 (Fri, 31 Jul 2009) $ by $Author: selhorst $
 */

#include <Keyrona.hxx>
#include <KeyronaMountHelper.hxx>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <time.h> 

using namespace keyrona;

pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;
string userpassword;

MountHelper::MountHelper ( bool mount, string users, string volume, string path )
{
    string mountcommand;
    if(mount)
    {
        mountcommand = users +  "," + volume + "," + path +  ",1";
        sendMessage(mountcommand);
    }
    else
    {
        mountcommand = users + "," + path + ",0";
        sendMessage(mountcommand);
    }
};


void* passwordThread(void* message)
{
   userpassword = getPassword((char*) message);
   pthread_mutex_lock( &condition_mutex );
   pthread_cond_signal( &condition_cond );
   pthread_mutex_unlock( &condition_mutex );
   return NULL;
};

void* getStringThread(void* message)
{
   userpassword = getStringFromUser((char*) message);
   pthread_mutex_lock( &condition_mutex );
   pthread_cond_signal( &condition_cond );
   pthread_mutex_unlock( &condition_mutex );
   return NULL;
};

void MountHelper::sendMessage(string tosend)
{
    int sock, sockfd, newsock, servlen, n;
    struct sockaddr_un  serv_addr, clntaddr;
    char buf[BUFFERSIZE];
    memset(buf,0,BUFFERSIZE);
    (void) signal(SIGINT, MountHelper::quitDaemon);
    (void) signal(SIGTERM, MountHelper::quitDaemon);
    bool quit = false;

    // Client Socket 
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
       throw SocketCreationError("Creating socket failed!");
    bzero(&clntaddr, sizeof(clntaddr));
    clntaddr.sun_family = AF_UNIX;
    strcpy(clntaddr.sun_path, myClientSocket.c_str());
    if (bind(sock, (struct sockaddr *)&clntaddr, sizeof(clntaddr)) < 0) 
       throw SocketBindError("Binding client socket failed!");

    //Sending server initial message
    string buffer = tosend;
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path,myServerSocket.c_str());
    servlen = strlen(serv_addr.sun_path) +  sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
       throw SocketCreationError("Creating socket failed!");
    if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
       throw SocketConnectError("Connection to server failed!");
    int send;
    send = write(sockfd,buffer.c_str(),BUFFERSIZE);
    debug << send << "Bytes sent" << endl;
    close(sockfd);

    //Listening to the servers response
    while(!quit){
        listen(sock,1);
        int clilen = sizeof(clntaddr);
        newsock = accept( sock,(struct sockaddr *)&clntaddr,(socklen_t *)&clilen);
        if (newsock < 0) 
            throw SocketAcceptError("Socket accept failed!");
        bzero(buf,BUFFERSIZE);
        n=read(newsock,buf,BUFFERSIZE);
        string help(buf);
        close(newsock);

        string command = help.substr(0,4);
        debug << "Command recieved: " << command << endl;
        help = help.substr(4);
        debug << "Message recieved: " << help << endl;

        if (!command.compare(KEYRONA_ISC_PASS_MESSAGE))
        {
	     // Get password with threat and timeout //
	     pthread_t thread1;
	     int  iret1,retcode;
	     struct timeval now;
	     struct timespec timeout;
	     gettimeofday(&now, NULL);
	     timeout.tv_sec = now.tv_sec + CLIENT_TIMEOUT;
	     timeout.tv_nsec = now.tv_usec * 1000;
             userpassword = "";
             termios before = getCurrentTerminalSettings();
	    /* Create independent threads each of which will execute function */
	     iret1 = pthread_create( &thread1, NULL, passwordThread, (void*) help.c_str());
	     pthread_mutex_lock( &condition_mutex );
             retcode = pthread_cond_timedwait(&condition_cond, &condition_mutex , &timeout);
             
             if (retcode == ETIMEDOUT) {
		cout << endl << "Timeout, please enter the password within " << CLIENT_TIMEOUT << " seconds." << endl;
		restoreTerminalSettings(before);
                unlink(myClientSocket.c_str());
                close(sock);
                close(sockfd);
                exit(-1);
		     /* timeout occurred */
	     } else {
		pthread_join( thread1, NULL);
		      /* operate on x and y */
	     }
	     pthread_mutex_unlock(&condition_mutex);

	    // sending the password to the server //
            bzero((char *)&serv_addr,sizeof(serv_addr));
            serv_addr.sun_family = AF_UNIX;
            strcpy(serv_addr.sun_path,mySocketPass.c_str());
            servlen = strlen(serv_addr.sun_path) +  sizeof(serv_addr.sun_family);
            if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
               throw SocketCreationError("Creating socket failed!");
            if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
               throw SocketConnectError("Connection failed!");
            send = write(sockfd,userpassword.c_str(),BUFFERSIZE);
            debug << send << "Bytes sent" << endl;
        }
        else if (!command.compare(KEYRONA_ISC_GET_STRING_MESSAGE))
        {
	     // Get password with threat and timeout //
	     pthread_t thread1;
	     int  iret1,retcode;
	     struct timeval now;
	     struct timespec timeout;
	     gettimeofday(&now, NULL);
	     timeout.tv_sec = now.tv_sec + CLIENT_TIMEOUT;
	     timeout.tv_nsec = now.tv_usec * 1000;
             userpassword = "";
             termios before = getCurrentTerminalSettings();
	    /* Create independent threads each of which will execute function */
	     iret1 = pthread_create( &thread1, NULL, getStringThread, (void*) help.c_str());
	     pthread_mutex_lock( &condition_mutex );
             retcode = pthread_cond_timedwait(&condition_cond, &condition_mutex , &timeout);

             if (retcode == ETIMEDOUT) {
		cout << endl << "Timeout, please enter the username within " << CLIENT_TIMEOUT << " seconds." << endl;
		restoreTerminalSettings(before);
                unlink(myClientSocket.c_str());
                close(sock);
                close(sockfd);
                exit(-1);
		     /* timeout occurred */
	     } else {
		pthread_join( thread1, NULL);
		      /* operate on x and y */
	     }
	     pthread_mutex_unlock(&condition_mutex);

	    // sending the password to the server //
            bzero((char *)&serv_addr,sizeof(serv_addr));
            serv_addr.sun_family = AF_UNIX;
            strcpy(serv_addr.sun_path,mySocketPass.c_str());
            servlen = strlen(serv_addr.sun_path) +  sizeof(serv_addr.sun_family);
            if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
               throw SocketCreationError("Creating socket failed!");
            if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
               throw SocketConnectError("Connection failed!");
            send = write(sockfd,userpassword.c_str(),BUFFERSIZE);
            debug << send << "Bytes sent" << endl;
        }
        else if (!command.compare(KEYRONA_ISC_MESG_MESSAGE))
        {
            if (!help.empty())
                cout << help << endl;
        }
        else if (!command.compare(KEYRONA_ISC_QUIT_MESSAGE))
        {
            if (!help.empty())
                cout << help << endl;
            quit = true;
        }
    }
    unlink(myClientSocket.c_str());
    close(sock);
    close(sockfd);
    exit(0);
};

void MountHelper::quitDaemon(int sig)
{
   debug << "quitDaemon called" << endl;
   unlink(myClientSocket.c_str());
   unlink(mySocketPass.c_str());
   // restoring terminal settings
   restoreTerminalSettings(terminalBackup);
   cout << endl;
   exit(0);
};

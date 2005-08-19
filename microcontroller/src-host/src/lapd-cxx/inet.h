#ifndef LAPDINET
#define LAPDINET

#include <sys/select.h>
#include <sys/types.h>
#include <list>

// Atmel Includes
#include "config.h"

#include "can.h"
#include "rs232can.h"


class LapClient
{
public:
	LapClient();
	virtual ~LapClient();

	/**
	 * Read/Write 
	 */
	rs232can_msg *getPacket();
	void sendPacket(rs232can_msg *msg);
};

class InetConnections
{
public:  // XXX
	typedef std::list<LapClient *> ClientList;

	std::list<int> watchFds;
	ClientList clients;
	int listen_socket;

public:
	/**
	 * Connstructor -- open listening socket
	 */
	InetConnections(int port);
	
	/**
	 * Destructor -- close all sockets 
	 */
	~InetConnections();
	
	/**
	 * Select methods
	 */
	void addWatchFd(int fd);
	void removeWatchFd(int fd);
	void waitForActivity();
	bool hasActivity(LapClient *);
	bool fdHasActivity(int fd);

	/**
	 * Read/Write 
	 */
	void sendPacket(rs232can_msg *msg);
};


#endif

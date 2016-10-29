/*
 * MessengerSreverApp.h
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#ifndef MESSENGERSERVERAPP_H_
#define MESSENGERSERVERAPP_H_
#include "LoginManger.h"
#include "MThread.h"
#include "MessengerProtocol.h"
#include "TCPSocket.h"
#include "MTCPListener.h"


using namespace std;

class MessengerServerApp : public MThread{
	TCPSocket * socket;
	LoginManger * loginManger;
	bool running;
	mapData data;
	void printList(mapData & data);

public:
	MessengerServerApp();
	void run();
	void listAllUsers();
	void listAllP2PSessions();
	void listAllRoomSessions();
	void listUsersInRoom(const string & roomName);
	void listConnectedUsers();
	void close();
	virtual ~MessengerServerApp();
};

#endif /* MESSENGERSERVERAPP_H_ */

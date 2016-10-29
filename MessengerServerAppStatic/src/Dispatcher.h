/*
 * Dispatcher.h
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_
#include "MThread.h"
#include <vector>
#include <map>
#include "User.h"
#include "Room.h"
#include "MessengerProtocol.h"
#include "MTCPListener.h"
#include "Model.h"
#include "TCPSocket.h"


class Dispatcher : public MThread {

	map<string,User*> connectedUsers;
	map<string,Room *> rooms;
	MTCPListener * listener;
	bool running;
	mapData data;
	pthread_mutex_t mutex;

	void disconnectPeer(User * user);
	User * findPeerBySocket(TCPSocket * peer);
	//P2P function.
	void openSessionWithPeer(User * user,const string & userName);
	void closeSessionWithPeer(User * user);

	//Room function.
	void createRoom(User * user,const string & roomName);
	void joinRoom(User * user,const string  roomName);
	void leaveRoom(User * user);
	void closeRoom(User * user,const string & roomName);

	vector<TCPSocket*> getConnectedPeersSockets();

	//List function.
	void listAllAvailableRooms(User * user);
	void listAllConnectedUsers(User * user);
	void listAllUsers(User * user);


	void removeUserFromRoom(User * user,Room * room);
	void removeRoom(Room * room);
	void addUserToRoom(User * user ,Room * room);
	void sendListOfUsers(User * user,Room * room);
	bool isRoomExits(User * user,const string & roomName);
public:
	Dispatcher();
	void addUserToConnected(TCPSocket * peer,string userName);
	void listUsersInRoomServer(mapData & data);
	void listUsersInRoomClient(User * user,const string & roomName);
	void run();
	void close();

	void lock();
	void unlock();
	void removeAllUsers();
	void removeAllRooms();

	virtual ~Dispatcher();
};

#endif /* DISPATCHER_H_ */

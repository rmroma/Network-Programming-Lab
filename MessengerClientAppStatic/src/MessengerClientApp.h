/*
 * MessengerClientApp.h
 *
 *  Created on: May 26, 2016
 *      Author: asi
 */

#ifndef MESSENGERCLIENTAPP_H_
#define MESSENGERCLIENTAPP_H_

#include <stdio.h>
#include <stdlib.h>
#include "MessengerProtocol.h"
#include "Color.h"
#include <string.h>
#include <vector>
#include "MThread.h"
#include "UDPMessenger.h"
#include "unistd.h"
#include  "TCPSocket.h"
#include "UDPSocket.h"

using namespace std;


class UDPClient{
public:
	string name;
	string ip;
	int port;
	void remove()
	{
		name.clear();
		ip.clear();
		port = 0;
	}

};

typedef vector<UDPClient *> tSession;

class MessengerClientApp: public MThread {
	tSession sessions;
	//string serverAddress;
	string clientName;
	string roomName;
	TCPSocket* socket;
	UDPMessenger* udpSocket;
	UDPClient sessionClinet;
	mapData data;
	bool running;
	bool peerSessionActive;
	bool roomSessionActive;
	bool roomOwner;
	bool loggedIn;//if the client is logged in
	bool connected;//if connected to the server right now
	int command;
	mapData::iterator iter;


	//*-----------------------------*//
	//    Incoming messages          //
	//*-----------------------------*//
	void getAllUserList(mapData & data);
	void getAllOnlineUsers(mapData & data);
	void getAllAvailableRooms(mapData & data);
	void getUsersInRoom(mapData & data);
	void getRoomUsersList(mapData & data);
	void roomCreated(const string & roomName);
	void sessionEstablished();
	void registrationError(const string & userName);
	void userVerified(const string & userName);
	void loginDenied();
	void roomAlreadyExits(const string & roomName);
	void joinedToRoom(const string & data);
	void roomClosed(const string & roomName);
	void sessionWithPeerEnded();
	void userLeaveRoom(const string & userName);
	void addUserToRoom(const string & userName,const string & ipAndPort);
	void newPeerSessionRequest(const string & userName,const string & ipAndPort);
	void peerNotFound(const string & peerName);
	void serverClosed();
	//*-------------------------------------------------*//

	void printList(mapData & data);

public:
	MessengerClientApp();
	void status();
	TCPSocket* getSocket(){return socket;}
	int getCommand(){
		return command;
	}

	//*-----------------------------*//
	//    Bool Functions             //
	//*-----------------------------*//
	bool isConnectedToServer();
	bool isPeerSessionActive();
	bool isRoomSessionActive();
	bool isLogged();
	bool connectToServer(const string & ip);
	//*-------------------------------------------*//

	void print();
	string getSessionName();

	/*For thread */
	void run();
	void startUDP(mapData & data);


	//*-----------------------------*//
	//    Outgoing messages          //
	//*-----------------------------*//

	void requestUserList();//Request for registered user list
	void requestAllOnlineUsers();//Request for online user list
	void requestAllAvailableRooms();//Request for available rooms list
	void requestUsersInRoom(const string & roomName);//Request for users in room
	void requestCreateRoom(const string & roomName);//Request to create room
	void requestJoinRoom(const string & roomName);
    void requestCloseRoom(const string & roomName);
    void login(string userName,string password);
    void signUp(string userName,string  password);
    void openSessionWithPeer(string userName);
    void sendMesseage(string message);
    void closeSession();
    void disconnect();
    //*-----------------------------------------------------------*//

	void close();
	virtual ~MessengerClientApp();
};

#endif /* MESSENGERCLIENTAPP_H_ */

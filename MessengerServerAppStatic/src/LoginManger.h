/*
 * LoginManger.h
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#ifndef LOGINMANGER_H_
#define LOGINMANGER_H_
#include "Dispatcher.h"
#include "MThread.h"
#include "TCPSocket.h"
#include <vector>
#include "MTCPListener.h"
#include "MessengerProtocol.h"
#include "Model.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>

using namespace std;


class LoginManger : public MThread {
	Dispatcher * dispatcher;
	vector <TCPSocket *> peers;
	MTCPListener * listener;
	mapData data;
	bool running;
	pthread_mutex_t mutex;



	void removePeer(TCPSocket * peer);


public:
	LoginManger();
	void signUp(const string & userName,const string & password,TCPSocket * readyPeer);
	void login(const string & userName,const string & password,TCPSocket * readyPeer);
	void listUsersInRoom(mapData & data);
	void run();
	void addPeer(TCPSocket * socket);
	void close();

	virtual ~LoginManger();
};

#endif /* LOGINMANGER_H_ */

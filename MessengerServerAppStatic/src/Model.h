/*
 * Model.h
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#ifndef MODEL_H_
#define MODEL_H_
#include <fstream>
#include <iostream>
using namespace std;
#include "MessengerProtocol.h"
#define DATABASE "database.txt"
using namespace std;


class Model
{
	static Model *s_instance;
	Model(){};
	Model(Model const &){};

	mapData loggedUsers;
	mapData P2Psession;
	mapData roomsNames;
	pthread_mutex_t mutex;

//	pthread_mutex_lock(&mutex);
//	pthread_mutex_unlock(&mutex);

//	pthread_mutex_t* Model::getMutex()
//	{
//		return &mutex;
//	}



public:
	static Model * Instance()
	{
		if(!s_instance)
			s_instance=new Model();
		return s_instance;
	}

	//Database
	void addNewRoom(const string & roomName);
	void removeRoom(const string & roomName);
	void insertNewP2PSession(const string userName1,const string userName2);
	void removeP2PSession(const string userName1,const string userName2);
	void addUserToDatabase(string userName,string password);
	void removeUserFromLogged(const string & userName);
	void addUserToLogged(string userName,string password);
	bool isRoomExists(const string & roomName);


	mapData getAllUserList();
	mapData getConnectUserList();
	mapData getRoomNames();
	mapData getP2PSessions();


	//Login
	bool isUserAlreadyLogged(const string & userName);
	bool login(string userName,string password);
	bool verfiyUserAndPassword(const string & userName,const string & password);
	bool signUp(string userName,string password);
	bool isNameExists(const string & userName);




};

#endif /* MODEL_H_ */

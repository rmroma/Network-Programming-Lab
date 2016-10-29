/*
 * Room.h
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#ifndef ROOM_H_
#define ROOM_H_
#include <string.h>
#include "User.h"
#include <vector>
using namespace std;

class Room {
	string roomName;
	string roomOwner;
	vector<User *> users;

public:
	Room(const string & roomName,const string roomOwner);
	void addUser(User * user);
	void removeUser(User * user);
	void setRoomName(const string & roomName);
	void setRoomOwner(const string & userOwner);
	string getRoomOwner();
	string getRoomName();

	vector<User *> getAllUsers();





	virtual ~Room();


};

#endif /* ROOM_H_ */

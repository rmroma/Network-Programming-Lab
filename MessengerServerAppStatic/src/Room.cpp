/*
 * Room.cpp
 *
 *  Created on: May 27, 2016
 *      Author: user
 */

#include "Room.h"

Room::Room(const string & roomName,const string roomOwner) {
	this->roomName=roomName;
	this->roomOwner=roomOwner;


}

void Room::addUser(User * user){
	users.push_back(user);

}
void Room::removeUser(User * user){
	vector<User *>::iterator iter=users.begin();
	vector<User *>::iterator eiter=users.end();
	for(;iter!=eiter;iter++){
		if((*iter)->getuserName()==user->getuserName()){
			users.erase(iter);
		}
	}
}
void Room::setRoomName(const string & roomName){
	this->roomName=roomName;

}
void Room::setRoomOwner(const string & userOwner){

	this->roomOwner=userOwner;
}
string Room::getRoomOwner(){
	return roomOwner;
}
string Room::getRoomName(){
	return roomName;

}
vector<User *> Room::getAllUsers(){
  return users;

}

Room::~Room() {
	roomName.clear();
	roomOwner.clear();
	users.clear();
}


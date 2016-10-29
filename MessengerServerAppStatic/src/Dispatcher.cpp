/*
 * Dispatcher.cpp
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#include "Dispatcher.h"

Dispatcher::Dispatcher() {
	running=false;
	listener=NULL;
	pthread_mutex_init(&mutex,NULL);//initalazation for mutex

}

void Dispatcher::run(){

	int command;
	while(running){

		listener=new MTCPListener();
		listener->addSockets(getConnectedPeersSockets());
		TCPSocket * readyPeer=listener->listenToSocket(3);
		delete listener;
		if(readyPeer!=NULL){
			if(!MessengerProtocol::readFromSocket(command,data,readyPeer)){
				running = false;
				break;
			}
			User * user=findPeerBySocket(readyPeer);
			mapData::iterator iter=data.begin();
			if(command==0)
				continue;
			switch (command){
			case OPEN_SESSION_WITH_PEER_CMD:
				openSessionWithPeer(user,iter->second);
				break;
			case CLOSE_SESSION_WITH_PEER_CMD:
				closeSessionWithPeer(user);
				break;
			case LEAVE_ROOM_CMD:
				leaveRoom(user);
				break;
			case CREATE_ROOM_CMD:
				createRoom(user,iter->first);
				break;
			case JOIN_ROOM_CMD:
				joinRoom(user,iter->first);
				break;
			case CLOSE_ROOM_CMD:
				closeRoom(user,iter->first);
				break;
			case LIST_ALL_AVALIBLE_ROOMS_CMD:
				listAllAvailableRooms(user);
				break;
			case LIST_ALL_ONLINE_USERS_CMD:
				listAllConnectedUsers(user);
				break;
			case LIST_ALL_USERS_CMD:
				listAllUsers(user);
				break;
			case LIST_USERS_IN_ROOM_CMD:
				listUsersInRoomClient(user,iter->first);
				break;
			case EXIT:
			default:
				disconnectPeer(user);
				break;
			}
		}
	}
}


//Send list of users in room client. Done
void Dispatcher::listUsersInRoomClient(User * user,const string & roomName){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" Request list of users in chat room: ["<<roomName<<"]."<<RESET<<endl;
	if(!isRoomExits(user,roomName))
		return;
	Room* room=rooms[roomName];
	vector<User *>users=room->getAllUsers();
	mapData userList;
	for (unsigned int i=0;i<users.size();i++ )
		userList.insert(pair<string,string>(users[i]->getuserName(),roomName));
	MessengerProtocol::sendToSocket(LIST_USERS_IN_ROOM_RPT,userList,user->getSocket());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"list of all users in room: ["<<roomName<<"] sent to: ["<<user->getuserName()<<"]."<<RESET<<endl;
}


//Check if room exits. Done
bool Dispatcher::isRoomExits(User * user,const string & roomName)
{
	if(!Model::Instance()->isRoomExists(roomName)){
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<roomName<<"] room not exits"<<RESET<<endl;
		MessengerProtocol::sendToSocket(ROOM_NAME_NOT_FOUND_RPT,data,user->getSocket());
		return false;
	}
	return true;
}



//Send list of users in room server. Done
void Dispatcher::listUsersInRoomServer(mapData & data){
	mapData::iterator miter=data.begin();
	//cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<miter->first<<"]: "<<" Request list of users in chat room: ["<<miter->second<<"]."<<RESET<<endl;
	if(!Model::Instance()->isRoomExists(miter->second)){
		MessengerProtocol::insertDataToSend(data,"ROOM_NAME_NOT_FOUND");
		return;
	}
	Room* room=rooms[miter->second];
	vector<User *>users=room->getAllUsers();
	data.clear();
	for(unsigned int i=0;i<users.size();i++)
		data.insert(pair<string,string>(users[i]->getuserName(),""));

}


//send to the user list of all registered users. Done
void Dispatcher::listAllUsers(User * user){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request list of all users."<<RESET<<endl;
	data=Model::Instance()->getAllUserList();
	MessengerProtocol::sendToSocket(LIST_ALL_USERS_RPT,data,user->getSocket());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"list of all users sent to: ["<<user->getuserName()<<"]."<<RESET<<endl;
}


//Send to the user list of all connected users. Done
void Dispatcher::listAllConnectedUsers(User * user){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request list of all connected users."<<RESET<<endl;
	data=Model::Instance()->getConnectUserList();
	MessengerProtocol::sendToSocket(LIST_ALL_ONLINE_USERS_RPT,data,user->getSocket());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"List of all connected users sent to: ["<<user->getuserName()<<"]."<<RESET<<endl;
}



//Send to the user list of all available rooms
void Dispatcher::listAllAvailableRooms(User * user)
{
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request list of all available rooms. "<<RESET<<endl;
	data=Model::Instance()->getRoomNames();
	MessengerProtocol::sendToSocket(LIST_ALL_AVALIBLE_ROOMS_RPT,data,user->getSocket());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"List of all available rooms sent to: ["<<user->getuserName()<<"]."<<RESET<<endl;
}


//Create new room. Done
void Dispatcher::createRoom(User * user,const string & roomName){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to open chat room: ["<<roomName<<"]"<<RESET<<endl;
	//Check if the room already exits.
	if(Model::Instance()->isRoomExists(roomName)){
		MessengerProtocol::sendToSocket(ROOM_NAME_ALREADY_EXSITS_RPT,data,user->getSocket());
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to open chat room denied, room already exists: ["<<roomName<<"]"<<RESET<<endl;
		return;
	}

	user->setIsConnectedToRoom(true);
	user->setRoomName(roomName);
	Room * room=new Room(roomName,user->getuserName());//Init the new room
	room->addUser(user);//Set the properties of the room.
	lock();
	rooms.insert(pair<string,Room *>(roomName,room));	//Insert the room to the rooms map.
	unlock();
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(CREATE_ROOM_RPT,data,user->getSocket());//Send the user message about room created.
	Model::Instance()->addNewRoom(roomName);//Update the model with new room.
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<"Opened chat room named: "<<roomName<<RESET<<endl;
}


//Join user to given room. Done
void Dispatcher::joinRoom(User * user,const string  roomName){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to join chat room: ["<<roomName<<"]"<<RESET<<endl;
	//Check if the given room exits. If not send update to the user that the room not exits.
	if(!isRoomExits(user,roomName)){
		return;
	}
	Room * room=rooms[roomName];
	sendListOfUsers(user,room);
	addUserToRoom(user,room);//Update each user in the room that new user join to the room.
	user->setIsConnectedToRoom(true);
	user->setRoomName(roomName);
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(JOIN_ROOM_RPT,data,user->getSocket());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<"join chat room :"<<roomName<<RESET<<endl;
}


//Close room .Done?
void Dispatcher::closeRoom(User * user,const string & roomName){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: request to close chat room ["<<roomName<<"]."<<RESET<<endl;
	Room * room=rooms[roomName];
	if(room->getRoomOwner()!=user->getuserName()){
		MessengerProtocol::sendToSocket(CLOSE_ROOM_ERROR_NOT_OWNER_RPT,data,user->getSocket());
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to close chat room: ["<<room->getRoomName()<<"] denied, not owner."<<RESET<<endl;
		return;
	}
	removeRoom(room);
	Model::Instance()->removeRoom(room->getRoomName());
	rooms.erase(user->getRoomName());
	delete room;
}

//Function to remove user from chat room.
//If the user is the owner of the room->close the room.
void Dispatcher::leaveRoom(User * user){
	Room * room=rooms[user->getRoomName()];//Find the room
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: request to leave room:" << "["<<room->getRoomName()<<"]" <<RESET<< endl;
	if(room->getRoomOwner()==user->getuserName()){//If the user is the owner of the room
		removeRoom(room);
		Model::Instance()->removeRoom(room->getRoomName());
		rooms.erase(user->getRoomName());
		cout <<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"Room ["<<room->getRoomName()<<"]: closed" <<RESET<< endl;
		delete room;
	}
	//If the user is not the owner of the room.
	else
	{
		removeUserFromRoom(user,room);
		cout <<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: removed from:" <<"["<<room->getRoomName()<<"]" <<RESET<< endl;
	}
}

//Disconnect user from server.
void Dispatcher::disconnectPeer(User * user){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: request to disconnected."<<RESET<<endl;

	//Update the model that the user disconnected->remove the user from database
	Model::Instance()->removeUserFromLogged(user->getuserName());
	//Remove the user from connected users map.
	lock();
	connectedUsers.erase(user->getuserName());
	unlock();
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: disconnected."<<RESET<<endl;
}

User * Dispatcher::findPeerBySocket(TCPSocket * peer){


	map<string,User * >::iterator iter=connectedUsers.begin();
	map<string,User * >::iterator eiter=connectedUsers.end();
	for(;iter!=eiter;iter++){
		if(iter->second->getSocket()==peer)
			return iter->second;

	}
	return NULL;
}


//Get the connected users sockets for listening.
vector<TCPSocket*> Dispatcher::getConnectedPeersSockets(){
	vector<TCPSocket*> peers;
	map<string,User*>::iterator iter=connectedUsers.begin();
	map<string,User*>::iterator eiter=connectedUsers.end();
	for (;iter!=eiter;iter++){
		peers.push_back(iter->second->getSocket());
	}
	return peers;
}

//Add user to connected map.
void Dispatcher::addUserToConnected(TCPSocket * peer,string userName){
	//Create new user
	User * newUser=new User(peer,userName);

	//Add the user to the connected map.
	lock();
	connectedUsers.insert(pair<string,User *>(userName,newUser));
	unlock();
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW"["<<newUser->getuserName()<<"] Connected."<<endl;

	//Start the tread.
	if(running==false){
		running=true;
		start();
	}
}

//Close session with peer. Done
void Dispatcher::closeSessionWithPeer(User * user){
	User * user2=user->getPeerSession();
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to close open session."<<RESET<<endl;

	MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER_RPT,data,user2->getSocket());
	MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER_RPT,data,user->getSocket());
	//Set the users status.
	user->setIsConnectedToPeer(false);
	user->removePeerSession();
	user2->setIsConnectedToPeer(false);
	user2->removePeerSession();
	//Remove the session from the session database
	Model::Instance()->removeP2PSession(user->getuserName(),user2->getuserName());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"Session between ["<<user->getuserName()<<", "<<user2->getuserName()<<"] ended"<<RESET<<endl;


}


//Open session with peer.
void Dispatcher::openSessionWithPeer(User * user,const string & userName){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to open session with "<<"["<<userName<<"] "<<RESET<<endl;
	//Finds the user how wants to open session.
	map<string,User*>::iterator iter=connectedUsers.find(userName);
	if (iter == connectedUsers.end()){
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to open session with "<<"["<<userName<<"] refused. "<<userName<<"not found."<<RESET<<endl;
		MessengerProtocol::sendToSocket(PEER_NOT_FOUND_RPT,data,user->getSocket());
		return;
	}
	if(iter->second->isConnectedToRoom()==true || iter->second->isConnectedToSession()==true )
	{
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<user->getuserName()<<"]: "<<" request to open session with "<<"["<<userName<<"] refused. "<<userName<<" already in session."<<RESET<<endl;
		MessengerProtocol::sendToSocket(SESSION_REFUSED_RPT,data,user->getSocket());
		return;
	}

	//Send to the users message about open session
	MessengerProtocol::insertDataToSend(data,iter->second->getuserName(),iter->second->getSocket()->destIpAndPort());
	MessengerProtocol::sendToSocket(OPEN_SESSION_WITH_PEER_RPT,data,user->getSocket());
	MessengerProtocol::insertDataToSend(data,user->getuserName(),user->getSocket()->destIpAndPort());
	MessengerProtocol::sendToSocket(OPEN_SESSION_WITH_PEER_RPT,data,iter->second->getSocket());
	user->setIsConnectedToPeer(true);
	iter->second->setIsConnectedToPeer(true);
	user->setPeerSession(iter->second);
	iter->second->setPeerSession(user);

	//Update the model about new session between peers.
	Model::Instance()->insertNewP2PSession(user->getuserName(),iter->second->getuserName());
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"Session between ["<<user->getuserName()<<", "<<iter->second->getuserName()<<"] Established"<<RESET<<endl;
}



void Dispatcher::close(){
	if(running){
		running=false;
	/*	map<string,User*>::iterator iter=connectedUsers.begin();
		map<string,User*>::iterator eiter=connectedUsers.end();
		for(;iter!=eiter;iter++){

			delete iter->second;
		}*/
		removeAllUsers();
		removeAllRooms();
		connectedUsers.clear();
		listener=NULL;
		waitForThread();
	}
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"dispatcher closed"<<endl;

}

Dispatcher::~Dispatcher(){
}

void Dispatcher::removeUserFromRoom(User * user,Room * room)
{

	vector<User *> users=room->getAllUsers();
	MessengerProtocol::insertDataToSend(data,user->getuserName());
	for(unsigned int i=0;i<users.size();i++){
		MessengerProtocol::sendToSocket(REMOVE_USER_FROM_ROOM_RPT,data,users[i]->getSocket());
	}
	user->setIsConnectedToRoom(false);
	user->removeRoomName();
	room->removeUser(user);
}

void Dispatcher::removeRoom(Room * room)
{
	vector<User *> users=room->getAllUsers();
	MessengerProtocol::insertDataToSend(data,room->getRoomName());
	for(unsigned int i=0;i<users.size();i++){
		MessengerProtocol::sendToSocket(ROOM_CLOSED_RPT,data,users[i]->getSocket());
		users[i]->setIsConnectedToRoom(false);
		users[i]->removeRoomName();
		room->removeUser(users[i]);
	}
}


//Add user to room and update all users in the room. Done
void Dispatcher::addUserToRoom(User * user ,Room * room)
{
	user->setIsConnectedToRoom(true);
	user->setRoomName(room->getRoomName());
	vector<User*>users=room->getAllUsers();
	MessengerProtocol::insertDataToSend(data,user->getuserName(),user->getSocket()->destIpAndPort());
	for(unsigned int i=0;i<users.size();i++){
		MessengerProtocol::sendToSocket(ADD_USER_TO_ROOM_RPT,data,users[i]->getSocket());
	}
	room->addUser(user);
}


//Send user list. Done
void Dispatcher::sendListOfUsers(User * user,Room * room)
{
	vector<User*>users=room->getAllUsers();
	mapData userList;
	for(unsigned int i=0;i<users.size();i++)
	{
		userList.insert(pair<string,string>(users[i]->getuserName(),users[i]->getSocket()->destIpAndPort()));
	}
	MessengerProtocol::sendToSocket(ROOM_USER_LIST_RPT,userList,user->getSocket());
}

void Dispatcher::removeAllUsers(){
	map<string,User *>::iterator iter=connectedUsers.begin();
	map<string,User *>::iterator eiter=connectedUsers.end();
	for (;iter!=eiter;iter++)
	{
		if(iter->second!=NULL){
			MessengerProtocol::sendToSocket(MESSENGER_CLOSED,data,iter->second->getSocket());
			delete iter->second;
		}
	}
	connectedUsers.clear();
}

void Dispatcher::lock(){
	pthread_mutex_lock(&mutex);
}
void Dispatcher::unlock(){
	pthread_mutex_unlock(&mutex);
}

void Dispatcher::removeAllRooms(){
	map<string,Room *>::iterator iter=rooms.begin();
	map<string,Room *>::iterator eiter=rooms.begin();
	for(;iter!=eiter;iter++)
	{
		delete iter->second;
	}
	rooms.clear();
}








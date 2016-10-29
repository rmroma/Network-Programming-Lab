/*
 * MessengerClientApp.cpp
 *
 *  Created on: May 26, 2016
 *      Author: asi
 */

#include "MessengerClientApp.h"


MessengerClientApp::MessengerClientApp() {
	socket=NULL;
	udpSocket=NULL;
	peerSessionActive=false;
	roomSessionActive=false;
	roomOwner=false;
	loggedIn=false;
	connected=false;
	running=false;
	command=0;
}

//Start UDP listener √
void MessengerClientApp::startUDP(mapData & data){
	iter=data.begin();
	int i=0;
	string ip;
	string port;
	i = iter->second.find(':');
	ip = iter->second.substr(0, i);
	port=iter->second.substr(i+1,iter->second.size());
	int _port=atoi(port.c_str());
	udpSocket=new UDPMessenger(_port);
}
//Connect to server √
bool MessengerClientApp::connectToServer(const string & ip){
	cout <<BOLDCYAN<< "Trying to connect to server"<<RESET<<endl;
	socket = new TCPSocket(ip, MSNGR_PORT);
	if (socket->getSocketFd()<0){
		connected=false;
		return false;
	}
	connected = true;
	running=true;
	start();
	return true;
}


void MessengerClientApp::run()
{
	while(running)
	{
		data.clear();
		if(!MessengerProtocol::readFromSocket(command,data,socket)){
			running = false;
			break;
		}
		iter=data.begin();
		switch(command)
		{
			case SESSION_ESTABLISHED_SERVER:
				sessionEstablished();
				break;
			case REGISTER__DENIED_RPT:
				registrationError(iter->first);
				break;
			case USER_VERIFIED_RPT:
				userVerified(iter->first);
				break;
			case LOGIN_DENIED_RPT:
				loginDenied();
				break;
			case PEER_NOT_FOUND_RPT:
				peerNotFound(iter->second);
				break;
			case LIST_USERS_IN_ROOM_RPT:
				getUsersInRoom(data);
				break;
			case SESSION_REFUSED_RPT:
				cout<<endl<<BOLDGREEN<<"\t----------------------------------------------"<<RESET<<endl;
				cout<<BOLDRED<<        "\t|  Connection to peer refused, peer might be |"<<RESET<<endl;
				cout<<BOLDRED<<        "\t|  busy or disconnected, try again later     |"<<RESET<<endl;
				cout<<BOLDGREEN<<      "\t---------------------------------------------"<<RESET<<endl;
				break;
			case ADD_USER_TO_ROOM_RPT:
				addUserToRoom(iter->first,iter->second);
				break;
			case OPEN_SESSION_WITH_PEER_RPT:
				newPeerSessionRequest(iter->first,iter->second);
				break;
			case ERROR_WHILE_CLOSING_SESSION_RPT:
				cout<<RED<<"Error while closing open session, all opened session closed";
				break;
			case CLOSE_SESSION_WITH_PEER_RPT:
				sessionWithPeerEnded();
				break;
			case LIST_ALL_USERS_RPT://
				getAllUserList(data);
				break;
			case LIST_ALL_ONLINE_USERS_RPT://
				getAllOnlineUsers(data);
				break;
			case LIST_ALL_AVALIBLE_ROOMS_RPT:
				getAllAvailableRooms(data);
				break;
			case CREATE_ROOM_RPT:
				roomCreated(iter->first);
				break;
			case CLOSE_ROOM_ERROR_NOT_OWNER_RPT:
				cout<<BOLDCYAN<<endl<<" \t----------------------------------------"<<RESET<<endl;
				cout<<BOLDRED<<"\t|  You are not the owner of the room.  |"<<RESET<<endl;
				cout<<BOLDCYAN<<" \t----------------------------------------"<<RESET<<endl;
				break;
			case ROOM_NAME_ALREADY_EXSITS_RPT:
				roomAlreadyExits(iter->first);
				break;
			case ROOM_USER_LIST_RPT:
				getRoomUsersList(data);
				break;
			case ROOM_CLOSED_RPT:
				roomClosed(iter->first);
				break;
			case JOIN_ROOM_RPT:
				joinedToRoom(iter->first);
				break;
			case ROOM_NAME_NOT_FOUND_RPT:
				cout<<BOLDCYAN<<endl<<"\t------------------------------------------"<<RESET<<endl;
				cout<<BOLDRED<<       "\t|  The room not exits.                   |"<<RESET<<endl;
				cout<<BOLDCYAN<<      "\t------------------------------------------"<<RESET<<endl;
				break;
			case LEAVE_ROOM_ERROR_NOT_FOUND:
				cout<<BOLDCYAN<<endl<<"\t------------------------------------------"<<RESET<<endl;
				cout<<BOLDRED<<"\t|  You are not connected to this room.   |"<<RESET<<endl;
				cout<<BOLDCYAN<<"\t-----------------------------------------"<<RESET<<endl;
				break;
			case REMOVE_USER_FROM_ROOM_RPT:
				userLeaveRoom(iter->first);
				break;
			case EXIT:
			case MESSENGER_CLOSED:
				serverClosed();
				break;
			default:
				disconnect();
				break;
		}

	}
}


//*-----------------------------*//
//    Get Functions              //
//*-----------------------------*//

//get user list
void MessengerClientApp::getAllUserList(mapData & data)
{
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<"\t|  There are no user registered.  |"<<RESET<<endl;
		cout<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		return;
	}
	cout<<endl<<BOLDCYAN<<"  \t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t|  List of all registered users.  |"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t| ------------------------------  |"<<RESET<<endl;
	printList(data);
}

//Get all user list. Done
void MessengerClientApp::getAllOnlineUsers(mapData & data)
{
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<"\t|  There are no user connected.   |"<<RESET<<endl;
		cout<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		return;
	}
	cout<<endl<<BOLDCYAN<<"  \t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t|  List of all connected users.   |"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t| -----------------------------   |"<<RESET<<endl;
	printList(data);
}

//Get all available rooms. Done
void MessengerClientApp::getAllAvailableRooms(mapData & data){

	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<"\t|  There are no rooms opened.     |"<<RESET<<endl;
		cout<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
		return;
	}
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	cout<<endl<<BOLDCYAN<<"  \t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t|  List of all rooms.             |"<<RESET<<endl;
	cout<<BOLDGREEN<<" \t| --------------------            |"<<RESET<<endl;
	int i=0;
	for(;iter!=eiter;iter++){
		cout <<BOLDGREEN<< "\t|"<<BOLDCYAN<<" Room session #"<<i+1<<": "<< iter->first<<RESET<< endl;
		i++;
	}
	cout<<BOLDGREEN<<"\t-----------------------------------"<<RESET<<endl;
}

void MessengerClientApp::getUsersInRoom(mapData & data){
	mapData::iterator iter=data.begin();
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|  Users in room: "<<iter->second<<"."<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------      |"<<RESET<<endl;
	printList(data);
}

//List Name and IP:Port of users in room. Done
void MessengerClientApp::getRoomUsersList(mapData & data){
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	int port;string _port,ipAndPort,ip;
	for(;iter!=eiter;iter++){
		UDPClient * newSession=new UDPClient();
		newSession->name=iter->first;
		int i=0;
		i = iter->second.find(':');
		ip = iter->second.substr(0, i);
		_port=iter->second.substr(i+1,iter->second.size());
		port=atoi(_port.c_str());
		newSession->ip=ip;
		newSession->port=port;
		sessions.push_back(newSession);
	}
	roomSessionActive=true;
	roomOwner=false;
}

//Return the active session name peer name/ room name. Done
string MessengerClientApp::getSessionName()
{
	if(peerSessionActive)
		return sessionClinet.name;
	else if(roomSessionActive)
		return roomName;

	return "";
}

/*-----------------------------------------------------------------------------------   */


//*-----------------------------*//
//    Request Functions          //
//*-----------------------------*//

//Send command for login. Done
void MessengerClientApp::login(string userName,string password){
	MessengerProtocol::insertDataToSend(data,userName,password);
	MessengerProtocol::sendToSocket(LOGIN_CMD,data,socket);

}

//Send command for login. Done
void MessengerClientApp::signUp(string userName,string  password)
{

	MessengerProtocol::insertDataToSend(data,userName,password);
	MessengerProtocol::sendToSocket(REGISTER_CMD,data,socket);
}

//Send command to get all online users. Done
void MessengerClientApp::requestAllOnlineUsers()
{
	MessengerProtocol::sendToSocket(LIST_ALL_ONLINE_USERS_CMD,data,socket);
}

//Send command to get all user list. Done
void MessengerClientApp::requestUserList()
{
	MessengerProtocol::sendToSocket(LIST_ALL_USERS_CMD,data,socket);
}

//Send command to get users in room. Done
void MessengerClientApp::requestUsersInRoom(const string & roomName){
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(LIST_USERS_IN_ROOM_CMD,data,socket);

}

//Send command to get all available rooms. Done
void MessengerClientApp::requestAllAvailableRooms(){
	MessengerProtocol::sendToSocket(LIST_ALL_AVALIBLE_ROOMS_CMD,data,socket);
}

//Send command to close room. Done
void MessengerClientApp::requestCloseRoom(const string & roomName){
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(CLOSE_ROOM_CMD,data,socket);

}

//Send command to close session with peer/leave room. Done
void MessengerClientApp::closeSession()
{
	if(peerSessionActive){
		//MessengerProtocol::insertDataToSend(data,clientName,sessionClinet.name);
		MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER_CMD,data,socket);
		return;
	}
	if(roomSessionActive){
		//MessengerProtocol::insertDataToSend(data,clientName,roomName);
		MessengerProtocol::sendToSocket(LEAVE_ROOM_CMD,data,socket);
	}
}

//Send command to open room. Done
void MessengerClientApp::requestCreateRoom(const string & roomName){
	closeSession();
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(CREATE_ROOM_CMD,data,socket);

}

//Send command to open session with peer. Done
void MessengerClientApp::openSessionWithPeer(string userName){
	closeSession();
	MessengerProtocol::insertDataToSend(data,clientName,userName);
	MessengerProtocol::sendToSocket(OPEN_SESSION_WITH_PEER_CMD,data,socket);//send command to open session with peer
}

//Send command to join room. Done
void MessengerClientApp::requestJoinRoom(const string & roomName){
	closeSession();
	MessengerProtocol::insertDataToSend(data,roomName);
	MessengerProtocol::sendToSocket(JOIN_ROOM_CMD,data,socket);
}

//Send message. Done
void MessengerClientApp::sendMesseage(string message){
	if (peerSessionActive == true){
		udpSocket->sendTo("[" + clientName + "] " + message,sessionClinet.ip,sessionClinet.port);
		return;
	}

	if(roomSessionActive){
		int size=sessions.size();
		for(int i=0;i<size;i++){
			udpSocket->sendTo("[" + clientName + "] "+message,sessions[i]->ip,sessions[i]->port);
		}
		return;
	}
	cout << "You don't have an open session, Please open session to send message" <<  endl;

}



//*--------------------------------------------------------------------*//


//*-----------------------------*//
//    Bool Functions             //
//*-----------------------------*//

//Checks if peer session active √

bool MessengerClientApp::isPeerSessionActive(){
	return peerSessionActive;
}
//Checks if connected to room √

bool MessengerClientApp::isRoomSessionActive(){
	return roomSessionActive;
}

//Checks if conncted to server √
bool MessengerClientApp::isConnectedToServer(){

	return connected;
}


//Checks if already logged to server √
bool MessengerClientApp::isLogged(){
	return loggedIn;
}

//*----------------------------------------------------------------*//

//*-----------------------------*//
//    Incoming messages          //
//*-----------------------------*//

//Updates users in room when user leave. √
void MessengerClientApp::userLeaveRoom(const string & userName){

	if (userName == clientName){
		cout<<endl<<BOLDCYAN<<"\t----------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<"\t|  You left the chat room.  |"<<RESET<<endl;
		cout<<BOLDCYAN<<"\t----------------------------"<<RESET<<endl;
		peerSessionActive=false;
		roomSessionActive=false;
		roomOwner=false;
		sessions.clear();
		roomName.clear();
		return;
	}
	for(unsigned int i=0;i < sessions.size();i++){
		if (sessions[i]->name == userName){
			cout<<endl<<BOLDCYAN<<"\t--------------------------------------------------"<<RESET<<endl;
			cout<<BOLDGREEN<<     "\t|  ["<<userName<<"]"<< " Has left the chat room   "<<RESET<<endl;
			cout<<BOLDCYAN<<      "\t--------------------------------------------------"<<RESET<<endl;
			sessions.erase(sessions.begin() + i);
			break;
		}
	}
}

//Login/Register verified. √
void MessengerClientApp::userVerified(const string & userName){
	cout<<endl<<BOLDCYAN<<"\t--------------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|     Login successful you are connected   |"<<RESET<<endl;
	cout<<BOLDCYAN<<"\t--------------------------------------------"<<RESET<<endl;
	mapData::iterator iter=data.begin();
	loggedIn=true;
	clientName=userName;
	startUDP(data);

}


void MessengerClientApp::sessionWithPeerEnded(){
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|   Your session with "<<sessionClinet.name<<" has ended "<<RESET<<endl;
	cout<<BOLDCYAN<<"\t-----------------------------------------"<<RESET<<endl;
	sessionClinet.remove();
	peerSessionActive=false;
}

//Add user to client session. Done
void MessengerClientApp::addUserToRoom(const string & userName,const string & ipAndPort){
	UDPClient * newSession=new UDPClient();
	mapData::iterator iter=data.begin();
	string ip,port;
	int i=0;
	i = ipAndPort.find(':');
	ip = ipAndPort.substr(0, i);
	port=ipAndPort.substr(i+1,iter->second.size());
	int _port=atoi(port.c_str());
	newSession->ip=ip;
	newSession->port=_port;
	newSession->name=userName;
	cout<<endl<<BOLDCYAN<< "\t------------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|   New user: " << newSession->name + " (" + newSession->ip << ":" << newSession->port << ")     "<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|   has joined.                          |"<<RESET<<endl;
	cout<<BOLDCYAN<<" \t------------------------------------------"<<RESET<<endl;
	sessions.push_back(newSession);

}

//Close udp room session. Done
void MessengerClientApp::roomClosed(const string & roomName){
	cout<<endl<<BOLDCYAN<<"\t----------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|  The Room: "<<roomName<<" closed       "<<RESET<<endl;
	cout<<BOLDCYAN<<"\t----------------------------"<<RESET<<endl;
	roomOwner=false;
	roomSessionActive=false;
	this->roomName.clear();
	sessions.clear();
}

void MessengerClientApp::loginDenied(){
	cout<<endl<<BOLDCYAN<<"\t----------------------"<<RESET<<endl;
	cout<<BOLDRED<< "\t|     Login Denied   |"<<RESET<<endl;
	cout<<BOLDCYAN<<"\t----------------------"<<RESET<<endl;
	clientName="";
	loggedIn=false;

}

void MessengerClientApp::registrationError(const string & userName){
	cout<<endl<<BOLDCYAN<<"\t----------------------------------------------------------"<<RESET<<endl;
	cout<<BOLDRED<<"\t|   Registration failed a user with name: "+userName+"      "<<RESET<<endl;
	cout<<BOLDRED<<"\t|   already exits. try different user name.                |"<<RESET<<endl;
	cout<<BOLDCYAN<<"\t----------------------------------------------------------"<<RESET<<endl;
	loggedIn=false;
}

void MessengerClientApp::sessionEstablished(){
	cout<<BOLDCYAN<<endl<< "\t----------------------------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|  Connection established. Please login or register,     |"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|  for help use instructions(p)                          |"<<RESET<<endl;
	cout<<BOLDCYAN<<" \t----------------------------------------------------------"<<RESET<<endl;
	connected=true;
}

void MessengerClientApp::joinedToRoom(const string & roomName){
	cout<<endl<<BOLDCYAN<<" \t------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|   Welcome to "+roomName+ " Room           "<<RESET<<endl;
	cout<<BOLDCYAN<<" \t------------------------------------"<<RESET<<endl;
	this->roomName=roomName;
}

void MessengerClientApp::roomAlreadyExits(const string & roomName){
	mapData::iterator iter=data.begin();
	cout<<endl<<BOLDGREEN<<"\t------------------------------------"<<RESET<<endl;
	cout<<BOLDRED<<"\t|  Room with name: "<<roomName<<" already exits. "<<RESET<<endl;
	cout<<BOLDGREEN<<"\t------------------------------------"<<RESET<<endl;
	roomSessionActive=false;
	roomOwner=false;
	this->roomName.clear();
}

void MessengerClientApp::roomCreated(const string & roomName){


	mapData::iterator iter=data.begin();
	cout<<endl<<BOLDGREEN<<"\t------------------------------"<<RESET<<endl;
	cout<<BOLDCYAN<<"\t|  Room named: "<<roomName<<" created. "<<RESET<<endl;
	cout<<BOLDGREEN<<"\t------------------------------"<<RESET<<endl;

	this->roomName=roomName;
	roomSessionActive=true;
	roomOwner=true;

}

void MessengerClientApp::serverClosed()
{
	cout<<endl<<BOLDCYAN<<"\t----------------------------------------------"<<RESET<<endl;
	cout<<BOLDRED<<       "\t|  The connection to the server interrupted  |"<<RESET<<endl;
	cout<<BOLDRED<<       "\t|  please try to connect again later         |"<<RESET<<endl;
	cout<<BOLDCYAN<<      "\t---------------------------------------------"<<RESET<<endl;
	connected=false;
	roomSessionActive=false;
	peerSessionActive=false;
	loggedIn=false;
	clientName.clear();
	roomOwner=false;
	sessionClinet.remove();
	sessions.clear();
	running=false;
}

//Get new peer session. Done
void MessengerClientApp::newPeerSessionRequest(const string & userName,const string & ipAndPort){
	mapData::iterator iter=data.begin();
	int i=0;
	string ip;
	string port;
	i=ipAndPort.find(':');
	ip=ipAndPort.substr(0,i);
	port=ipAndPort.substr(i+1,iter->second.size());
	sessionClinet.ip=ip;
	sessionClinet.name=userName;
	sessionClinet.port=atoi(port.c_str());
	cout<<endl<<BOLDCYAN<<"\t--------------------------------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<"\t|   You are in session With ->" <<sessionClinet.name<< "(" +  ipAndPort + ")."<<RESET<<endl;
	cout<<BOLDCYAN<<"\t--------------------------------------------------------"<<RESET<<endl;
	peerSessionActive=true;
}


void MessengerClientApp::peerNotFound(const string & peerName){
	cout<<BOLDCYAN<<endl<<"\t--------------------------------------------------------|"<<RESET<<endl;
	cout<<BOLDRED<<"\t|  Open session with "<<peerName<<" failed user not found.  "<<RESET<<endl;
	cout<<BOLDCYAN<<"\t--------------------------------------------------------|"<<RESET<<endl;
	peerSessionActive=false;
}

//*------------------------------------------------------------------------*//


//Disconnect user from server. Done
void MessengerClientApp::disconnect(){
	closeSession();
	MessengerProtocol::insertDataToSend(data,clientName);
	MessengerProtocol::sendToSocket(EXIT,data,socket);
	sleep(1);
	running=false;
	connected = false;
	peerSessionActive = false;
	roomSessionActive = false;
	loggedIn = false;
	sessions.empty();
	sessionClinet.remove();
	if (udpSocket) {
		udpSocket->close();
		delete udpSocket;
		udpSocket = NULL;
	}
	if (socket) {
		socket->close();
	}
	waitForThread();
	delete socket;
	socket = NULL;
	cout<<BOLDGREEN<<"You are disconnected from server"<<RESET<<endl;
}

//Close client
void MessengerClientApp::close(){
	if(!connected)
		return;
	disconnect();
}

//Print list of users. Done
void MessengerClientApp::printList(mapData & data){
	int i=0;
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	for(;iter!=eiter;iter++){
		cout<<BOLDGREEN<<   " \t|"<<BOLDCYAN<<"  User #"<<i+1<<": "<<iter->first<<"" <<BOLDGREEN<<RESET<< endl;
		i++;
	}
	cout<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl;
}

void MessengerClientApp::print(){
	cout<<"Please connect to server, use Instructions(p) "<<endl;
	cout<<endl<<BOLDCYAN<<"\t--------------------------------------------------"<<RESET<<endl;
	cout<<BOLDRED<<       "\t|  Please connect to server, use Instructions(p) |"<<RESET<<endl;
	cout<<BOLDCYAN<<       "\t-------------------------------------------------"<<RESET<<endl;
}


void MessengerClientApp::status(){
	if(peerSessionActive){
		cout<<endl<<BOLDCYAN<<"\t----------------------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  You are in session with ["<<getSessionName()<<"]."<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t----------------------------------------------"<<RESET<<endl;
	}
	else if (roomSessionActive){
		cout<<endl<<BOLDCYAN<<"\t----------------------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  You are connected to room ["<<getSessionName()<<"]. "<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t----------------------------------------------"<<RESET<<endl;
	}
	else{
		cout<<endl<<BOLDCYAN<<"\t-------------------------------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  You are connected to server with no open session.  |"<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t-------------------------------------------------------"<<RESET<<endl;
	}



}

MessengerClientApp::~MessengerClientApp() {}

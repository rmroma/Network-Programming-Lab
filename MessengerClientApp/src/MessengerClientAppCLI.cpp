//============================================================================
// Name        : MessengerClientApp.cpp
// Author      : Asi Belachow
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Color.h"
#include "MThread.h"

#include "MessengerClientApp.h"
#include <iostream>

using namespace std;


void printInstructions(){
	cout<<BOLDGREEN<<"*      ❶  To connect to server type: c <server ip>                          *"<<endl;
	cout<<"*      ❷   To open session type with peer type: o <user name>               *"<<endl;
	cout<<"*         Opening session will close any previously active sessions        *"<<endl;
	cout<<"*      ❸   To see your status type: l                                       *"<<endl;
	cout<<"*      ❹   To register type: register <username> password>                  *"<<endl;
	cout<<"*      ❺   To login type: login <username password>                         *"<<endl;
	cout<<"*      ❻   To see list of all users type: lu                                *"<<endl;
	cout<<"*      ❼   To see list of online users type: lcu                            *"<<endl;
	cout<<"*      ❽   To create room type: cnr <room name>                             *"<<endl;
	cout<<"*      ❾   To join room type: or <room name>                                *"<<endl;
	cout<<"*      ❿   To see all the available rooms type: lr                          *"<<endl;
	cout<<"*      ⓫   To see all the users in specific room, type: lru <room name>     *"<<endl;
	cout<<"*      ⓬   To close chat room enter cr <room name>                          *"<<endl;
	cout<<"*      ⓭   To send message type: s <message>                                *"<<endl;
	cout<<"*      ⓮   To close opened session/exit from a room type: cs                *"<<endl;
	cout<<"*      ⓯   To disconnect from server type: d                                *"<<endl;
	cout<<"*      ⓰   To exit type: x                                                  *"<<endl;
	cout<<"*****************************************************************************"<<RESET<<endl;
}

int main() {
	cout<<BOLDGREEN<<"©Asi Belachow"<<RESET<<endl;
	cout<<BOLDGREEN<<"*****************************************************************************"<<endl;
	cout<<"*                    Welcome to Messenger Client App (MCA)                  *"<<endl;
	cout<<"*                  -------------------------------------------              *"<<endl;
	printInstructions();
	string command,userName,password,peerName,roomName,message;
	MessengerClientApp* messenger=new MessengerClientApp();

	while(true){
		command.clear();
		cin>>command;
		if(command=="c")
		{
			if(messenger->isConnectedToServer()==true){
				cout<<RED<<"You are already connected to server "<<RESET<<endl;
				continue;
			}
			else{
			string serverIp;
			cin>>serverIp;
			if(messenger->connectToServer(serverIp)==false)
				cout<<"Error while trying to connect to connect to "<<serverIp<<endl;
			}
		}
		else if(command=="o")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();

				continue;
			}else{
				cin>>userName;
				messenger->openSessionWithPeer(userName);
			}
		}
		else if(command=="l")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			else{
				messenger->status();
			}
		}
		else if(command=="register")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			else{
			cin>>userName;
			cin>>password;
			messenger->signUp(userName,password);
			}
		}
		else if(command=="login")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			if(messenger->isLogged()==true){
				cout<<"You are already logged in"<<endl;
				continue;
			}
			cin>>userName;
			cin>>password;
			messenger->login(userName,password);
		}
		else if(command=="lu")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			messenger->requestUserList();
		}
		else if(command=="lcu")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			messenger->requestAllOnlineUsers();
		}
		else if(command=="or")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			string roomName;
			cin>>roomName;
			messenger->requestJoinRoom(roomName);

		}
		else if(command=="lr")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			messenger->requestAllAvailableRooms();

		}
		else if(command=="lru")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			cin>>roomName;
			messenger->requestUsersInRoom(roomName);
		}
		else if(command=="cr")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			if(messenger->isRoomSessionActive()==false){
				cout<<"You are not connected to any room"<<endl;
						continue;
			}

			string roomName;
			cin>>roomName;
			messenger->requestCloseRoom(roomName);
		}

		else if (command=="cnr"){

			if(messenger->isConnectedToServer()==false){
				messenger->print();
					continue;
				}
				cin>>roomName;
				messenger->requestCreateRoom(roomName);

		}
		else if(command=="s")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			message.clear();
			getline(std::cin,message);
			if(message.size()>0 && message[0]==' ')
				message.erase(0,1);
			messenger->sendMesseage(message);
		}
		else if(command=="cs")
		{
			if(messenger->isConnectedToServer()==false){
				messenger->print();
				continue;
			}
			if(messenger->isPeerSessionActive()==false && messenger->isRoomSessionActive()==false)
				cout<<"You don't have session active, use instruction to connect to session"<<endl;
			messenger->closeSession();
		}
		else if(command=="d"){
			if(messenger->isConnectedToServer()==false){
				cout<<"Please connect to server, use Instructions(p) "<<endl;
				continue;
			}
			else
				messenger->disconnect();
		}
		else if(command=="x")
		{
			messenger->close();
			break;
		}
		else if(command=="p")
		{
			printInstructions();
		}
		else
			cout <<RED << "wrong input"<<RESET << endl;
	}
	delete messenger;
	cout<<BOLDGREEN<<"\t**********************************"<<endl;
	cout<<"\t*   Messenger Client App closed  *"<<endl;
	cout<<"\t**********************************"<<RESET<<endl;

	return 0;
}

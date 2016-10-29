//============================================================================
// Name        : MessengerServerApp.cpp
// Author      : Asi Belachow
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Color.h"
#include "MessengerProtocol.h"
#include "MessengerServerApp.h"

void printInstructions(){


	cout<<BOLDCYAN<<"*     ❶ To see list of all users type: lu                                  *"<<endl;
	cout<<BOLDCYAN<<"*     ❷ To see list of online users type: lcu                              *"<<RESET<<endl;
    cout<<BOLDCYAN<<"*     ❸ To see all the available rooms type: lr                            *"<<RESET<<endl;
    cout<<BOLDCYAN<<"*     ❹ To see all the users in specific room, type: lru <room name>       *"<<RESET<<endl;
    cout<<BOLDCYAN<<"*     ❺ To see all the P2P sessions type: ls                               *"<<RESET<<endl;
	cout<<BOLDCYAN<<"*     ❻ To exit type: x                                                    *"<<RESET<<endl;
	cout<<BOLDCYAN<<"****************************************************************************"<<RESET<<endl;
}


int main() {



	cout<<BOLDCYAN<<"****************************************************************************"<<RESET<<endl;
	cout<<BOLDCYAN<<"*                   Welcome to Messenger Server APP (MCA)                  *"<<RESET<<endl;
	cout<<BOLDCYAN<<"*                 -------------------------------------------              *"<<RESET<<endl;
	printInstructions();

	MessengerServerApp * server= new MessengerServerApp();
	while(true)
	{
		string command;
		cin>>command;
		if(command=="lu"){
			server->listAllUsers();

		}
		else if(command=="lcu")
		{
			server->listConnectedUsers();
		}
		else if(command=="ls")
		{
			server->listAllP2PSessions();
		}
		else if(command=="lr")
		{
			server->listAllRoomSessions();
		}
		else if(command=="lru")
		{
			string roomName;
			cin>>roomName;
			server->listUsersInRoom(roomName);
		}
		else if (command=="x")
		{
			break;
		}else
				cout<<RED<<"Worng Input... Try again"<<RESET<<endl;

	}
	server->close();
	delete server;
	cout<<BOLDCYAN<<"\t**********************************"<<endl;
	cout<<"\t*   Messenger Server App closed  *"<<endl;
	cout<<"\t**********************************"<<RESET<<endl;
	return 0;
}

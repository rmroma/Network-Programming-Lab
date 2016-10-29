/*
 * MessengerSreverApp.cpp
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#include "MessengerServerApp.h"

MessengerServerApp::MessengerServerApp() {
	loginManger=new LoginManger();
	socket=NULL;
	running=false;
	start();

}

void MessengerServerApp::run(){
	running=true;
	socket=new TCPSocket(MSNGR_PORT);
	while(running)
	{
		TCPSocket * peer=socket->listenAndAccept();
		if(!peer)
			continue;
		else{
			cout<<BOLDCYAN<<"new peer connected: "<<peer->destIpAndPort()<<RESET<<endl;
			loginManger->addPeer(peer);
		}
	}
}


void MessengerServerApp::printList(mapData & data){
	int i=0;
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	for(;iter!=eiter;iter++){
		cout<<BOLDGREEN<<   " \t|"<<BOLDCYAN<<"  User #"<<i+1<<": "<<iter->first<<"" <<BOLDGREEN<<RESET<< endl;
		i++;
	}
	cout<<BOLDCYAN<<" \t-----------------------------------"<<RESET<<endl<<endl;
}



void MessengerServerApp::listAllUsers(){
	data.clear();
	data=Model::Instance()->getAllUserList();
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  There are no user registered.   |"<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t-----------------------------------"<<RESET<<endl;
		return;
	}
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|    Registered users list.       |"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------     |"<<RESET<<endl;
	printList(data);
}


void MessengerServerApp::listConnectedUsers(){
	data.clear();
	data=Model::Instance()->getConnectUserList();
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  There are no user connected.   |"<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t-----------------------------------"<<RESET<<endl;
		return;
	}
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|    Logged users list.           |"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------     |"<<RESET<<endl;
	printList(data);
	cout<<endl;

}



void MessengerServerApp::listAllP2PSessions(){
	data.clear();
	data=Model::Instance()->getP2PSessions();
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  There are no P2P session.      |"<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t-----------------------------------"<<RESET<<endl;
		return;
	}
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|    P2P Session list.            |"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------     |"<<RESET<<endl;
	int i=0;
	for(;iter!=eiter;iter++){
		cout <<BOLDGREEN<< "\t|"<<BOLDCYAN<<"  Session #"<<i+1<<": [" << iter->first<<","<<iter->second<<"]"<<RESET<< endl;
		i++;
	}
	cout<<BOLDCYAN<<     "\t-----------------------------------"<<RESET<<endl<<endl;

}



void MessengerServerApp::listAllRoomSessions(){
	data.clear();
	data=Model::Instance()->getRoomNames();
	if(data.size()==0){
		cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  There are no room session.     |"<<RESET<<endl;
		cout<<BOLDCYAN<<      "\t-----------------------------------"<<RESET<<endl;
		return;
	}
	mapData::iterator iter=data.begin();
	mapData::iterator eiter=data.end();
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|    Rooms session lists.         |"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------     |"<<RESET<<endl;
	int i=0;
	for(;iter!=eiter;iter++){
		cout <<BOLDGREEN<< "\t|"<<BOLDCYAN<< "Rooms session #"<< i+1 << ": [" << iter->first <<"]"<<RESET<< endl;
		i++;
	}
	cout<<BOLDCYAN<<         "\t-----------------------------------"<<RESET<<endl<<endl;



}



void MessengerServerApp::listUsersInRoom(const string & roomName){
	//MessengerProtocol::insertDataToSend(data,"",roomName);

	MessengerProtocol::insertDataToSend(data,"Server",roomName);
	loginManger->listUsersInRoom(data);
	mapData::iterator iter=data.begin();
	if(iter->first=="ROOM_NAME_NOT_FOUND"){
		cout<<RED<<"The room: ["<<roomName<<"] not exits "<<RESET<<endl;
		cout<<endl<<BOLDCYAN<<"\t--------------------------------------"<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t|  The room: "<<roomName<<" not exits."<<RESET<<endl;
		cout<<BOLDGREEN<<     "\t| ------------------------------------|"<<RESET<<endl;
		return;
	}

	cout<<endl<<BOLDYELLOW<<"Users in room ["<<roomName<<"]"<<endl;
	cout<<BOLDCYAN<<"--------------------"<<RESET<<endl;
	cout<<endl<<BOLDCYAN<<"\t-----------------------------------"<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t|  Users in room "<<roomName<<".       "<<RESET<<endl;
	cout<<BOLDGREEN<<     "\t| ---------------------------     |"<<RESET<<endl;
	printList(data);


}



void MessengerServerApp::close(){
	cout <<BOLDCYAN<< "Server application is closing ..." <<RESET<<endl;
	if(running)
	{
		running=false;
		if(loginManger)
			loginManger->close();
		socket->close();
		waitForThread();
		delete socket;
		socket=NULL;
	}

	cout<<BOLDCYAN<<"Server closed"<<RESET<<endl;

}



MessengerServerApp::~MessengerServerApp() {
	// TODO Auto-generated destructor stub
}


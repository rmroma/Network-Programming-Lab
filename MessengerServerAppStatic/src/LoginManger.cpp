/*
 * LoginManger.cpp
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#include "LoginManger.h"

LoginManger::LoginManger() {

	running=false;
	dispatcher=new Dispatcher();
	listener=NULL;
}

void LoginManger::run(){

	while (running){
		listener=new MTCPListener();
		listener->addSockets(peers);
		TCPSocket * readyPeer = listener->listenToSocket(3);
		delete listener ;
		if (readyPeer != NULL)
		{
			data.clear();
			int command;
			if(!MessengerProtocol::readFromSocket(command,data,readyPeer)){
				running = false;
				break;
			}
			mapData::iterator iter = data.begin();
			switch (command){
				case REGISTER_CMD:
					signUp(iter->first,iter->second,readyPeer);
					break;
				case LOGIN_CMD:
					login(iter->first,iter->second,readyPeer);
					break;
				case EXIT:
				default:
					removePeer(readyPeer);
					break;

			}
		}
	}
}

void LoginManger::removePeer(TCPSocket * peer){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"peer disconnected."<<RESET<<endl;
	vector<TCPSocket*>:: iterator it=find(peers.begin(),peers.end(),peer);
	if((*it)==peer){
		pthread_mutex_lock(&mutex);
		peers.erase(it);
		pthread_mutex_unlock(&mutex);
	}
}

void LoginManger::signUp(const string & userName,const string & password,TCPSocket * readyPeer){

	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<userName<<"]: "<<" Request to register."<<RESET<<endl;
	if(Model::Instance()->signUp(userName,password)==true)
	{
		MessengerProtocol::insertDataToSend(data,userName,readyPeer->destIpAndPort());
		MessengerProtocol::sendToSocket(USER_VERIFIED_RPT,data,readyPeer);
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<userName<<"]: "<<" Registered and logged in."<<RESET<<endl;
		dispatcher->addUserToConnected(readyPeer,userName);
		Model::Instance()->addUserToLogged(userName,password);
		vector <TCPSocket*>::iterator iter;
		iter = find(peers.begin(), peers.end(),readyPeer);
		if (iter != peers.end())
			peers.erase(iter);

	}
	else{
		string name=userName;
		MessengerProtocol::insertDataToSend(data,name);
		MessengerProtocol::sendToSocket(REGISTER__DENIED_RPT,data,readyPeer);
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDRED<<"["<<userName<<"]: "<<" Register denied."<<RESET<<endl;
	}
}

void LoginManger::login(const string & userName,const string & password,TCPSocket * readyPeer){
	cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<userName<<"]: "<<"Request to login in."<<RESET<<endl;
	if(Model::Instance()->login(userName,password)==true){

			MessengerProtocol::insertDataToSend(data,userName,readyPeer->destIpAndPort());
			MessengerProtocol::sendToSocket(USER_VERIFIED_RPT,data,readyPeer);
			cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW<<"["<<userName<<"]: "<<" Has logged in"<<RESET<<endl;
			dispatcher->addUserToConnected(readyPeer,userName);
			Model::Instance()->addUserToLogged(userName,password);
			vector <TCPSocket*>::iterator iter;
			iter = find(peers.begin(), peers.end(),readyPeer);
			if (iter != peers.end())
				peers.erase(iter);
	}
	else{
		MessengerProtocol::sendToSocket(LOGIN_DENIED_RPT,data,readyPeer);
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDRED<<"["<<userName<<"]: "<<" Login denied."<<RESET<<endl;
	}

}

void LoginManger::addPeer(TCPSocket * socket){
	peers.push_back(socket);

	if(running==false){
		running=true;
		start();
	}
	MessengerProtocol::insertDataToSend(data);
	MessengerProtocol::sendToSocket(SESSION_ESTABLISHED_SERVER,data,socket);

}

void LoginManger::close(){
	if(running){
		running=false;
		if(dispatcher)
			dispatcher->close();
		vector<TCPSocket *>::iterator iter=peers.begin();
		vector<TCPSocket *>::iterator eiter=peers.end();
		for(;iter!=eiter;iter++)
			MessengerProtocol::sendToSocket(MESSENGER_CLOSED,data,(*iter));
		peers.clear();
		//delete listener;
		listener=NULL;
		waitForThread();
		peers.clear();
	}
		cout<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDYELLOW"Login Manager closed"<<RESET<<endl;

}

void LoginManger::listUsersInRoom(mapData & data){
	dispatcher->listUsersInRoomServer(data);
}

LoginManger::~LoginManger() {
}


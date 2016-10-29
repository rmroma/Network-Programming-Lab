

#include "UDPMessenger.h"

UDPMessenger::UDPMessenger(int _port)
{
	socket = new UDPSocket(_port);
	this->port = _port;
	running = true;
	this->start();
}



void UDPMessenger::run()
{
	char buff[1024];
	while(running)
	{
		memset(buff,0,1024);
		memset(buff,0,1024);
		if(socket->recv(buff , 1024)<= 0){
			running = false;
			break;
		}
		cout<<WHITE<<MessengerProtocol::getTime()<<BOLDGREEN<<buff<<endl;
	}
}

void UDPMessenger::sendTo(string message,string ip,int port)
{
	int num;

	num = socket->sendTo(message,ip,port);
	if(num < 0)
	{
		cout<<"Message could not be sent"<<endl;
	}

}

void UDPMessenger::reply(string message)
{
	if(socket->reply(message) < 0)
	{
		cout<< "Message could not be reply"<<endl;
	}
}

void UDPMessenger::close()
{
	socket->close();
		cout<<BOLDGREEN<<"waiting for udp thread to close"<<RESET<<endl;

		this->waitForThread();
		delete socket;
		cout<<BOLDGREEN<<"UDP listener was closed."<<RESET<<endl;
}

UDPMessenger::~UDPMessenger() {

}


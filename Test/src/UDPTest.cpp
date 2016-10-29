/*
 * UDPTest.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: user
 */

#include "UDPTest.h"

UDPTest::UDPTest() {
	// TODO Auto-generated constructor stub

}

bool UDPTest::test()
{
	UDPSocket* sSocket = new UDPSocket(3346);
	UDPSocket* cSocket = new UDPSocket();
	string message = "Test 1234567890";
	cSocket->sendTo(message,"127.0.0.1", 3346);
	char buffer[100];
	memset((void*)buffer,0,100);
	int rc = sSocket->recv(buffer, 100);
	cout<<BOLDGREEN"send msg:"<<message<<RESET<<endl;
	cout<<BOLDGREEN<<"recv msg:"<<buffer<<RESET<<endl;
	if (rc != (int)message.length()){
		return false;
	}
	if (message != buffer){
		return false;
	}
	message = "Test Reply 129012901290";
	sSocket->reply(message);
	memset((void*)buffer,0,100);
	rc = cSocket->recv(buffer, 100);
	cout<<BOLDGREEN"send msg:"<<message<<RESET<<endl;
	cout<<BOLDGREEN<<"recv msg:"<<buffer<<RESET<<endl;
	if (rc != (int)message.length()){
		return false;
	}
	if (message != buffer){
		return false;
	}
	sSocket->close();
	cSocket->close();
	delete sSocket;
	delete cSocket;
	return true;


}

UDPTest::~UDPTest() {
	// TODO Auto-generated destructor stub
}


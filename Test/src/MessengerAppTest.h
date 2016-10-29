/*
 * TestMessengerApp.h
 *
 *  Created on: Jun 2, 2016
 *      Author: user
 */

#ifndef MESSENGERAPPTEST_H_
#define MESSENGERAPPTEST_H_
#define REPORT "report.html"


#include <iostream>
#include "TCPSocket.h"
#include "MessengerProtocol.h"
#include "Color.h"
#include "unistd.h"
#include  "MessengerClientApp.h"
#include "MessengerServerApp.h"
#include "fstream"

class MessengerAppTest{

	MessengerClientApp  * client1;
	MessengerClientApp  * client2;
	string ip;
	string cnt1;
	string cnt2;
	string password;
	string roomName;
	mapData data;


public:
	MessengerAppTest();
	~MessengerAppTest();
	void openReport();
	void close();
	bool test();


};

#endif /* MESSENGERAPPTEST_H_ */

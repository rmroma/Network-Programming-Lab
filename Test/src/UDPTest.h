/*
 * UDPTest.h
 *
 *  Created on: Jul 8, 2016
 *      Author: user
 */

#ifndef UDPTEST_H_
#define UDPTEST_H_

#include <iostream>
#include "UDPSocket.h"
#include "MessengerProtocol.h"


using namespace std;

class UDPTest {
public:
	UDPTest();
	bool test();
	virtual ~UDPTest();
};

#endif /* UDPTEST_H_ */

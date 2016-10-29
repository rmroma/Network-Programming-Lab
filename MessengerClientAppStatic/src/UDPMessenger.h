
#ifndef UDPMESSENGER_H_
#define UDPMESSENGER_H_
#include "MThread.h"
#include <Color.h>
#include <MessengerProtocol.h>
#include "UDPSocket.h"
using namespace std;


class UDPMessenger: public MThread {
public:
	UDPSocket* socket;
	int port;
	bool running;

	UDPMessenger(int port);

	void run();
	void sendTo(string message,string ip,int port);
	void reply(string message);
	void close();
	virtual ~UDPMessenger();
};

#endif /* UDPMESSENGER_H_ */

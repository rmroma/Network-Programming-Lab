/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef MESSENGERPROTOCOL_H_
#define MESSENGERPROTOCOL_H_

using namespace std;

#include "TCPSocket.h"
#include "FileInter.h"
#include "Color.h"
#include <map>
#include <vector>
typedef map<string,string> mapData;
typedef vector<TCPSocket *> tPeers;

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */

#define MSNGR_PORT 3346
#define TEST_PORT  3347


//Commands
#define OPEN_SESSION_WITH_PEER_CMD    1
#define CLOSE_SESSION_WITH_PEER_CMD   2
#define LOGIN_CMD                     3
#define REGISTER_CMD                  4
#define CREATE_ROOM_CMD               5
#define CLOSE_ROOM_CMD                6
#define JOIN_ROOM_CMD                 7
#define LEAVE_ROOM_CMD                8
#define LIST_ALL_USERS_CMD            9
#define LIST_ALL_ONLINE_USERS_CMD     10
#define LIST_ALL_AVALIBLE_ROOMS_CMD   11
#define LIST_USERS_IN_ROOM_CMD        12
#define LIST_ALL_USERS_IN_SESSION     13



//Reports
#define SESSION_ESTABLISHED_SERVER    14
#define OPEN_SESSION_WITH_PEER_RPT    15
#define CLOSE_SESSION_WITH_PEER_RPT   16
#define SESSION_REFUSED_RPT           17
#define PEER_NOT_FOUND_RPT            18
#define USER_VERIFIED_RPT             19
#define LOGIN_DENIED_RPT              20
#define REGISTER__DENIED_RPT          21
#define LIST_ALL_USERS_RPT            22
#define LIST_ALL_ONLINE_USERS_RPT     23
#define LIST_ALL_AVALIBLE_ROOMS_RPT   24
#define LIST_USERS_IN_ROOM_RPT        25
#define CLOSE_ROOM_ERROR_NOT_OWNER_RPT 26
#define ROOM_NAME_ALREADY_EXSITS_RPT  27
#define CREATE_ROOM_RPT               28
#define ROOM_NAME_NOT_FOUND_RPT       29
#define REMOVE_USER_FROM_ROOM_RPT     30
#define JOIN_ROOM_RPT                 31
#define ADD_USER_TO_ROOM_RPT          32
#define ROOM_USER_LIST_RPT            33
#define ERROR_WHILE_CLOSING_SESSION_RPT 34
#define ROOM_CLOSED_RPT               35
#define ROOM_NAME                     36
#define LEAVE_ROOM_USER_NOT_IN_ROOM   37
#define LEAVE_ROOM_ERROR_NOT_FOUND    38
#define CLOSE_SESSION_WITH_ROOM       39
#define MESSENGER_CLOSED              40
#define EXIT                          41



#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"
#define SERVER_SHUTDOWN "The connection to the server interrupted please try to connect again later"




class MessengerProtocol{
private:
	void static sendInt(int val,TCPSocket* sock){
		val = htonl(val);
		sock->write((char*)&val,4);
	}
	void static sendString(const string& val,TCPSocket* sock){
		sendInt(val.length(),sock);
		sock->write(val.c_str(),val.length());
	}

	bool static readInt(int& val,TCPSocket* sock){
		int rc = sock->read((char*)&val,4);
		val = ntohl(val);
		if (rc < 4) return false;
		return true;
	}
	bool static readString(string& val,TCPSocket* sock){
		int len;
		bool rc = readInt(len,sock);
		if(!rc) return false;
		char buff[512];
		sock->read(buff,len);
		buff[len] = '\0';
		val = buff;
		return true;
	}
public:
	void static sendToSocket(int command,const map<string,string>& data, TCPSocket* sock){
		//first send the command
		sendInt(command,sock);

		//send the size of the map
		sendInt(data.size(),sock);

		//send the map
		if(data.size()>0){
			map<string,string>::const_iterator iter = data.begin();
			map<string,string>::const_iterator eiter = data.end();
			for(;iter != eiter ; ++iter){
				string key = iter->first;
				string val = iter->second;
				sendString(key,sock);
				sendString(val,sock);
			}
		}
	}


	bool static readFromSocket(int& command, map<string,string>& data,TCPSocket* sock ){
		bool rc = readInt(command,sock);
		if (!rc) return false;

		data.clear();
		int mapSize = 0;
		rc = readInt(mapSize,sock);
		if (!rc) return false;

		if (mapSize > 0){
			for(int i=0;i<mapSize;++i){
				string key;
				string val;
				if(!readString(key,sock)) return false;
				if(!readString(val,sock)) return false;
				data.insert(pair<string,string>(key,val));
			}
		}
		return true;
	}

	void static insertDataToSend(mapData & data, const string & data1="",const string & data2="")
	{
		data.clear();
		data.insert(pair<string,string>(data1,data2));

	}

	string static getTime()
	{
		stringstream convert;
		time_t now;
		struct tm * current;
		now=time(0);
		current=localtime(&now);
		convert<<"["<<current->tm_hour<<":"<<current->tm_min<<":"<<current->tm_sec<<"]  ";
		return convert.str();

	}


};




#endif /* MESSENGERPROTOCOL_H_ */

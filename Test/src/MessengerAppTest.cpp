/*
 * TestMessengerApp.cpp
 *
 *  Created on: Jun 2, 2016
 *      Author: user
 */

#include "MessengerAppTest.h"
using namespace std;



MessengerAppTest::MessengerAppTest(){

	sleep(2);
	client1=new MessengerClientApp();
	client2=new MessengerClientApp();
	ip="127.0.0.1";
	cnt1="client1";
	cnt2="client2";
	password="1234";
	roomName="Test";
}


bool MessengerAppTest::test()
{
	streambuf* cout_sbuf = cout.rdbuf();
	ofstream  fout("/dev/null");
	cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
	mapData::iterator iter=data.begin();
	int i=1;
	ofstream reportFile;
	reportFile.open(REPORT,ios::trunc);
	reportFile <<"<!DOCTYPE html><html><head><link rel="<<"\"stylesheet\" type="<<"\"text/css\""<<" href="<<"\"style.css\"> </head><body>"; //starting html
	reportFile<<"<h2 id=\"headline1\"><u>LOG FILE FOR TEST MESSNGER APPLICATIONS</u><h2> "<<endl<<endl;

	reportFile<<"<h3 id=\"headline2\">Test 'c' command:</h3>";
	client1->connectToServer(ip);
	sleep(2);
	if(client1->getCommand()==SESSION_ESTABLISHED_SERVER)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt1<<" connect to server successfully"<<RESET<<endl<<endl;;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<i<<") "<<cnt1<<" connect to server successfully</h4>"<<endl;
		i++;
	}
	else
	{
		reportFile<<client1->getCommand()<<endl;
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt1<<" trying to connect to server"<<RESET<<endl<<endl;;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt1<<" trying to connect to server</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'c' command:</h3>";
	client2->connectToServer(ip);
	sleep(2);
	if(client2->getCommand()==SESSION_ESTABLISHED_SERVER){
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" connect to server successfully"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<<" connect to server successfully</h4>"<<endl;
		i++;
	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" trying to connect to server"<<RESET<<endl<<endl;;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" trying to connect to server</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'login' command:</h3>";
	client1->login(cnt1,password);
	sleep(2);
	if(client1->getCommand()==USER_VERIFIED_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt1<<" has logged in"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt1<<" has logged in</h4>"<<endl;
		i++;

	}
	else
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt1<<" trying to log in"<<RESET<<endl<<endl;;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt1<<" trying to login</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	client2->login(cnt2,password);
	sleep(2);
	if(client2->getCommand()==USER_VERIFIED_RPT){
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" has logged in"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<<" has logged in</h4>"<<endl;
		i++;
	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" trying to log in"<<RESET<<endl<<endl;;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" trying to login</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'lu' command:</h3>";
	client2->requestUserList();
	sleep(2);
	if(client2->getCommand()==LIST_ALL_USERS_RPT){
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" get list of registered users"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<<" get list of registered users</h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" requesting all registered users"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" requesting all registered users</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'lcu' command:</h3>";
	client2->requestAllOnlineUsers();
	sleep(2);
	if(client2->getCommand()==LIST_ALL_ONLINE_USERS_RPT){
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" get list of logged users"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<<" get list of logged users</h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" requesting all logged users"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" requesting all logged users</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'o' command:</h3>";
	client1->openSessionWithPeer(cnt2);
	sleep(2);
	if(client2->getCommand()==OPEN_SESSION_WITH_PEER_RPT || client1->getCommand()!=OPEN_SESSION_WITH_PEER_RPT )
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<"Session between "<<cnt1<<" and "<<cnt2<< " established"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<"Session between "<<cnt1<<" and "<<cnt2<< " established </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<"try to  open session between"<<cnt1<<" and "<<cnt2<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" try to open session between"<<cnt1<<" and "<<cnt2<<"</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'cs' command:</h3>";
	client1->closeSession();
	sleep(2);
	if(client2->getCommand()==CLOSE_SESSION_WITH_PEER_RPT || client1->getCommand()!=CLOSE_SESSION_WITH_PEER_RPT )
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<"Session between "<<cnt1<<" and "<<cnt2<< " closed"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<"Session between "<<cnt1<<" and "<<cnt2<< " closed </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" Closing session between "<<cnt1<<" and "<<cnt2<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" Closing session between "<<cnt1<<" and "<<cnt2<<"</h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'cnr' command:</h3>";
	client1->requestCreateRoom(roomName);
	sleep(2);
	if(client1->getCommand()==CREATE_ROOM_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt1<< " open chat room "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt1<< " open chat room "<<roomName<<" </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" request to open chat room"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" request to open chat room </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'or' command:</h3>";
	client2->requestJoinRoom(roomName);
	sleep(2);
	if(client2->getCommand()==JOIN_ROOM_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<< " join chat room "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<< " join chat room "<<roomName<<" </h4>"<<endl;
		i++;
	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" request to join chat room "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" request to join chat room "<<roomName<< " </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}



	reportFile<<"<h3 id=\"headline2\">Test 'l' command:</h3>";
	client1->status();
	sleep(2);
	cout.rdbuf(cout_sbuf); // restore the original stream buffer
	cout<<BOLDGREEN<<cnt1<< " connection status"<<RESET<<endl<<endl;
	cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
	reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt1<<" connection status </h4>"<<endl;
	i++;


	reportFile<<"<h3 id=\"headline2\">Test 'lr' command:</h3>";
	client2->requestAllAvailableRooms();
	sleep(2);
	if(client2->getCommand()==LIST_ALL_AVALIBLE_ROOMS_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<< " get list of all available rooms"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<< " get list of all available rooms "<<roomName<<" </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" request list of all available rooms "<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" request list of all available rooms "<< " </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'lru' command:</h3>";
	client2->requestUsersInRoom(roomName);
	sleep(2);
	if(client2->getCommand()==LIST_USERS_IN_ROOM_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" get list of all users in room: "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<< " get list of all users in room: "<<roomName<<" </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" request list of all users in room: "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" request list of all users in room: "<<roomName<< " </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'cr' (when not owner of the room) command:</h3>";
	client2->requestCloseRoom(roomName);
	sleep(2);
	if(client2->getCommand()==CLOSE_ROOM_ERROR_NOT_OWNER_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" cannot close room  "<<roomName<<", not owner"<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<< " cannot close room  "<<roomName<<", not owner </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt2<<" request request tp close room room "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt2<<" request tp close room room "<<roomName<< ", not owner </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}

	reportFile<<"<h3 id=\"headline2\">Test 'cr' (when owner of the room) command:</h3>";
	client1->requestCloseRoom(roomName);
	sleep(2);
	if(client1->getCommand()==ROOM_CLOSED_RPT)
	{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDGREEN<<cnt2<<" close room  "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"green\""<<">"<<i<<") "<<cnt2<< " close room  "<<roomName<<" </h4>"<<endl;
		i++;

	}
	else{
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		cout<<BOLDRED<<"Test failed: while "<<cnt1<<" request to close room: "<<roomName<<RESET<<endl<<endl;
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		reportFile<<"<h4 id=\"red\""<<">Test failed: while "<<cnt1<<" request to close room: "<<roomName<< ", not owner </h4>"<<endl;
		reportFile<<"<h2 id=\"headline3\"><u>TEST FINISHED - FAILED</u><h2> "<<endl<<endl;
		reportFile.close();
		close();
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return false;
	}


	reportFile<<"<h2 id=\"headline4\"><u>TEST FINISHED - PASSED</u><h2> "<<endl<<endl;





	 reportFile << "</body></html>";
	 reportFile.close();


	 close();
	 cout.rdbuf(cout_sbuf); // restore the original stream buffer
	 return true;

}

void MessengerAppTest::openReport()
{
	system("xdg-open  /home/user/Desktop/Asi/Asi-Project/Test/Debug/report.html &");
}

void MessengerAppTest::close()
{
	client1->close();
	client2->close();
	delete client1;
	delete client2;

}














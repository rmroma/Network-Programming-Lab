/*
 * main.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: user
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include "MessengerAppTest.h"
#include "UDPTest.h"


using namespace std;


int main()
{
	bool flag=false;
	streambuf* cout_sbuf = cout.rdbuf();
	ofstream  fout("/dev/null");
	string choice;
	cout<<endl<<endl;
	cout<<BOLDCYAN<<"\t//********************************************************//"<< endl;
	cout<<BOLDCYAN<<"\t//"<<BOLDGREEN<<"         WELCOME TO MESSENGER APPLICATION TEST"<<BOLDCYAN<<"          //"<< endl;
	cout<<BOLDCYAN<<"\t//       -----------------------------------------        //"<< endl;
	cout<<BOLDCYAN<<"\t// Please choose one of the following:                    //"<< endl;
	cout<<BOLDCYAN<<"\t// 1)To Test messenger application app                    //"<< endl;
	cout<<BOLDCYAN<<"\t// 2)To Test UDPMessnger                                  //"<< endl;
	cout<<BOLDCYAN<<"\t// 3)To exit messenger application app test               //"<< endl;
	cout<<"\t//********************************************************//"<<RESET<< endl;

	MessengerServerApp * server=new MessengerServerApp();
	sleep(2);
		cout<<BOLDCYAN<<"Your choice: "<<RESET;
		cin>>choice;
		if(choice=="1")
		{
			MessengerAppTest * appTest=new MessengerAppTest();
			cout<<BOLDCYAN<<"Test messenger application app started at: " <<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDCYAN<<RESET<<endl;
			cout<<BOLDCYAN<<"please wait for the test to finish"<<RESET<<endl;
			cout<<BOLDCYAN<<"A report file will generate at the end of the test"<<RESET<<endl<<endl;
			flag=appTest->test();
			cout<<BOLDCYAN<<"Test finish at: "<<BOLDWHITE<<MessengerProtocol::getTime()<<BOLDCYAN<<RESET<<endl;
			if(flag)
				cout<<endl<<BOLDGREEN<<"TEST MESSENGER APPLICATION PASSED!!!"<<RESET<<endl<<endl;
			else
				cout<<endl<<BOLDRED<<"TEST MESSENGER APPLICATION FAILED!!!"<<RESET<<endl<<endl;

			appTest->openReport();
		}
		if(choice=="2")
		{
			UDPTest * udpTest=new UDPTest();
			cout<<endl;
			cout<<BOLDCYAN<< "UDP Socket test started at "<<BOLDWHITE<<MessengerProtocol::getTime()<<RESET<<endl<<endl;
			flag=udpTest->test();
			cout<<BOLDCYAN<< "UDP Socket test finished at "<<BOLDWHITE<<MessengerProtocol::getTime()<<RESET<<endl<<endl;
			if(flag)
				cout<<endl<<BOLDGREEN<<"TEST UDP MESSENGER PASSED!!!"<<RESET<<endl<<endl;

			else
				cout<<endl<<BOLDGREEN<<"TEST UDP MESSENGER FAILED!!!!!!"<<RESET<<endl<<endl;

		}
		if (choice=="3")
		{
			cout<<BOLDCYAN<<"\t//********************************************************//"<< endl;
			cout<<BOLDCYAN<<"\t//"<<BOLDGREEN<<"        MESSENGER APPLICATION TEST CLOSED    "<<BOLDCYAN<<"           //"<< endl;
			cout<<"\t//********************************************************//"<<RESET<< endl;
			return 1;
		}
		cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
		server->close();
		delete server;
		cout.rdbuf(cout_sbuf); // restore the original stream buffer
		return 1;


}




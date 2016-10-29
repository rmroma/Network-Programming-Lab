/*
 * Model.cpp
 *
 *  Created on: May 27, 2016
 *      Author: asi
 */

#include "Model.h"

Model * Model::s_instance=NULL;


bool Model::isUserAlreadyLogged(const string & userName)
{
	mapData::iterator iter=loggedUsers.begin();
	mapData::iterator eiter=loggedUsers.end();
	if (loggedUsers.size()!=0)
	{
		for(;iter!=eiter;iter++)
		{
			if (iter->first==userName)
				return true;
		}
	}
	return false;
}

void Model::removeUserFromLogged(const string & userName){
	mapData::iterator iter=loggedUsers.find(userName);
	if(iter!=loggedUsers.end())
		loggedUsers.erase(iter);
}


bool Model::login(string userName,string password){
	if(isUserAlreadyLogged(userName)==true)
		return false;
	if(verfiyUserAndPassword(userName,password)==false)
			return false;
	addUserToLogged(userName,password);
	return true;


}


bool Model::verfiyUserAndPassword(const string & userName,const string & password){
	string line;
	ifstream myFile(DATABASE);
	if(myFile.is_open())
	{
		while(myFile.good())
		{
			getline(myFile,line);

			if(line.find("User:"+userName)==0 && "User:"+userName == line)
			{
				//getting password
				getline(myFile,line);
				//checking if equals
				if(line.find("Pass:"+password)==0 && "Pass:"+password == line)
					return true;
			}
			else
				continue;
		}
		myFile.close();
	}
	//if after reading the whole file we didnt found match, return false
	return false;

}

void Model::addNewRoom(const string & roomName){
	roomsNames.insert(pair<string,string>(roomName,""));

}

mapData Model::getRoomNames(){
	return roomsNames;

}

void Model::removeRoom(const string & roomName){
	roomsNames.erase(roomName);
}





void Model::insertNewP2PSession(const string userName1,const string userName2){

	P2Psession.insert(pair<string,string>(userName1,userName2));



}
mapData Model::getP2PSessions(){
	return P2Psession;
}
void Model::removeP2PSession(const string userName1,const string userName2){
	P2Psession.erase(userName1);
	P2Psession.erase(userName2);

}


//Checks if room name already exits
bool Model::isRoomExists(const string & roomName){
	map<string,string>::iterator iter=roomsNames.begin();
	map<string,string>::iterator eiter=roomsNames.end();
	for(;iter!=eiter;iter++){
		if(iter->first==roomName){
			return true;
		}
	}
	return false;
}

//Checks if user name already exits
bool Model::isNameExists(const string & userName) {

	string line;
	ifstream myFile(DATABASE);
	if(myFile.is_open())
	{
		while(myFile.good())
		{
			getline(myFile,line);

			if(line.find("User:"+userName)==0)
				return true;
			else
				continue;
		}
		myFile.close();
	}

	//If the user not found
	return false;

}

void Model::addUserToLogged(string userName,string password){
	loggedUsers.insert(pair<string,string>(userName,password));

}

void Model::addUserToDatabase(string userName,string password){
	ofstream writeFile;
	writeFile.open(DATABASE,ios::app);
	if(writeFile.is_open())
	{
		writeFile<<"User:"+userName<<endl;
		writeFile<<"Pass:"+password<<endl;
		writeFile.close();

	}

}

mapData Model::getAllUserList(){
	mapData userList;
	string line;
	string name;
	int i=1;
	ifstream readFile(DATABASE);
	if(readFile.is_open())
	{
		while(readFile.good())
		{
			getline(readFile,line);
			if(line.find("User:")==0)
			{
				userList.insert(pair<string,string>(line.substr(5,line.length()),""));
				i++;
			}
			else
				continue;
		}
		readFile.close();
	}

	return userList;

}
//data.insert(pair<string,string>(userName,""));
mapData Model::getConnectUserList(){

	return loggedUsers;

}




bool Model::signUp(string userName,string password){

	if(isNameExists(userName)==true){
		return false;
	}
	else{
		addUserToDatabase(userName,password);
		addUserToLogged(userName,password);
		return true;

	}
}

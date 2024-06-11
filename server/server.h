#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define closes closesocket
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#endif

using namespace std;

const int PORT = 12345;
const int BUFFER_SIZE = 1024 * 8;
const int MAX_CLIENTS = 10;

extern mutex clientMutex;
extern vector<int> connectedClients;

// use this function to send a message to a client
// returns true if message sent successfully
// returns false otherwise
bool _sendToClient(int clientNo, string message);

// this function will be called when server receives a message from a client
void recvFromClient(int clientNo, string message) {
	cout << clientNo << " " << message << endl;
	size_t ind = message.find(":");
	string clNum = message.substr(0, ind);
	string msg = message.substr(ind+1);

	if (clNum != "all" and clNum != "file" and clNum != "getfile") {
		int clNo = stoi(clNum);
		string nummsg = "Message from " + to_string(clientNo) + ": " + msg;
		if (find(connectedClients.begin(), connectedClients.end(), clNo) == connectedClients.end()) {
			cout << "Failed to send message to client " << clNo << endl;
			_sendToClient(clientNo, "Error: Client " + to_string(clNo) + " is not online.");
		}
		else {
			_sendToClient(clNo, nummsg);
		}
	}
	else if (clNum == "file") {
		ifstream ifile;
		string mmsg = "C:\\Users\\vrers\\OneDrive\\Рабочий стол\\Прога\\Курсовая\\client\\" + msg;
		//"C:\Users\vrers\OneDrive\Рабочий стол\Прога\Курсовая\client\clientfile.txt"
		ifile.open(mmsg);
		if (!ifile.is_open())
		{
			string error = "ERROR: The client doesn't have such a file!";
			_sendToClient(clientNo, error);
		}
		else {
			string strfile;
			string contfile;
			while (!ifile.eof()) {
				getline(ifile, strfile);
				contfile += (strfile + "\n");
			}
			size_t point = msg.find(".");
			string copyfilename = msg.substr(0, point) + "_copy.txt";
			ofstream ofile;
			ofile.open(copyfilename);
			if (!ofile.is_open())
			{
				string error = "ERROR: A copy of the file was not created!";
				cout << "ERROR: A copy of the file was not created!" << endl;
				_sendToClient(clientNo, error);
			}
			else {
				ofile << contfile;
				cout << "A copy of the "<<msg<<" file has been created with name: "<< copyfilename << endl;
				_sendToClient(clientNo, "The file was successfully sent!");
			}
			ofile.close();
		}
		ifile.close();
	}
	else if (clNum == "getfile") {
		ifstream ifile;
		ifile.open(msg);
		if (!ifile.is_open())
		{
			string error = "ERROR: There is no such file on the server!";
			_sendToClient(clientNo, error);
		}
		else {
			string strfile;
			string contfile;
			while (!ifile.eof()) {
				getline(ifile, strfile);
				contfile += (strfile+"\n");
			}
			string msgfile = msg + " content:\n" + contfile;
			_sendToClient(clientNo, msgfile);
		}
		ifile.close();
	}
	else {
		string nummsg = "Message from " + to_string(clientNo) + " to everyone: " + msg;
		for (int client : connectedClients) {
			if (client != clientNo) { // Не отправляем обратно отправителю
				if (find(connectedClients.begin(), connectedClients.end(), client) == connectedClients.end()) {
					cout << "Failed to send message to client " << client << endl;
				}
				else {
					_sendToClient(client, nummsg);
				}
			}
		}
	}
}

// this function will be called when client connects to server
void handleClientConnect(int clientId) {
	cout << clientId << " connected" << endl;
}

// this function will be called when client disconnects from server
void handleClientDisconnect(int clientId) {
	cout << clientId << " disconnected" << endl;
}

void my_server_code() {
	while (true) {

	}
}
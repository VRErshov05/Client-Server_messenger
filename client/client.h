#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <algorithm>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
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


// use this function to send a message to the serevr
// returns true if message sent successfully
// returns false otherwise
bool _send_to_server(string msg);

// this function will be called when server receives a message from the server
void recvFromServer(string msg) {
    cout << msg << "\n";
}

void my_client_code() {
    string message;
    cout << "Enter message to send to another client(s) or server" << endl;
    cout << "Format: clientNo:message or getfile:file_name or file:file_name" << endl;
   

    while (true) {
        
        getline(cin, message);

        // ѕровер€ем, что сообщение имеет правильный формат "clientNo:message"
        size_t delPos = message.find(':');
        if (delPos != string::npos) {
            _send_to_server(message);
        }
        else {
            cout << "Invalid message format. Please use the format." << endl;
        }
    }
}
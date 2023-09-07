#include"stdafx.h"

#include<iostream>

#include<winsock2.h>
#include<ws2tcpip.h>

#define PORT 8080;

using namespace std;

int main(int argc, char** argv[]) {
	cout << "SERVER SIDE SOCKET" << endl;
	cout << "------------------" << endl;
	//Step - 1 -> Creating a WSAStartup
	SOCKET serverSocket, acceptSocket;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		cout << "Winsock DLL is not found" << endl;
		return 0;
	}
	cout << "Winsock DLL is found" << endl;
	cout << "The Status : " << wsaData.szSystemStatus << endl;  

	//step - 2 -> creating the Server Socket
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at Socket()" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	cout << "Socket() is OK!" << endl;


	//Step-3 -> creating the Bind
	sockaddr_in service; 
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr); 
	int port = PORT;
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		cout << "Bind() : failed " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	cout << "Bind() is OK!" << endl;

	//Step - 4 -> Create the Listen
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		cout << "Listen() : Error listening on socket " << WSAGetLastError() << endl;
	}
	cout << "Listen() is OK" << endl; 
	cout << "Waiting for connection...................." << endl;

	//Step - 5 -> Create the Accept
	acceptSocket = accept(serverSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			cout << "Accept() : failed " << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}
		cout << "Accepted the Connection" << endl;
		system("pause");
		WSACleanup();
 }

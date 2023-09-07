#include"stdafx.h"

#include<iostream>

#include<winsock2.h>
#include<ws2tcpip.h>

#define PORT 8080;
using namespace std;

int main(int agrc, char** argv[]) {
	//Step - 1 -> Creating a WSAStartup
	cout << "CLIENT SIDE " << endl;
	SOCKET clientSocket;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		cout << "Winsock is not found " << endl;
		return 0;
	}
	cout << "winsock DLL is found " << endl;
	cout << "The Status : " << wsaData.szSystemStatus << endl;

	//step - 2 -> creating the client Socket
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at Socket() : " << WSAGetLastError() << endl;
		return 0;
	}
	cout << "Socket is OK!" << endl;


	//Step - 3 -> Create a Connect

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	int port = PORT;
	clientService.sin_port = port;
	InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == INVALID_SOCKET) {
		cout << "Client Connect() : failed to connect " << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "Client Connect() is OK!" << endl;
		cout << "Client can start sending and receving data ......." << endl;
	}
	system("pause");
	WSACleanup();
	return 0;
}

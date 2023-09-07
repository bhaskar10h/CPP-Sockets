#include "stdafx.h"

#include <iostream>
#include <string>
#include <stdlib.h>

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int sendData(SOCKET socket, const char* data, int dataSize) {
    int bytesSent = send(socket, data, dataSize, 0);
    if (bytesSent == SOCKET_ERROR) {
        cout << "send() failed: " << WSAGetLastError() << endl;
    }
    return bytesSent;
}

int receiveData(SOCKET socket, char* buffer, int bufferSize) {
    int bytesReceived = recv(socket, buffer, bufferSize, 0);
    if (bytesReceived == SOCKET_ERROR) {
        cout << "recv() failed: " << WSAGetLastError() << endl;
    }
    return bytesReceived;
}

void cleanup(SOCKET socket) {
    closesocket(socket);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    cout << "CLIENT SIDE " << endl;
    cout << "-----------" << endl;

    SOCKET clientSocket;
    WSADATA wsaData;
    int port = 55555;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        cout << "Winsock is not found " << endl;
        return 0;
    }
    cout << "winsock DLL is found " << endl;
    cout << "The Status : " << wsaData.szSystemStatus << endl;

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at Socket() : " << WSAGetLastError() << endl;
        return 0;
    }

    cout << "Socket is OK!" << endl;

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);

    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == INVALID_SOCKET) {
        cout << "Client Connect() : failed to connect " << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client Connect() is OK!" << endl;
        cout << "Client can start sending and receiving data ......." << endl;
    }

    const char* sendDataStr = "HELLO THERE, SERVER";
    int bytesSent = sendData(clientSocket, sendDataStr, strlen(sendDataStr));

    char buffer[1024];
    int bytesReceived = receiveData(clientSocket, buffer, sizeof(buffer));
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Received: " << buffer << endl;
    }

    cleanup(clientSocket);
    return 0;
}

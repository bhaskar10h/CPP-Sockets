#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

using namespace std;

enum MessageType {
    TEXT_MESSAGE,
    FILE_TRANSFER
};

int receiveData(SOCKET socket, char* buffer, int bufferSize) {
    int bytesReceived = recv(socket, buffer, bufferSize, 0);
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error == WSAECONNRESET) {
            cout << "Connection reset by peer." << endl;
        } else {
            cout << "recv() failed: " << error << endl;
        }
    }
    return bytesReceived;
}

int sendData(SOCKET socket, const char* data, int dataSize) {
    int bytesSent = send(socket, data, dataSize, 0);
    if (bytesSent == SOCKET_ERROR) {
        cout << "send() failed: " << WSAGetLastError() << endl;
    }
    return bytesSent;
}

void cleanup(SOCKET socket) {
    if (closesocket(socket) == SOCKET_ERROR) {
        cout << "closesocket() failed: " << WSAGetLastError() << endl;
    }
    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup() failed: " << WSAGetLastError() << endl;
    }
}

void handleClient(SOCKET clientSocket) {
    MessageType messageType;
    int bytesReceived = receiveData(clientSocket, (char*)&messageType, sizeof(MessageType));

    if (bytesReceived == sizeof(MessageType)) {
        switch (messageType) {
            case TEXT_MESSAGE: {
                char buffer[1024];
                bytesReceived = receiveData(clientSocket, buffer, sizeof(buffer));
                if (bytesReceived > 0) {
                    buffer[bytesReceived] = '\0';
                    cout << "Received text message: " << buffer << endl;
                }
                break;
            }
            case FILE_TRANSFER: {
                // Handle file transfer
                // ...
                break;
            }
        }
    } else {
        cout << "Invalid message type received." << endl;
    }

    cleanup(clientSocket);
}

int main(int argc, char* argv[]) {
    cout << "SERVER SIDE" << endl;
    cout << "-----------" << endl;

    SOCKET serverSocket, acceptSocket;
    WSADATA wsaData;
    int port = 55555;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        cout << "Winsock DLL is not found" << endl;
        return 0;
    }
    cout << "Winsock DLL is found" << endl;
    cout << "The Status : " << wsaData.szSystemStatus << endl;

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at Socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    cout << "Socket() is OK!" << endl;

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "Bind() : failed " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    cout << "Bind() is OK!" << endl;

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Listen() : Error listening on socket " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    cout << "Listen() is OK" << endl;
    cout << "Waiting for connection...................." << endl;

    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "Accept() : failed " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    cout << "Accepted Connection" << endl;

    std::thread clientThread(handleClient, acceptSocket);
    clientThread.detach();

    system("pause");
    return 0;
}

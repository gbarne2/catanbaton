#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_SERV_BUFLEN 4096
#define DEFAULT_SERV_PORT "4242"

class tcpserver
{
private:
	int iResult;
	sockaddr client_address;
	int client_addr_length;
	int iSendResult;
	SOCKET ClientSocket = INVALID_SOCKET;
	char recvbuf[DEFAULT_SERV_BUFLEN];
	int recvbuflen;

public:
	int stopInitFlag;
	SOCKET client_sockets[30];
	fd_set readfds;
	tcpserver(char*);
	~tcpserver();
	int sendPacket(SOCKET ClientSocket, char *data);
	SOCKET initializeServer(SOCKET ClientSocket);
	int receiveUntilDoneWithEcho(SOCKET ClientSocket);
	int receiveUntilDone(SOCKET ClientSocket);
	int shutDownClientSocket(SOCKET ClientSocket);
	void cleanup(SOCKET ClientSocket);

};


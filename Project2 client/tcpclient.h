#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

class tcpclient
{
	WSADATA wsaData;
	SOCKET ConnectSocket;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen;
	char address[DEFAULT_BUFLEN];
public:
	tcpclient(char*);
	~tcpclient();
	int initWinsock(char*);		//sets up socket, resolves server address and port, and attemps to connect to address until 1 succeeds
	int connectServer();
	int shutdownConnection();
	int sendBuf();
	int sendThenReceive();
	int recieveSingle();
	int recieveUntilDone();
	void cleanup();
};


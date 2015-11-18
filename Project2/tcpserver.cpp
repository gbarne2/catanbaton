#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tcpserver.h"
#include "serverMain.h"


//#undef UNICODE
//int __cdecl main(void)

#pragma comment (lib, "Ws2_32.lib")


SOCKET tcpserver::initializeServer(SOCKET ClientSock)
{
	WSADATA wsaData;
	timeval timeout;
	fd_set readSet;
	static SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	timeout.tv_sec = 0;
	timeout.tv_usec = 100;
	// Initialize Winsock
	if (ListenSocket == INVALID_SOCKET)
	{
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return INVALID_SOCKET;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_SERV_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return INVALID_SOCKET;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return INVALID_SOCKET;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return INVALID_SOCKET;
		}

		freeaddrinfo(result);
		//	std::cout << "About to listen" << std::endl;
		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return INVALID_SOCKET;
		}
	}
//	std::cout << "done with listen" << std::endl;
	// Accept a client socket
	FD_ZERO(&readSet);
	FD_SET(ListenSocket, &readSet);
	timeout.tv_sec = 0;  // Zero timeout (poll)
	timeout.tv_usec = 100;
	if (select(ListenSocket, &readSet, NULL, NULL, &timeout) == 1)
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			cleanup(ListenSocket);
			return INVALID_SOCKET;
		}
		std::cout << "Done with accept" << std::endl;
	}
	else
		ClientSocket = INVALID_SOCKET;
	ClientSock = ClientSocket;
	// No longer need server socket
//	closesocket(ListenSocket);
	return(ClientSocket);
}

int tcpserver::receiveUntilDoneWithEcho(SOCKET ClientSock)
{
	// Receive until the peer shuts down the connection
	do {

//		iResult = recv(ClientSock, recvbuf, recvbuflen, 0);
		iResult = recv(ClientSock, databuff, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
//			iSendResult = send(ClientSock, recvbuf, iResult, 0);
			iSendResult = send(ClientSock, databuff, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				cleanup(ClientSock);
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			cleanup(ClientSock);
			return 1;
		}

	} while (iResult > 0);
	return(iResult);
}

int tcpserver::sendPacket(SOCKET ClientSock, char *data, int length)
{
	iSendResult = send(ClientSock, data, length, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSock);
		return -1;
	}
	printf("Bytes sent: %d\n", iSendResult);
	return(0);
}


int tcpserver::receiveUntilDone(SOCKET ClientSock)
{
// Receive until the peer shuts down the connection
//	do {
//		iResult = recv(ClientSock, recvbuf, recvbuflen, 0);
	timeval timeout;
	fd_set readSet;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100;
	ZeroMemory(databuff, DEFAULT_SERV_BUFLEN);
	FD_ZERO(&readSet);
	FD_SET(ClientSock, &readSet);
	timeout.tv_sec = 0;  // Zero timeout (poll)
	timeout.tv_usec = 1000;
//	if (select(ClientSock, &readSet, NULL, NULL, &timeout) == 1)
	{
		iResult = recv(ClientSock, databuff, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
//			printf("recv failed with error: %d\n", WSAGetLastError());
//			cleanup(ClientSock);
		}
	}

//	} while (iResult == 0);
	return(iResult);
}

int tcpserver::shutDownClientSocket(SOCKET ClientSock)
{
	// shutdown the connection since we're done
	iResult = shutdown(ClientSock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSock);
		return -1;
	}
	return 0;
}

void tcpserver::cleanup(SOCKET ClientSock)
{
	// cleanup
	closesocket(ClientSock);
	WSACleanup();
}

tcpserver::tcpserver(char *addr)
{
//	for (int x = 0; x < sizeof(addr); x++)
//		address[x] = addr[x];
//	ListenSocket = INVALID_SOCKET;
//	ClientSock = INVALID_SOCKET;
//	result = NULL;
	recvbuflen = DEFAULT_SERV_BUFLEN;
	client_addr_length = sizeof(sockaddr);
}

tcpserver::~tcpserver()
{
//	if(ClientSock != INVALID_SOCKET)
//	shutDownClientSocket();
//	WSACleanup();
//	ClientSock = INVALID_SOCKET;
}
#include <iostream>
#include "tcpserver.h"


//#undef UNICODE
//int __cdecl main(void)

//#pragma comment (lib, "Ws2_32.lib")

SOCKET tcpserver::initializeServer(SOCKET ClientSocket)
{
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
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

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		cleanup(ListenSocket);
		return INVALID_SOCKET;
	}

	// No longer need server socket
	closesocket(ListenSocket);
	return(ClientSocket);
}

int tcpserver::receiveUntilDoneWithEcho(SOCKET ClientSocket)
{
	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				cleanup(ClientSocket);
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			cleanup(ClientSocket);
			return 1;
		}

	} while (iResult > 0);
	return(0);
}

int tcpserver::sendPacket(SOCKET ClientSocket, char *data)
{
	iSendResult = send(ClientSocket, data, iResult, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSocket);
		return 1;
	}
	printf("Bytes sent: %d\n", iSendResult);
}


int tcpserver::receiveUntilDone(SOCKET ClientSocket)
{
// Receive until the peer shuts down the connection
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			cleanup(ClientSocket);
			return 1;
		}

	} while (iResult > 0);
	return(0);
}

int tcpserver::shutDownClientSocket(SOCKET ClientSocket)
{
	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSocket);
		return 1;
	}
	return 0;
}

void tcpserver::cleanup(SOCKET ClientSocket)
{
	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

tcpserver::tcpserver(char *addr)
{
//	for (int x = 0; x < sizeof(addr); x++)
//		address[x] = addr[x];
//	ListenSocket = INVALID_SOCKET;
//	ClientSocket = INVALID_SOCKET;
//	result = NULL;
	recvbuflen = DEFAULT_SERV_BUFLEN;
	client_addr_length = sizeof(sockaddr);
}

tcpserver::~tcpserver()
{
//	if(ClientSocket != INVALID_SOCKET)
//	shutDownClientSocket();
	WSACleanup();
//	ClientSocket = INVALID_SOCKET;
}
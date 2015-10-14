#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tcpserver.h"
#include "serverMain.h"


//#undef UNICODE
//int __cdecl main(void)

#pragma comment (lib, "Ws2_32.lib")

#define max_clients	15
#define _WINSOCK_DEPRECATED_NO_WARNINGS	1

SOCKET tcpserver::initializeServer(SOCKET ClientSocket)
{
	char str[INET_ADDRSTRLEN];
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET new_sock = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	timeval *timeout = new timeval;
	int addrlen = 0;
	struct sockaddr_in address;
	timeout->tv_sec = 0;
	timeout->tv_usec = 10000;	//10ms delay
	int activity = 0;
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
	std::cout << "About to listen" << std::endl;
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	std::cout << "done with listen" << std::endl;
	// Accept a client socket
/*	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		cleanup(ListenSocket);
		return INVALID_SOCKET;
	}
	std::cout << "Done with accept" << std::endl;
*/
	while (!stopInitFlag)
	{
		FD_ZERO(&readfds);
		FD_SET(ListenSocket, &readfds);		//save master socket to readfds
		for (int i = 0; i < max_clients; i++)
		{
			ClientSocket = client_sockets[i];
			if (ClientSocket > 0)
				FD_SET(ClientSocket, &readfds);
		}
		activity = select(0, &readfds, NULL, NULL, timeout);
		if (activity == SOCKET_ERROR)
		{
			std::cout << "Socket error when using select!" << std::endl;
			break;
		}
		if (FD_ISSET(ListenSocket, &readfds))
		{
			if ((new_sock = accept(ListenSocket, (struct sockaddr *)&address, (int *)&addrlen)) < 0)
			{
				perror("accept");
				break;
			}
			inet_ntop(AF_INET, &(address.sin_addr), str, INET_ADDRSTRLEN);
			std::cout << "New connection established! Socket fd is" << new_sock << std::endl << "IP Addr: " << str << std::endl << "Connection Port: " << ntohs(address.sin_port) << std::endl;
			for (int i = 0; i < max_clients; i++)
			{
				if (client_sockets[i] == 0)
				{
					client_sockets[i] = new_sock;
					break;
				}
			}
		}
		else
		{
			if (databuff[0] == 0)
			{
				//some IO on another socket!
				for (int i = 0; i < max_clients; i++)
				{
					new_sock = client_sockets[i];
					if (FD_ISSET(new_sock, &readfds))
					{
						getpeername(new_sock, (struct sockaddr*)&address, (int*)&addrlen);
						activity = recv(new_sock, databuff, 4096, 0);
						if (activity > 0)
							printf("Bytes received: %d\n", iResult);
						else if (iResult == 0)
						{
							inet_ntop(AF_INET, &(address.sin_addr), str, INET_ADDRSTRLEN);
							std::cout << "Client closed connection. IP Addr: " << str << std::endl << "Connection Port: " << ntohs(address.sin_port) << std::endl;
						}
						else {
							printf("recv failed with error: %d\n", WSAGetLastError());
							//cleanup(new_sock);
							return 1;
						}
					}
				}
			}
		}
	}
	delete[] timeout;
	// No longer need server socket
//	closesocket(ListenSocket);
	return(new_sock);
}

int tcpserver::receiveUntilDoneWithEcho(SOCKET ClientSocket)
{
	// Receive until the peer shuts down the connection
	do {

//		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		iResult = recv(ClientSocket, databuff, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
//			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			iSendResult = send(ClientSocket, databuff, iResult, 0);
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
		return -1;
	}
	printf("Bytes sent: %d\n", iSendResult);
	return(0);
}


int tcpserver::receiveUntilDone(SOCKET ClientSocket)
{
// Receive until the peer shuts down the connection
//	do {
//		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	int activity = 0;
	char str[INET_ADDRSTRLEN];
	SOCKET new_sock = INVALID_SOCKET;
	struct sockaddr_in address;
	int addrlen = 0;
/*
	iResult = recv(ClientSocket, databuff, recvbuflen, 0);
	if (iResult > 0)
		printf("Bytes received: %d\n", iResult);
	else if (iResult == 0)
		printf("Connection closing...\n");
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSocket);
		return -1;
	}
*/
	ZeroMemory(databuff, DEFAULT_SERV_BUFLEN);
	for (int i = 0; i < max_clients; i++)
	{
		new_sock = client_sockets[i];
		if (FD_ISSET(new_sock, &readfds))
		{
			getpeername(new_sock, (struct sockaddr*)&address, (int*)&addrlen);
			activity = recv(new_sock, databuff, 4096, 0);
			if (activity > 0)
				printf("Bytes received: %d\n", iResult);
			else if (iResult == 0)
			{
				inet_ntop(AF_INET, &(address.sin_addr), str, INET_ADDRSTRLEN);
				std::cout << "Client closed connection. IP Addr: " << str << std::endl << "Connection Port: " << ntohs(address.sin_port) << std::endl;
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				cleanup(new_sock);
				return 1;
			}
		}
	}
//	} while (iResult == 0);
	return(0);
}

int tcpserver::shutDownClientSocket(SOCKET ClientSocket)
{
	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		cleanup(ClientSocket);
		return -1;
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
	stopInitFlag = 0;
	recvbuflen = DEFAULT_SERV_BUFLEN;
	client_addr_length = sizeof(sockaddr);
	FD_ZERO(&this->readfds);
}

tcpserver::~tcpserver()
{
//	closesocket(ListenSocket);
//	if(ClientSocket != INVALID_SOCKET)
//	shutDownClientSocket();
//	WSACleanup();
//	ClientSocket = INVALID_SOCKET;
}
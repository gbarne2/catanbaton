#include "tcpclient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

//comment
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")


//#define DEFAULT_BUFLEN 4096
//#define DEFAULT_PORT "4242"

//int __cdecl main(int argc, char **argv)
//{
/*
WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
*/
	// Validate the parameters
//	if (argc != 2) {
//		printf("usage: %s server-name\n", argv[0]);
//		return 1;
//	}
//}
static char* tempaddrr;
SOCKET tcpclient::initWinsock(char* addr)
{
    tempaddrr = addr;
	WSADATA wsaData;
    ConnectSocket = INVALID_SOCKET;
//	struct addrinfo *result = NULL,
//		*ptr = NULL,
//		hints;
	int iResult;
	char* tempstr = "This is a test, bitch!";
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	memcpy(sendbuf, tempstr, 23);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(addr, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
        else
            break;
	}

//	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
/*	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);
*/
/*
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
*/
/*
	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
*/
    return ConnectSocket;
}

int tcpclient::sendThenReceive(char *tempbuff, int size)
{
	// Send an initial buffer
//    initWinsock(tempaddrr);
    iResult = send(ConnectSocket, tempbuff, size, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
        return -1;
	}
//    send(ConnectSocket, tempbuff, 0, 0);

    std::cout << "Bytes Sent: " << iResult << std::endl;
/*
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
*/
	// Receive until the peer closes the connection
//	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

//	} while (iResult > 0);

	// cleanup
//	closesocket(ConnectSocket);
//	WSACleanup();

    return(iResult);
}

char tcpclient::read_receive_buff(int index)
{
    char temp = 0;
    if(index < recvbuflen)
        temp = recvbuf[index%recvbuflen];
    else
        temp = -1;
    return(temp);
}

int tcpclient::get_rxbuffsize()
{
    return(recvbuflen);
}

int tcpclient::connectServer()
{
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
        return(INVALID_SOCKET);
	}
	return(0);
}

int tcpclient::sendBuf()
{
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes Sent: %ld\n", iResult);
	return(0);
}

int tcpclient::sendData(char *tempbuff, int size)
{
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    printf("Bytes Sent: %ld\n", iResult);
    return(0);
}

int tcpclient::shutdownConnection()
{
	// shutdown the connection if no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	return(0);
}

int tcpclient::recieveSingle()
{
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
		printf("Bytes received: %d\n", iResult);
	else
		printf("recv failed with error: %d\n", WSAGetLastError());
	return(iResult);
}

int tcpclient::recieveUntilDone()
{
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

void tcpclient::cleanup()
{
	closesocket(ConnectSocket);
	WSACleanup();
}


tcpclient::tcpclient(char* addr)
{
	for (int x = 0; x < sizeof(addr); x++)
		address[x] = addr[x];
	ConnectSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
}


tcpclient::~tcpclient()
{
	WSACleanup();
	std::cout << "Make sure that the structs dont need their own destructors!" << std::endl;
}
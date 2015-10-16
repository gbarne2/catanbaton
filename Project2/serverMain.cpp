#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include "tcpserver.h"
#include <iomanip>
#include "game.h"
#include <fstream>
#include <sstream>
#include "player.h"
#include "tile.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "server_Catan.h"
#include "serverMain.h"

#define setwval 1
using namespace std;
char* tempaddr = "192.168.0.103";

const int max_clients = 16;
SOCKET socketarray[max_clients];

game catan;

static tcpserver serv(" ");
static tcpserver rxserv("");
static int lockrx = 0;
int droll(int tilenumb)
{
	return(catan.get_dice_roll(tilenumb));
}

SOCKET srvinit(game session, tcpserver &tcpserv)
{
	//will need to spawn a new thread for each players connection. the connection shouldnt be closed until either the end of the game or the player leaves the game
//	cout << "Starting server server" << endl;
	SOCKET tempsocket = INVALID_SOCKET;
	tempsocket = tcpserv.initializeServer(tempsocket);
//	cout << "Server started" << endl;
//	tcpserv.receiveUntilDoneWithEcho(tempsocket);
//	tcpserv.shutDownClientSocket(tempsocket);
//	tcpserv.cleanup(tempsocket);
	return(tempsocket);
}

char databuff[4096] = { 0, };
static int connplayerf = 0;
int current_num_clients = 0;

SOCKET connectPlayers(game session, tcpserver &tcpserv)
{
	SOCKET tempsock;
	int retval = 0;
	while (game_status == 0)
	{
		tempsock = srvinit(session, tcpserv);
		if (tempsock != INVALID_SOCKET)	//!lockrx)
		{
			cout << "Player connected!" << endl;
			socketarray[current_num_clients++] = tempsock;
			lockrx = 1;
			retval = tcpserv.receiveUntilDone(tempsock);
			if (retval == 0)
				retval = framehandler(catan, databuff, 4096, tcpserv, tempsock);		//join players and what not.
			lockrx = 0;
			Sleep(500);
		}
	}
	return(retval);
}

int processData(game &session, tcpserver &tcpserv)
{
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	int retval = 0;
	while (game_status == 0)
	{
		if ((session.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 0; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = session.player_list.begin() + 1;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					tcpserv.receiveUntilDone(hostsocket);
					retval = framehandler(session, databuff, 4096, tcpserv, hostsocket);
				}
				lockrx = 0;
			}
		}
	}
	return(retval);
}
/*
int processData_Game(game &session, tcpserver &tcpserv)
{
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	int retval = 0;
	while (game_status ==1)
	{
		if ((session.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 0; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = session.player_list.begin() + 1;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					tcpserv.receiveUntilDone(hostsocket);
					retval = framehandler(session, databuff, 4096, tcpserv, hostsocket);
				}
				lockrx = 0;
			}
		}
	}
	return(retval);
}
*/
int main()
{
	SOCKET usethisasshole = INVALID_SOCKET;
	int retval = 1;
	int retval2 = 0;
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	//	thread uno(srvinit, catan, server);
//	usethisasshole = srvinit(catan, serv);
	thread two(processData, catan, rxserv);
	thread one(connectPlayers, catan, serv);
	while(game_status == 0)
		Sleep(500);
	one.join();
	two.join();
	while (game_status == 1)
	{
		if ((catan.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 0; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = catan.player_list.begin() + 1;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					rxserv.receiveUntilDone(hostsocket);
					retval = framehandler(catan, databuff, 4096, rxserv, hostsocket);
				}
				lockrx = 0;
			}
		}
	}
/*
	while (1)
	{
		//retval = serv.receiveUntilDone(usethisasshole);
		if (retval == 0)		//if 0, packet was recievced. if 1, wtf happened? if -1, nothing was received.
		{
			retval2 = framehandler(catan, databuff, 4096, serv, usethisasshole);
		}
	}
*/
	return(0);
}


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

game catan;

static tcpserver serv(" ");

int droll(int tilenumb)
{
	return(catan.get_dice_roll(tilenumb));
}

SOCKET srvinit(game session, tcpserver &tcpserv)
{
	//will need to spawn a new thread for each players connection. the connection shouldnt be closed until either the end of the game or the player leaves the game
	cout << "Starting server server" << endl;
	SOCKET tempsocket = INVALID_SOCKET;
	tempsocket = tcpserv.initializeServer(tempsocket);
	cout << "Server started" << endl;
//	tcpserv.receiveUntilDoneWithEcho(tempsocket);
//	tcpserv.shutDownClientSocket(tempsocket);
//	tcpserv.cleanup(tempsocket);
	return(tempsocket);
}

char databuff[4096] = { 0, };

int main()
{
	SOCKET usethisasshole;
	int retval = 1;
	int retval2 = 0;
	usethisasshole = srvinit(catan, serv);
	while (1)
	{
		retval = serv.receiveUntilDone(usethisasshole);
		if (retval == 0)		//if 0, packet was recievced. if 1, wtf happened? if -1, nothing was received.
		{
			retval2 = framehandler(catan, databuff, 4096, serv);
		}
	}
	return(0);
}


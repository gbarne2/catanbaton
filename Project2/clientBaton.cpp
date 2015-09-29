//this will be the main game file for the client side, other than for the GUI. 
//All functionality on the client side will go through this function! the GUI will point to these functions do execute tasks


//need to be able to do the following
/*
-Call function to setup client and host server stuff to run on local host
-Init game
-request current # cards from server
-request board info from server
-build roads
-build settlement
-build cities (when allowed)
-perform initial placement
-trade with player (need to get other stuff working well first, and need to have 2 clients for this?)
-buy DV card (not implemented yet)
-place robber and steal card from player
-start turn
-end turn

*/

//Plan of action
/*
-Get client side control done first, verify code with the strings that would be sent to the server.
-Get client - server interface working
-get gui running
*/
#include <iostream>
#include <iomanip>
#include "game.h"
#include <fstream>
#include <sstream>
#include "player.h"
#include "tile.h"
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "tcpclient.h"
#include "tcpserver.h"
#include "clientTransmitHandler.h"
#include "clientFramehandler.h"
#include "clientBaton.h"

using namespace std;

//need to define the structure containing info about the player! (player name, number, etc.)
int get_player_num()
{
  cout << "make 'get_player_num()' actually work, in clientBaton.cpp" << endl;
  return(1);
}

int refresh_cards()
{
  sendPacketTX(get_player_num(), 0, READ_RESOURCES);
  return(0);
}

int start_turn()
{
	int retval = 0;
	retval = tx_start_turn();
	FLAG_TURN_START = 1;	//set flag to one so that when we get info back we can display it to user?
	return(retval);
}

int end_turn()
{
	FLAG_MY_TURN = 0;
	return(tx_end_turn());
}

//flag_rx_packet_needs_processing = 1;

int client_build_road(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_ROAD = 1;
	retval = txhandler_build_road(get_player_num(), tile, corner);
	return(retval);
}

int client_build_settlement(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_SETTLEMENT = 1;
	retval = txhandler_build_settlement(get_player_num(), tile, corner);
	return(retval);
}

int client_build_city(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_CITY = 1;
	retval = txhandler_build_city(get_player_num(), tile, corner);
	return(retval);
}

//this function should be the one that is called whenever flag_rx_packet_needs_processing goes high
int rx_packet_checker(int packet_type)
{
	return(1);
}

/*
int place_robber(int tile, int corner)
{

}
*/

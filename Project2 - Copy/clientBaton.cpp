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

const int max_x = ACTIVE_NUM_TILES_CLIENT;
playerClient playerdata;
tileclient board[ACTIVE_NUM_TILES_CLIENT];

void init_game()
{
	ClientCorner temps;
	vector<int> tempvec;

	//	corner_index = xy;
	for (int x = 0; x <= max_x; x++)
	{
		for (int xy = 0; xy < 6; xy++)
		{
			temps.corner_index = xy;
			temps.players_connected = tempvec;
			temps.property_owner = 0;
			temps.property_type = 0;
			temps.road_connected = 0;
			board[x].init_corners(temps);
		}
	}
	build_board();
}

int rolldice()
{
	static int dice_rolls[active_num_tiles] = { 2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12 };
	int temp = 0;
startagainrolldice:
	temp = (rand()*rand()) % 11 + 2;
	for (int x = 0; x < active_num_tiles; x++)
	{
		if (temp == dice_rolls[x])		//if random number = one in dice_rolls, then assign it to tile
		{
			dice_rolls[x] = 0;
			goto exit_diceroll;
		}
	}
	goto startagainrolldice;
	//	__nop();
exit_diceroll:
	return(temp);
}

//calling function must check if resource == desert. if it does,m then it needs to assign a dice roll of 0. or 1. whatever
//generates pseudo random resources based on what resources have not been allocated yet
static int rand_resources[active_num_tiles] = { WHEAT, WHEAT, WHEAT, WHEAT, ORE, ORE, ORE, WOOD,
WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP,
BRICK, BRICK, BRICK, DESERT };

int assign_resources()
{
	int temp = 0;
startagain_assign_resources:
	temp = (rand()*rand()) % 6 + 1;
	for (int x = 0; x < active_num_tiles; x++)
	{
		if (temp == rand_resources[x])		//if random number = one in dice_rolls, then assign it to tile
		{
			rand_resources[x] = 0;
			goto exit_assign_resources;
		}
	}
	goto startagain_assign_resources;
	//	__nop();
exit_assign_resources:
	return(temp);
}

int build_board()
{
	int resource = 0;
	int dice = 0;
	srand(time(0));
	//must create tile, put it into pieces array, and update tile number in other array
	for (int x = 0; x < active_num_tiles; x++)
	{
		resource = assign_resources();
		if (resource == DESERT)
			dice = 1;
		else
			dice = rolldice();
		board[x].set_resource_type(resource);
		board[x].set_dice_roll(dice);
	}
	return(1);
}
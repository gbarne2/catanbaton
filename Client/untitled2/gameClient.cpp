#include "gameClient.h"
#include <time.h>
#include <stdlib.h>
#include "clientTransmitHandler.h"
#include "clientFramehandler.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include "tcpclient.h"
#include "mainwindow.h"

char* tempaddr= "192.168.0.103";
//this will be the main game file for the client side, other than for the GUI. 
//All functionality on the client side will go through this function! the GUI will point to these functions do execute tasks
tcpclient clienttcp(tempaddr);
int num_dev_cards_bought = 0;

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
//the start game needs to updtae the player info and player_list private structures.

//tcpclient clienttcp(tempaddr);
int gameClient::update_dice_roll(int newroll)
{
    if((newroll > 1) && (newroll < 13))
        current_roll = newroll;
    return(current_roll);
}

int gameClient::startGame()
{
    return(sendPacketTX(get_player_num(), 0, START_GAME));
}

int gameClient::check_player_resource_amt(int type)
{
    return(this->playerinfo.check_resource_amount(type));
}

int gameClient::rolldice()
{
	static int dice_rolls[MAX_NUM_ACTIVE_TILES] = { 2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12 };
	int temp = 0;
startagainrolldice:
	temp = (rand()*rand()) % 11 + 2;
	for (int x = 0; x < MAX_NUM_ACTIVE_TILES; x++)
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
static int rand_resources[active_num_tiles] = { WHEAT, WHEAT, WHEAT, WHEAT, ORE, ORE, ORE, WOOD,
WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP,
BRICK, BRICK, BRICK, DESERT };
int gameClient::assign_resources()
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

SOCKET gameClient::initsocketthing()
{
    sockaddr tempad;
    SOCKET tempsock = INVALID_SOCKET;
    char* nulppt;
    tempsock = clienttcp.initWinsock(tempaddr);
    playerinfo.set_server_address(tempad, tempsock);
    setsockopt(tempsock,SOL_SOCKET, SO_KEEPALIVE,nulppt,0);
    return(tempsock);
}

gameClient::gameClient()
{
	ClientCorner temps;
	vector<int> tempvec;
//    clienttcp.initWinsock(tempaddr);

	//	corner_index = xy;
/*
 * 	for (int x = 0; x < MAX_NUM_ACTIVE_TILES; x++)
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
*/
	build_board();
}


gameClient::~gameClient()
{
}


int gameClient::build_board()
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

int gameClient::build_road(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_ROAD = 1;
	retval = txhandler_build_road(get_player_num(), tile, corner);
	return(retval);
}

int gameClient::build_settlement(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_SETTLEMENT = 1;
	retval = txhandler_build_settlement(get_player_num(), tile, corner);
	return(retval);
}

int gameClient::build_city(int tile, int corner)
{
	int retval = 0;
	FLAG_BUILD_CITY = 1;
	retval = txhandler_build_city(get_player_num(), tile, corner);
	return(retval);
}

int gameClient::check_current_player()
{
	cout << "make 'check_current_player()' actually work, in gameClient.cpp" << endl;
	return(1);
}

int gameClient::start_turn()
{
	int retval = 0;
    retval = tx_start_turn(get_player_num());
	FLAG_TURN_START = 1;	//set flag to one so that when we get info back we can display it to user?
	return(retval);
}

int gameClient::end_turn()
{
	FLAG_MY_TURN = 0;
    return(tx_end_turn(get_player_num()));
}

int gameClient::get_player_num()
{
	return(playerinfo.get_player_num());
}

int gameClient::refresh_cards()
{
	sendPacketTX(get_player_num(), 0, READ_RESOURCES);
	return(0);
}

int gameClient::Get_dice_roll()
{
    sendPacketTX(get_player_num(), 0, SEND_DICE_ROLL);
    return(0);
}

int gameClient::update_flag(int flag, int val)
{
    int retval = 0;
	switch (flag)
	{
	case F_TURN_START:
        retval = FLAG_TURN_START;
		FLAG_TURN_START = val;
		break;
	case F_TURN_END:
        retval = FLAG_MY_TURN;
		FLAG_MY_TURN = val;
		break;
	case F_BUILD_CITY:
        retval = FLAG_BUILD_CITY;
		FLAG_BUILD_CITY = val;
		break;
	case F_BUILD_ROAD:
        retval = FLAG_BUILD_ROAD;
		FLAG_BUILD_ROAD = val;
		break;
	case F_BUILD_SETTLEMENT:
        retval = FLAG_BUILD_SETTLEMENT;
		FLAG_BUILD_SETTLEMENT = val;
		break;
	default:
		if (debug_text)
			cout << "Invalid flag! flag location-> gameClient::update_flag(...)" << endl;
		break;
	}
    if(val >= 0)
        retval = flag;
    return(retval);
}

int gameClient::update_board(char* data, int datasize, int startindex, int tilenum)
{
    int retval = 0;
//    this->board[tilenum].Clientcornersz = this->board[tilenum].update_board_info_from_server(data, datasize, startindex);
    retval = this->board[tilenum].update_board_info_from_server(data, datasize, startindex);
    return(retval);
}

int gameClient::check_resource(int tilenum)
{
    return(board[tilenum].check_tile_resource_type());
}

int gameClient::get_board_info()
{
    sendPacketTX(get_player_num(), 0, GET_BOARD_INFO);
    return(0);
}

int gameClient::get_corner_owner(int tilenum, int corner)
{
    return(board[tilenum].check_corner_owner(corner));
}

int gameClient::get_corner_bldg_type(int tilenum, int corner)
{
    return(board[tilenum].check_corner_building_type(corner));
}

int gameClient::check_tile_dice_roll(int tilenum)
{
    return(board[tilenum].check_dice_roll());
}
int gameClient::set_player_number(int idnum)
{
	int retval = 0;
	if ((playerinfo.get_player_num() == -72) && (idnum > 0) && (idnum <= MAX_NUM_PLAYERS))
	{
		playerinfo.set_player_num(idnum);
		retval = 1;
	}
	else
		retval = -1;
	return(retval);
}

int gameClient::get_road_owner(int road, int tilenum)
{
	return(board[tilenum].read_road(road));
}

int gameClient::rx_packet_checker(int packet_type)
{
    return(1);
}


int gameClient::place_robber(int tile, int corner)
{
    cout << "Make gameClient::place_robber function work! " << endl;
	return(-1);
}

int gameClient::buy_dv_cardd(int numcards)
{
    num_dev_cards_bought = numcards;
    return(sendPacketTX(get_player_num(), numcards, BUY_DV_CARD));
}

int gameClient::get_qty_dv_cardd(int devcard)
{
    return(this->playerinfo.get_qty_dev_card(devcard));
}

int gameClient::joinGame()
{
    string tempstr = "";
    int temp = playerinfo.get_player_name().length();
    tempstr += temp;
    tempstr += playerinfo.get_player_name();
    sendPacketTX(get_player_num(), 0, JOIN_GAME);
}

/*
What does the client need to provide
- A way to communicate with the server
- A way to locate and open up a network session with the server
- GUI


What does the client need to define / do
- Framehanding function to receive packets for the things the server can send ?
-define actions the user can take(defined as functions and stuff)
->Start turn
->End turn
->Roll dice
->Place robber
->Propose trade
->Accept / reject trade
->Buy DV card
->build roads
->build settlements
->upgrade settlements
->Start a game / wait for players
->find open games
->chat with other players
->Look at what else the server can send
->Read / display / update # of cards the player has
->request an updated board layout ?
-
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "clientFramehandler.h"
#include "Tile_client.h"
#include "playerClient.h"
#include "clientBaton.h"

using namespace std;

//static trade_cards_offer trade_to_process;
//static playerClient playerdata;

int clientFrameHandler(char* datain)
{
	int dataptr = 7;		//use this to grab data from datain buffer.
	int tempdata = 0;
	char datatype = datain[4];
	int player_number = datain[5];
	string tempstring;
	int datasize = datain[6];
	char *nulptr;
	int retval = 0;
	if ((datain[0] == 'S') && (datain[1] == 8) && (datain[2] == 53) && (datain[3] == 'p'))
	{
		cout << "Valid packet received.... Processing" << endl;
		last_packet_sent(datatype);
		switch (datatype)
		{
		case PROPOSE_TRADE://this case should prompt the user about the trade being proposed and allow them to response whether or not to accept the trade. when sent by a	user, they initiate the trade, when received they are being asked to trade
			trade_to_process.qty_wood_to_trade = datain[dataptr];
			trade_to_process.qty_wood_to_receive = datain[dataptr + 1];
			trade_to_process.qty_ore_to_trade = datain[dataptr + 2];
			trade_to_process.qty_ore_to_receive = datain[dataptr + 3];
			trade_to_process.qty_brick_to_trade = datain[dataptr + 4];
			trade_to_process.qty_brick_to_receive = datain[dataptr + 5];
			trade_to_process.qty_wheat_to_trade = datain[dataptr + 6];
			trade_to_process.qty_wheat_to_receive = datain[dataptr + 7];
			trade_to_process.qty_sheep_to_trade = datain[dataptr + 8];
			trade_to_process.qty_sheep_to_receive = datain[dataptr + 9];
			break;
		case ACCEPT_REJECT_TRADE:
			//if this packet is received, it contains the status of the trade
			if (requested_player == player_number)	//if the player we traded with sent us this trade, then process it!
			{
				retval = datain[dataptr];		//update retval with status of trade
				trade_status(retval);
				flag_rx_packet_needs_processing = 1;
			}
			else
				retval = INVALID_TRADE;
			requested_player = 0;
			trade_to_process.qty_wood_to_trade = 0;
			trade_to_process.qty_wood_to_receive = 0;
			trade_to_process.qty_ore_to_trade = 0;
			trade_to_process.qty_ore_to_receive = 0;
			trade_to_process.qty_brick_to_trade = 0;
			trade_to_process.qty_brick_to_receive = 0;
			trade_to_process.qty_wheat_to_trade = 0;
			trade_to_process.qty_wheat_to_receive = 0;
			trade_to_process.qty_sheep_to_trade = 0;
			trade_to_process.qty_sheep_to_receive = 0;
			break;
		case GET_PLAYER_INFO:
			//should send data about this player to the server, like their name and stuff?
			break;
		case SEND_DICE_ROLL:
			retval = dice_roll(datain[dataptr++]);
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_QTY_ROADS_LEFT:
			retval = datain[dataptr];	//this should be the # of roads left to build
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_QTY_SETTLEMENTS_LEFT:
			retval = datain[dataptr];
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_QTY_CITIES_LEFT:
			retval = datain[dataptr];
			flag_rx_packet_needs_processing = 1;
			break;
		case BUILD_ROAD:
			if (datain[dataptr] != FAILED_TO_BUILD_ROAD)	//if the road was built, then update the GUI.
			{
				nulptr = new char[datasize];
				for (int x = 0; x < datasize; x++)
					nulptr[x] = datain[x + dataptr];
				update_board_info(nulptr, datasize);
				retval = 1;
				flag_rx_packet_needs_processing = 1;
			}
			else
				retval = FAILED_TO_BUILD_ROAD;
			break;
		case BUILD_SETTLEMENT:
			if (datain[dataptr] != FAILED_TO_BUILD_SETTLEMENT)
			{
				nulptr = new char[datasize];
				for (int x = 0; x < datasize; x++)
					nulptr[x] = datain[x + dataptr];
				update_board_info(nulptr, datasize);
				retval = 1;
				flag_rx_packet_needs_processing = 1;
			}
			else
				retval = FAILED_TO_BUILD_SETTLEMENT;
			break;
		case UPGRADE_SETTLEMENT:
			if (datain[dataptr] != FAILED_TO_UPGRADE_SETTLEMENT)
			{
				nulptr = new char[datasize];
				for (int x = 0; x < datasize; x++)
					nulptr[x] = datain[x + dataptr];
				update_board_info(nulptr, datasize);
				retval = 1;
				flag_rx_packet_needs_processing = 1;
			}
			else
				retval = FAILED_TO_UPGRADE_SETTLEMENT;
//			flag_rx_packet_needs_processing = 1;
			break;
		case BUY_DV_CARD:
			break;
		case READ_RESOURCES:
			playerdata.update_resources(1, datain[dataptr++]);
			playerdata.update_resources(2, datain[dataptr++]);
			playerdata.update_resources(3, datain[dataptr++]);
			playerdata.update_resources(4, datain[dataptr++]);
			playerdata.update_resources(5, datain[dataptr++]);
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_BOARD_INFO:
			nulptr = new char[datasize];
			for (int x = 0; x < datasize; x++)
				nulptr[x] = datain[x + dataptr];
			update_board_info(nulptr, datasize);
			retval = 1;
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_TIME_LIMIT:
			retval = time_limit(datain[dataptr]);
			flag_rx_packet_needs_processing = 1;
			break;
		case START_GAME:
			//Not sure what this needs to do on the client side when received.
			break;
		case JOIN_GAME:
			//maybe this wont ever be called?
			break;
		case STEAL_CARD_ROBBER:
			//data[0] = card that was stolen!
			//	-> if data[0] > 0, it was obtained, < 0 it was stolen
			retval = playerdata.check_resource_amount(abs(data[dataptr]));
			playerdata.update_resources(abs(retval), (1 * (retval / abs(retval))));
			if (retval < 0)	//if card stolen, tell use!
				cout << "replace this text, but yo bitch ass just got robbed" << endl;
			else
				cout << "You just stole a card, u thug masta G, card stolen: "  << retval << endl;
			flag_rx_packet_needs_processing = 1;
			break;
		case START_TURN:
		//datastructure:
		//datain[0] = current_player -> whose turn it is
		//datain[1] = is current_player you? if 1, yes and go. if 0, not your turn.
			current_players_turn = datain[dataptr++];
			if (datain[dataptr] == '1')
			{
				MY_TURN = 1;
				cout << "It is my turn! End turn should actually handle starting turn. client needs to send this packet to 'roll' the dice" << endl;
			}
			//this should maybe just update whose turn it is.
			flag_rx_packet_needs_processing = 1;
			break;
			
		default:
			retval = INVALID_PACKET_TYPE;
		}
	}
	else
		retval = INVALID_PACKET_HEADER;
	return(retval);
}

int time_limit(int data)
{
	static int limit = -1;
	if (data > 0)
		limit = data;
	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
		limit = -1;
	return(limit);
}

vector<int> get_num_active_tiles(int data)
{
	static int active_num_tile = 0;
	static int X_tiles = 0;
	static int Y_tiles = 0;
	vector<int> retval;
	if ((data > 0) && (data < MAX_NUM_TILES))
	{
		active_num_tile = data;
		X_tiles = ceil(sqrt(active_num_tile));
		Y_tiles = ceil(sqrt(active_num_tile));
	}
	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
		active_num_tile = 0;
	retval.push_back(active_num_tile);
	retval.push_back(X_tiles);
	retval.push_back(Y_tiles);
	return(retval);
}

int update_board_info(char* data, int datasize)
{
	int startindex = 0;
	int size_corner;
	int retval = 0;
	int x_ind = 0;
	int y_ind = 0;
	int tilenum = 0;
	vector<int> numtiles = get_num_active_tiles(0);
	int numtiles = 0;
	vector<int>::iterator p = numtiles.begin();
	numtiles = *p;		//?  pretty sure this is how to get the value!
	while (startindex < datasize)
	{
		if (data[startindex] == "S")
		{
			size_corner = data[startindex + 1];
			if (startindex + size_corner < datasize)
			{
				tilenum = data[startindex + 2];
				retval = board[tilenum].update_board_info_from_server(data, datasize, startindex);
				startindex += retval;
			}
		}
		else
		{
			if (debug_text)
				cout << "ERROR in update_board_info! invalid packet header!" << endl;
			start_index++;	//search through string for start deliminator!
		}
	}
}

int dice_roll(int data)
{
	static int current_roll = 0;
	if ((data > 1) && (data < 13))	//if valid dice roll, update the local dice roll.
		current_roll = data;
	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
		current_roll = 0;
	return(current_roll);
}

int trade_status(int data)
{
	static int trade_status_info = 0;
	if ((data == DENY_TRADE) || (data == APPROVE_TRADE))
		trade_status_info = data;
	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
		trade_status_info = 0;
	return(trade_status_info);
}
//this function will be used to check/update the last packet type sent.
//the input is the packet type that was sent. to read what the last packet type was, pass a 0 to this function

int last_packet_sent(int data)
{
	static int last_packet_type_sent = 0;		//this global will hold the last packet type that was sent. It should be used to ensure that a response was received.
	if ((data >= MIN_PACKET_VAL) && (data <= MAX_PACKET_VAL))
		last_packet_type_sent = data;		//if the datatype is valid, update the last packet type sent. if invalid, then return the value of the last packet.
	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
		last_packet_type_sent = 0;
	return(last_packet_type_sent);
}

int request_player_trade(int player_action)
{
	static int requested_player = 0;
	if (player_action > 0)
		requested_player = player_action;
	else if (player_action == RESET_STATIC_VAR_IN_FUNCTION)
		requested_player = 0;
	return(player_action);
}

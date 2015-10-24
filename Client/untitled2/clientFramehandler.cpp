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
#include "gameClient.h"
#include "clientTransmitHandler.h"
#include <cmath>
#include "mainwindow.h"

using namespace std;

#ifndef PACKET_TYPES
#define PACKET_TYPES					69
#define INVALID_PACKET_TYPE				-1001
#define INVALID_PACKET_HEADER			-1002
#define FAILED_TO_BUILD_ROAD			-31
#define FAILED_TO_BUILD_SETTLEMENT		-32
#define FAILED_TO_UPGRADE_SETTLEMENT	-33
#define MIN_PACKET_VAL					30
#define PROPOSE_TRADE					30
#define ACCEPT_REJECT_TRADE				31
#define GET_PLAYER_INFO					32
#define SEND_DICE_ROLL					33
#define GET_QTY_ROADS_LEFT				34
#define GET_QTY_SETTLEMENTS_LEFT		35
#define GET_QTY_CITIES_LEFT				36
#define BUILD_ROAD						37
#define BUILD_SETTLEMENT				38
#define UPGRADE_SETTLEMENT				39
#define BUY_DV_CARD						40
#define READ_RESOURCES					41
#define GET_BOARD_INFO					42
#define GET_TIME_LIMIT					43
#define START_GAME						44
#define ACCEPT_GAME						45
#define STEAL_CARD_ROBBER				49
#define CONNECT							50
#define START_TURN						51
#define JOIN_GAME						46
#define END_TURN						47
#define END_GAME						48
#define MAX_PACKET_VAL					51
#define USE_DV_CARD                     52
#define PLACE_ROBBER_PACKET             53
#define RESET_STATIC_VAR_IN_FUNCTION	-57
#define INVALID_PACKET_OR_SENDER        69


#define APPROVE_TRADE 					1
#define DENY_TRADE						-43
#define INVALID_TRADE					-49
#endif
//static client_trade_cards_offer trade_to_process;
//static playerClient playerdata;
static client_trade_cards_offer trade_to_process;


int clientFrameHandler(gameClient &session, char* datain)
{
	int dataptr = 8;		//use this to grab data from datain buffer.
	int tempdata = 0;
	char datatype = datain[6];
	int player_number = datain[7];
	string tempstring;
    int datasize = datain[4];
    datasize = datasize << 7;
    datasize += datain[5];
	static int requested_player = 0;
	static int current_players_turn = 0;
    char *nulptr;
    char tempchar[1] = {0};
    int retval = 0;
    check_rx_data_buff = 0;
    if(debug_text)
    {
        cout << "Player number: " << player_number << endl;
        cout << "Data type:     " << datatype << endl;
        cout << "Data size:     " << datasize << endl;
    }
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
				update_board_info(session, nulptr, datasize);
				retval = 1;
                delete[] nulptr;
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
				update_board_info(session, nulptr, datasize);
				retval = 1;
                delete[] nulptr;
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
				update_board_info(session, nulptr, datasize);
				retval = 1;
                delete[] nulptr;
				flag_rx_packet_needs_processing = 1;
			}
			else
				retval = FAILED_TO_UPGRADE_SETTLEMENT;
//			flag_rx_packet_needs_processing = 1;
			break;
        case BUY_DV_CARD:
            tempdata = datain[dataptr];
            if((datain[dataptr] == -52) || (datain[dataptr] < 0))  //if the player tried to buy too many cards, this is the error
            {
                retval = -52;
                cout << "requested too many cards!" << endl;
            }
            else
            {	//data[0] = fail/success buying dev cards
                //data[1] = number of dev cards bought
                //data[2] = num knights
                //data[3] = num vp
                //data[4] = num year of plenty
                //data[5] = num monopoly
                //data[6] = num build roads
                //process data.
                dataptr += 1;       //increment past pass/fail byte.
                if(debug_text)
                    cout << "Number of dev cards purchased: " << +datain[dataptr] << endl;
                dataptr += 1;
                session.playerinfo.update_dev_cards(1, datain[dataptr++]-1);
                session.playerinfo.update_dev_cards(2, datain[dataptr++]-1);
                session.playerinfo.update_dev_cards(3, datain[dataptr++]-1);
                session.playerinfo.update_dev_cards(4, datain[dataptr++]-1);
                session.playerinfo.update_dev_cards(5, datain[dataptr++]-1);
                if(num_dev_cards_bought > tempdata) //if less development cards were bought than requested, notify user
                    cout << "Warning! User requested " << num_dev_cards_bought << " but only " << tempdata << " cards could be purchased" << endl;
//                update_dev_cards_on_gui = 1;
                retval = 1;
            }
			break;
		case READ_RESOURCES:
			session.playerinfo.update_resources(1, datain[dataptr++]);
			session.playerinfo.update_resources(2, datain[dataptr++]);
			session.playerinfo.update_resources(3, datain[dataptr++]);
			session.playerinfo.update_resources(4, datain[dataptr++]);
			session.playerinfo.update_resources(5, datain[dataptr++]);
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_BOARD_INFO:
			nulptr = new char[datasize];
            for (int x = 0; x < datasize; x++)
            {
                nulptr[x] = datain[x + dataptr];
                if(debug_text)
                    cout << +nulptr[x] << " ";
            }
            update_board_info(session, nulptr, datasize);
			retval = 1;
            delete[] nulptr;
			flag_rx_packet_needs_processing = 1;
			break;
		case GET_TIME_LIMIT:
			retval = time_limit(datain[dataptr]);
			flag_rx_packet_needs_processing = 1;
			break;
		case START_GAME:
			//data[0] = player number
            if (session.get_player_num() == 0)
                session.set_player_number(datain[dataptr]);
            else if (debug_text)
                cout << "cannot set the player number! it has already been set!" << endl;
            //data[1] = ???
			//Not sure what this needs to do on the client side when received.
			break;
		case JOIN_GAME:
            tempchar[0] = datain[dataptr];
            session.playerinfo.set_player_num(player_number);
            cout << "My player number is: " << player_number << endl;
            break;
		case STEAL_CARD_ROBBER:
            if(dv_play_knight_flag == 1)    //if this flag is set, then process it as the notification of which player to steal a card from
            {
                dv_play_knight_flag = 0;        //next one of these packets will just be the confirmation and stuff.
                //notify user some how!
                //data[0] = number of players on tile
                //data[1] = player 1...
                //data[2] = player 2 if there...
                //.....
            }
            else
            {
                //data[0] = card that was stolen!
                //	-> if data[0] > 0, it was obtained, < 0 it was stolen
                retval = session.playerinfo.check_resource_amount(abs(datain[dataptr]));
                session.playerinfo.update_resources(abs(retval), (1 * (retval / abs(retval))));
                if (retval < 0)	//if card stolen, tell use!
                    cout << "replace this text, but yo bitch ass just got robbed. retval = "<< retval << endl;
                else
                    cout << "You just stole a card, u thug masta G, card stolen: "  << retval << endl;
                flag_rx_packet_needs_processing = 1;
            }
            break;
        case PLACE_ROBBER_PACKET:
            //this function should be called when a 7 is rolled.
            dv_play_knight_flag = 1;
            cout << "Make framehandler function notify user about robber placement" << endl;
            //note, this packet shouldnt be sent from here. it should only be sent by the server when either a 7 is rolled or for a knight
            break;
		case START_TURN:
		//datastructure:
		//datain[0] = current_player -> whose turn it is
		//datain[1] = is current_player you? if 1, yes and go. if 0, not your turn.
        //datain[2] = dice roll

            //after the first packet is received, another one should be received containing the resource info. This needs to receive a single packet and call clientframehandler recursively!
			current_players_turn = datain[dataptr++];
            tempchar[0] = datain[dataptr++];
            retval = atoi(tempchar);
            if ((datain[dataptr++] == '1') && (current_players_turn == session.get_player_num()))
			{
				session.update_flag(F_TURN_START, 1);// FLAG_MY_TURN = 1;
                if(debug_text)
                    cout << "It is my turn! End turn should actually handle starting turn (sending this packet to client). client needs to send this packet to 'roll' the dice" << endl;
			}
            tempchar[0] = datain[dataptr++];

            session.update_dice_roll(tempchar[0]);
            if((tempchar[0] == 7) || (tempchar[0] == '7'))  //if a 7 was rolled, the next packet to update resources wont be sent.  need to check if its this players turn
            {
                if(current_players_turn == session.get_player_num())
                {
                    //do whateever I need to so that the GUI asks the player what tile to place robber on.
                    request_user_place_robber = 1;
                }
            }

//            if((flag_rx_packet_needs_processing == 0) || (session.update_flag(F_TURN_START, -1)))
//            {

                nulptr = new char[4096];
                for(int x = 0; x < 4096; x++)
                   nulptr[x] = 0;
                clienttcp.recieveSingle();
                for(int x = 0; x < clienttcp.get_rxbuffsize(); x++)
                    nulptr[x] = clienttcp.read_receive_buff(x);
                clientFrameHandler(session,nulptr);
//            }
                delete[] nulptr;

                flag_rx_packet_needs_processing = 1;

			break;
        case INVALID_PACKET_OR_SENDER:
            if(debug_text)
                cout << "Invalid packet or sender!" << endl;
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
//	if ((data > 0) && (data < ACTIVE_NUM_TILES_CLIENT+1))
//	{
		active_num_tile = ACTIVE_NUM_TILES_CLIENT;
		X_tiles = ceil(sqrt(active_num_tile));
		Y_tiles = ceil(sqrt(active_num_tile));
//	}
//	else if (data == RESET_STATIC_VAR_IN_FUNCTION)
//	active_num_tile = 0;
	retval.push_back(active_num_tile);
	retval.push_back(X_tiles);
	retval.push_back(Y_tiles);
	return(retval);
}

int update_board_info(gameClient &session, char* data, int datasize)
{
	int startindex = 0;
	int size_corner;
	int retval = 0;
	int x_ind = 0;
	int y_ind = 0;
	int tilenum = 0;
	char ptrchar[1] = { 0 };
	char tempchar = 0;
	vector<int> numtiles = get_num_active_tiles(0);
//	int numtiles = 0;
	vector<int>::iterator p = numtiles.begin();
    tilenum = *p;		//?  pretty sure this is how to get the value!
	while (startindex < datasize)
	{
		if (data[startindex] == 'S')
		{
			ptrchar[0] = data[startindex + 1];
            size_corner = ptrchar[0];
            if ((startindex + size_corner) < datasize)
			{
				ptrchar[0] = data[startindex + 2];
                tilenum = ptrchar[0];
//				tilenum = data[startindex + 2];
//				retval = session.update_board(data, datasize, startindex, tilenum);
//                session.board[tilenum].Clientcornersz = session.board[tilenum].update_board_info_from_server(data, datasize, startindex);
                retval = session.board[tilenum].update_board_info_from_server(data, datasize, startindex);      //start index is passed by reference
//                startindex += retval;
			}
		}
		else
		{
			if (debug_text)
			{
				ptrchar[0] = data[startindex];
				cout << "ERROR in update_board_info! invalid packet header! Invalid data: " << atoi(ptrchar) << endl;
			}
			startindex++;	//search through string for start deliminator!
		}
	}
	return(retval);
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
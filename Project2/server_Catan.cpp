//Created by: Tyler Barney
//this file will contain the framework for communicating with clients. It will/should contain all of the different
//types of packets that the client/server will need to communicate, but will not operate on any data itself. It should simply serve
//as the interface between the two computers. AKA, this will format whatever data is sent to it and then send it off, or parse received
//data and do something with data to alert higher functions.

//Need to make seperate game code for the clients. it will basically only work with the server, and will essentially just be the gui interface and middle man. the server
//	must be able to update all the variables in this client program. client program should not be allowed to change its own variables!!!!

//may need to define a class? just so there are private global variables. or need some other way of keeping track of last message.
//maybe, if a command being sent by server requires a response, the server should send the command and wait until it gets a response before continuing on?
//		doing this in a thread would allow other functionalities to go on while waiting for the response. but thread should be spawned by higher
//		function so that we cant jump ahead of ourselves?

/***
todo:
-Need to start working on actual game play flow as well as the server/client communication. Most of the communications can be
	done after the game is built, but the server needs to be able to run the game. aka, it needs to call start turn, and 
	process the roll val, then do other things. the server should start turn once the client tells it to, BUT it needs to handle
	all of the other things that happen after start turn as well.
Will need to have the initialization period set up a listen socket for some port, and then keep listening for new clients until game is started!
Launch this operation in another thread, and once the start game command is received, then rejoin threads and start game.
-have every functio that sends data add its packet type to the begining of the message! 
-make all packets be formatted to whatever the standard format will be. Maybe have sendpacket(...) call a function in this file rahter than tcpserver.cpp. this function would format the data and then call the sendpacket(...) function from tcpserver.cpp.
-make send packet accept the session name as an input and use the session name and player number to get the socket info.

*/
#include "tcpserver.h"
#include <thread>
#include <iostream>
#include <map>
#include <new>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include "game.h"
#include "player.h"
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include "serverMain.h"
//#include "catanBaton.h"

using namespace std;

/*
Function needed (what ive come up with so far anyways)
Propose_trade
Accept_reject_trade
connect
get_player_info			-> name, resources?
send_dice_roll
get_dice_roll
		-> for client to request. the serve should always determine what dice roll is. this function on server should just send last dice roll
end_turn
get_qty_roads_remaining
build_road		-> may need to update road?
build_settlement		-> should check if there are enough settlements left to build!
upgrade_settlement
buy_DV_card
update_resources
start_game
quit_game
get_board_info		-> should just send info to client
get_time_limit
*/

#ifndef PACKET_TYPES
#define PACKET_TYPES				1
#define PROPOSE_TRADE				30
#define ACCEPT_REJECT_TRADE			31
#define GET_PLAYER_INFO				32
#define SEND_DICE_ROLL				33
#define GET_QTY_ROADS_LEFT			34
#define GET_QTY_SETTLEMENTS_LEFT	35
#define GET_QTY_CITIES_LEFT			36
#define BUILD_ROAD					37
#define BUILD_SETTLEMENT			38
#define UPGRADE_SETTLEMENT			39
#define BUY_DV_CARD					40
#define READ_RESOURCES				41
#define GET_BOARD_INFO				42
#define GET_TIME_LIMIT				43
#define START_GAME					44
#define ACCEPT_GAME					45
#define JOIN_GAME					46
#define END_TURN					47
#define END_GAME					48
#define STEAL_CARD_ROBBER			49
#define CONNECT						50
#define START_TURN					51
#define USE_DV_CARD					52
#define PLACE_ROBBER_PACKET         53
#define START_TURN_INIT_PLACEMENT	54
#define END_INIT_PLACEMENT_PHASE    55
#define INVALID_PACKET_OR_SENDER	69
#define MAX_DEV_CARDS_PER_TRANSACTION 10
#endif

//Message format:
/*
index 0-3 = {"S", 8, 53, "p"}		-> validity bits to verify its a real packet!
index 4   = data type (see #defines above)
index 5   = player number of request
index 6 - (size_of_data - 1) = data

*/
using namespace std;

//game catan;
map<int, player> playermap;
int game_status = 0;

//function prototypes						
int send_init_placement_start(game session, tcpserver servv);
int send_end_init_placement(game session, tcpserver servv);
int send_end_turn(game session, tcpserver servv, int prev_player);
int send_start_game(game session, tcpserver servv);
int get_qty_cities_left(game session, int player_number);
int get_qty_settlements_left(game session, int player_number);
int get_qty_roads_remaining(game session, int player_number);
int send_board_info(game session, tcpserver servv);
int steal_card(game &session, int player_taking_card, int player_giving);
static int trade_in_progress = 0;
static int initiating_player_trade = 0;
static int requested_player_trade = 0;
static int last_player = 0;
int send_packet(game session, int player_num, string data_to_send, int packet_type, tcpserver servv);
int send_packet(game session, int player_num, int data_to_send, int packet_type, tcpserver servv);
int send_packet(game session, int player_num, char *data_to_send, int packet_type, int length, tcpserver servv);
int send_dice_roll(game session, tcpserver servv);
int send_resources(game session, int playernum, tcpserver servv);
int join_game(game &session, int &player_number, string name, tcpserver servv, SOCKET sock);
unsigned int read_dice_roll(game session);
int send_dev_cards(game session, int playern, int numcards, int success, tcpserver servv);
int place_robber(game &session, int tilenum, int playernum, tcpserver servv);
int send_resources_all_players(game session, tcpserver servv);
int use_dev_card(game &session, int cardtype, int playernumm, int data, tcpserver servv);

char txdatabuff[4096];
//extern tcpserver serv(" ");

static int debug_text = 1;
static trade_cards_offer trade_to_process;
//functions

//add packets allowed before game is started here so that more packets can easily be added as needed.
int check_valid_packets_before_game(int packet)
{
	int retval = 0;
	if (packet == JOIN_GAME)
		retval = 1;
	else if (packet == START_GAME)
		retval = 1;
	else
		retval = 0;
	return(retval);
}
int framehandler(game &session, char *datain, int size_of_data, tcpserver servv, SOCKET tempsocket)
{

	//the datain field will need to be pulled in from tcpserver->receivebuffer or whateevr its called. 
	//some higer level function will need to manage this and allow it to receive data and indicate when
	//there is data to be read. when that indicator goes off, the frame handler function should be called
	int dataptr = 8;		//use this to grab data from datain buffer.
	int tempdata = 0;
	int temp = 0;
	char tempchar[1] = { 0 };
	char datatype = datain[6];
	int player_number = datain[7];
	string tempstring;
	int datasize = datain[4];
	datasize = datasize << 7;
	datasize += datain[5];
	int invalid_sender = 0;
	char *nulptr;
	int retval = 0;	
	static int turn_started_already = 0; 
	static int number_of_init_placements = 1;	// session.check_number_of_players() * 2;


	if ((datain[0] == 'S') && (datain[1] == '8') && (datain[2] == 53) && (datain[3] == 'p'))
	{
		cout << "Valid packet received.... Processing" << endl;
		if ((game_status != 0) || check_valid_packets_before_game(datatype))
		{
			switch (datatype)
			{
			case PROPOSE_TRADE:
				if (!trade_in_progress)
				{
					if ((session.check_current_player() == player_number) && (turn_started_already == 1))
					{
						trade_in_progress = 1;
						initiating_player_trade = player_number;
						if (debug_text)
							cout << "initiating_player (for trade) = " << initiating_player_trade << endl;
						requested_player_trade = datain[dataptr++];
						if (debug_text)
							cout << "requested_player = " << requested_player_trade << endl;
						tempstring = "";
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
						for (int x = dataptr; x < 10; x++)		//pull all data into a string to send off
						{
							tempstring += datain[x];
						}
						retval = send_packet(session, requested_player_trade, tempstring, PROPOSE_TRADE, servv);
					}
				}
				else
					invalid_sender = 1;
				//data field:
				//this case should create a new trade_cards object in a place that will keep it in memory while the players review trade.
				//to avoid issues with going from int to char to possibly unsigned char somewhere back to int, the notion of -2 wood = i want 2 wood isnt used.
				//instead, the data will be sent in # wood to give, # wood to receive, etc.
				//data[0] = player to trade with
				//data[1] = qty wood to trade
				//data[2] = qty wood to receive
				//data[3] = qty ore to trade
				//data[4] = qty ore to receive
				//data[5] = qty brick to trade
				//data[6] = qty brick to receive
				//data[7] = qty wheat to trade
				//data[8] = qty wheat to receive
				//data[9] = qty sheep to trade
				//data[10] = qty sheep to receive
				//this function needs to maybe format the data, and send this off to the requested player.
				//it should also update some status variable to indicate a trade is pending so when the packet comes back, the server can proceed with trade?
				break;
			case ACCEPT_REJECT_TRADE:
				//datafield
				if ((requested_player_trade == player_number) && (turn_started_already == 1) && (trade_in_progress))	//if this trade packet was sent by the correct player, then proceed
				{
					if (datain[dataptr] == initiating_player_trade)	//if the player is requesting to trade with the original player...
					{
						dataptr += 1;
						retval = session.trade_with_player(trade_to_process, initiating_player_trade, requested_player_trade, datain[dataptr]);
						//need to add return value handling! tell both users what happened with trade (accepted or denied, why)
						send_packet(session, initiating_player_trade, retval, ACCEPT_REJECT_TRADE, servv);
						send_packet(session, requested_player_trade, retval, ACCEPT_REJECT_TRADE, servv);	//may need to change these to not return retval, but something else so that the client wont be getting the real reason why (ex: if req player doesnt have enough cards, then retval will tell the initiating player that the other player doesnt have the cards.)
						//need to zeroize all of the data because its all static variables. only do this for valid. things!
						initiating_player_trade = 0;
						requested_player_trade = 0;
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
						trade_in_progress = 0;
					}
					else
						invalid_sender = 1;	//throw an error flag since you cant trade with a different player while another trade is pending
				}
				else
					invalid_sender = 1;
				//playernum should contain the requested player.
				//the original trade should be used, and then deleted at the end of this case.
				//this should make sure that a trade was proposed by the player in data[0] and the original trade object should be used, not the one send to the user for approval
				//if approved, call trade_with_player(...) function to execute trade. send a message to both parties to tell them the resulting stauts of the trade after (success or fail, and the reason why?)
				//data[0] = player who requested trade
				//data[1] = trade status
				//data[2] = qty wood to trade
				//data[3] = qty wood to receive
				//data[4] = qty ore to trade
				//data[5] = qty ore to receive
				//data[6] = qty brick to trade
				//data[7] = qty brick to receive
				//data[8] = qty wheat to trade
				//data[9] = qty wheat to receive
				//data[10] = qty sheep to trade
				//data[11] = qty sheep to receive
				break;
			case GET_PLAYER_INFO:
				//data[0] = player number
				//data[1] = num bytes in name
				//data[2] = player name
				//data[2+1]	= player name
				//....
				//data[2+data[1]] = player name

				break;
			case SEND_DICE_ROLL:		//this is going to become the new 'roll dice' function.
				//if should only be able to be called by the current player after their turn has started.
				if ((turn_started_already == 1) && (player_number == session.check_current_player()))
				{
					send_dice_roll(session, servv);
					Sleep(50);
					send_resources_all_players(session, servv);	//update players cards with resources from new turn
				}
				else
					invalid_sender = 1;
//old function
//				retval = send_dice_roll(session, servv);
				//maybe just send dice roll to every client?
				break;
			case GET_QTY_ROADS_LEFT:
				retval = get_qty_roads_remaining(session, player_number);
				send_packet(session, player_number, retval, GET_QTY_ROADS_LEFT, servv);
				//data field:
				break;
			case GET_QTY_SETTLEMENTS_LEFT:
				retval = get_qty_settlements_left(session, player_number);
				send_packet(session, player_number, retval, GET_QTY_SETTLEMENTS_LEFT, servv);
				//data field:
				break;
			case GET_QTY_CITIES_LEFT:
				retval = get_qty_cities_left(session, player_number);
				send_packet(session, player_number, retval, GET_QTY_CITIES_LEFT, servv);
				//data field:
				break;
			case BUILD_ROAD:
				//data field:
				//data[0]	=	tile number
				//data[1]	=	road index?
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					retval = session.build_roads(datain[dataptr], player_number, datain[dataptr + 1]);
					if (retval >= 0)		//if a success, then send player new board layout!
						send_board_info(session, servv);
					else
						send_packet(session, player_number, -31, BUILD_ROAD, servv);
				}
				else
					invalid_sender = 1;
				//add some error handling if the road was not able to be built! should tell client so they can make another move
				//if successful, send the board info to all players.
				break;
			case BUILD_SETTLEMENT:
				//data field:
				//data[0]	=	tile number
				//data[1]	=	corner index
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					if (initial_placement_phase == 1)
					{
						retval = session.temp_build_settlement(datain[dataptr], player_number, datain[dataptr + 1]);
						if (retval >= 0)
							send_board_info(session, servv);
						else
							send_packet(session, player_number, -32, BUILD_SETTLEMENT, servv);
					}
					else
					{
						retval = session.build_settlement(datain[dataptr], player_number, datain[dataptr + 1]);
						if (retval >= 0)		//if a success, then send player new board layout!
							send_board_info(session, servv);
						else
							send_packet(session, player_number, -32, BUILD_SETTLEMENT, servv);
					}
				}
				else
					invalid_sender = 1;
				break;
			case UPGRADE_SETTLEMENT:
				//data field:
				//data[0]	=	tile number
				//data[1]	=	corner index
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					retval = session.upgrade_settlement(datain[dataptr], player_number, datain[dataptr + 1]);
					if (retval >= 0)		//if a success, then send player new board layout!
						send_board_info(session, servv);
					else
						send_packet(session, player_number, -33, UPGRADE_SETTLEMENT, servv);
				}
				else
					invalid_sender = 1;
				break;
			case BUY_DV_CARD:		//should allow user to buy more than 1 at once?
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					temp = datain[dataptr++];
					if (temp < MAX_DEV_CARDS_PER_TRANSACTION)
					{
						for (int x = 0; x < temp; x++)
						{
							retval = session.purchase_DV_card(player_number);
							if (retval < 0)
								break;
							tempdata = x+1;
						}
						send_dev_cards(session, player_number, tempdata, retval, servv);
					}
					else
						send_packet(session, player_number, -52, BUY_DV_CARD, servv);
				}
				else
					invalid_sender = 1;
				//data field:
				//data[0]	=	number of DV cards to buy
				break;
			case USE_DV_CARD:
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					//data[0] = card type
					//data[1] = data associated with it. will end up needing more than one int for this field...
					use_dev_card(session, datain[dataptr], player_number, datain[dataptr + 1], servv);
					cout << "Make me able to handle development cards!" << endl;
				}
				else
					invalid_sender = 1;
				break;
			case READ_RESOURCES:
				send_resources(session, player_number, servv);
				//data field:
				break;
			case GET_BOARD_INFO:
				send_board_info(session, servv);
				//data field:
				break;
			case GET_TIME_LIMIT:
				send_packet(session, player_number, 1000, GET_TIME_LIMIT, servv);
				break;
			case START_GAME:
				if (game_status == 0)
				{
					game_status = 1;	//flag to show game is started
					session.start_game(active_num_tiles);
//					session.build_std_board(active_num_tiles);
					cout << "Make START_GAME frame send out player number as data byte. " << endl << "right now its sending out the variable player_number. it needs to come" << endl << "from the player info class" << endl;
					send_start_game(session, servv);
					initial_placement_phase = 1; 
					number_of_init_placements = 1;

					//					send_packet(session, player_number, player_number, START_GAME, servv);
				}
				else
					invalid_sender = 1;
				//data field:
				//**** need a way to get all player names that will be playing in memory before this function. maybe to a join game case to get player info
				break;
			case ACCEPT_GAME:
				break;
			case JOIN_GAME:
				//needs to send player info to server and the server needs to save the following player info off into static fields
				//data[0]	=	number of characters in name
				//data[1-data[0]]	=	name
				//player IP address
				//??
				//need to tell player their player number
				tempchar[0] = datain[dataptr];
				tempdata = atoi(tempchar);
				dataptr += 1;
				for (int x = 0; x < tempdata; x++)
				{
					tempstring += datain[dataptr];
					dataptr += 1;
				}
				retval = join_game(session, player_number, tempstring, servv, tempsocket);
				player_number = session.check_number_of_players();
				send_packet(session, player_number, player_number, JOIN_GAME, servv);
				break;
			case END_TURN:
				if ((session.check_current_player() == player_number) && (turn_started_already == 1))
				{
					retval = session.next_player();
					if (debug_text)
					{
						cout << "Player " << player_number << " has ended their turn." << endl;
						cout << "The next player up is player " << retval << endl;
					}
					trade_in_progress = 0;	//reset this flag just in case a trade never went through. otherwise it would block other people from trading on their turns
					send_end_turn(session, servv, player_number);
					send_board_info(session, servv);
					//need to make this send the command to clients to inform players its someone elses turn! probably should also send board data now
					turn_started_already = 0;
				}
				else
					invalid_sender = 1;
				break;
			case STEAL_CARD_ROBBER:
				//datain[0] = player to steal from
				//datain[1] = tile to place robber
				if ((last_player == player_number) && ((read_dice_roll(session) == 7) || (session.who_can_place_robber(-7) == player_number)))					
				{
					place_robber(session, datain[dataptr + 1], player_number, servv);
					retval = steal_card(session, player_number, datain[dataptr]);
					send_packet(session, player_number, retval, STEAL_CARD_ROBBER, servv);	//tell player they stole a card (and what the card was!)
					temp = datain[dataptr];
					send_packet(session, temp, retval*(-1), STEAL_CARD_ROBBER, servv);	//tell player what card was stolen from them. if > 0, it was aquired, if < 0 it was stolen
				}
				else
					invalid_sender = 1;
				last_player = 0;
				break;
			case END_GAME:
				break;
			case START_TURN:
				//data org:
				//data[0] = current players turn
				//data[1] = is it your turn?
				//data[2] = dice roll
				if ((session.check_current_player() == player_number) && (turn_started_already == 0))
				{
					turn_started_already = 1;
					retval = session.start_turn(0);
					if (debug_text)
					{
						cout << "new turn started! Current player: " << player_number << endl;
						cout << "Dice roll 1: " << retval << endl;
					}
					retval = read_dice_roll(session);
					if (debug_text)
						cout << "Dice roll 2: " << retval << endl;
					for (int x = 1; x < session.check_number_of_players() + 1; x++)
					{
						tempstring = "";
						tempstring += player_number;		//first databyte = current players turn
						if (player_number == x)
							tempstring += '1';		//if it is this players turn, then tell them (2nd byte is indicator)
						else
							tempstring += '0';
						tempstring += retval;		//dice roll
						send_packet(session, x, tempstring, START_TURN, servv);
					}
					//				retval = send_dice_roll(session, servv);		//send out dice roll to every player

					//move to beginning and send this with the start turn packet. receiving multiple packets at a time on client isnt easy right now!)
					//				retval = read_dice_roll(session);
					if (retval == 7)
					{
						//		place_robber(session, );
						last_player = player_number;
						session.who_can_place_robber(player_number);	//update who can place the robber. need to send packet to tell client to update 
					}
					else		//if not a 7, then send out all players resources
					{
						send_resources_all_players(session, servv);	//maybe dont do this yet, so that the player 'rolls' the dice to get this info.
					}
					Sleep(1000);
//					send_resources_all_players(session, servv);
				}
				else
					invalid_sender = 1;
				//Data format:
				//1:		dice roll
				//2 - 7:	resource amnts
				//8:		current players turn
				//retval = send_dice_roll(session);
				//need to make this do a bunch more stuff probably. like update each user of their respective resources and what not.
				break;
			case START_TURN_INIT_PLACEMENT:
				//receiving this packet means that the player is ending their turn. need to process their data, send out board info, and then move to next player.
				//data[0] = tile for settlement
				//data[1] = corner for settlement
				//data[2] = tile for road
				//data[3] = road
				if ((player_number == session.check_current_player()) && (initial_placement_phase == 1))
				{
					retval = session.temp_build_settlement(datain[dataptr] - 1, player_number, datain[dataptr + 1] - 1);
					if (debug_text)
						cout << "Build settlement finished with status: " << retval << endl;
					if (retval > 0)
					{
						Sleep(15);
						retval = session.build_roads(datain[dataptr + 2] - 1, player_number, datain[dataptr + 3] - 1);
						if (debug_text)
							cout << "Build road finished with status: " << retval << endl;
					}
					if (retval > 0)
						session.next_player();		//only go to next player if the settlement/road was successfully built
					else
						number_of_init_placements -= 1;	//if unable to build, reduce this by one to keep the count at the same value after increment.
					Sleep(1000);	//give the clients a chance to update their side
					send_board_info(session, servv);
					Sleep(3000);	//give the clients a chance to update their side
					if (number_of_init_placements >= session.check_number_of_players() * 2)	//if each player has been able to place twice, then end init placement phase
					{
						send_end_init_placement(session, servv);
						initial_placement_phase = 0;
					}
					else
						send_init_placement_start(session, servv);		
					number_of_init_placements++;
				}
				else
					invalid_sender = 1;
				break;
			default:
				send_packet(session, player_number, INVALID_PACKET_OR_SENDER, INVALID_PACKET_OR_SENDER, servv);
				cout << "ERROR: INVALID DATA TYPE! This should probably send an error back to the client saying something like please contact your system administrator" << endl;
				break;
			}
			if (invalid_sender == 1)
			{
				send_packet(session, player_number, INVALID_PACKET_OR_SENDER, INVALID_PACKET_OR_SENDER, servv);
				invalid_sender = 0;
			}
		}
		else
		{
			if (debug_text)
				cout << "Game not started, unable to process non-initiating pakcet" << endl;
			send_packet(session, player_number, INVALID_PACKET_OR_SENDER, INVALID_PACKET_OR_SENDER, servv);
		}
	}
	return(0);
}

int send_init_placement_start(game session, tcpserver servv)
{
	//this needs to notify all players who turn it is to get the ball rolling.
	//this should trigger the first player to begin their placement. they should build one settlement and one road, then the server should go to the next player
	//packet should contain the current player and some data to indicate it is the initial placement phase.
	//data[0]	=	current player
	//data[1]	=	is current player you?

	//START_TURN_INIT_PLACEMENT
	int num = session.check_number_of_players();
	char tempdata[2] = { 0, };
	tempdata[0] = session.check_current_player();	//current player number
	for (int x = 1; x < num + 1; x++)
	{
		if (x == session.check_current_player())
			tempdata[1] = 2;		//this players turn
		else
			tempdata[1] = 1;		//not this players turn
		send_packet(session, x, tempdata, START_TURN_INIT_PLACEMENT, 2, servv);		//make END_TURN be start turn when received from server?
	}
	return(num);
}

int send_end_init_placement(game session, tcpserver servv)
{
	int num = session.check_number_of_players();
	char tempdata[2] = { 0, };
	int tempnum = 0;
	tempdata[0] = session.check_current_player();	//current player number
	tempnum = session.check_current_player();
	for (int x = 1; x < num + 1; x++)
	{
		send_packet(session, x, tempnum, END_INIT_PLACEMENT_PHASE, servv);		//make END_TURN be start turn when received from server?
	}
	return(num);
}

int send_end_turn(game session, tcpserver servv, int prev_player)
{
	//data format:
	//data[0] = next players turn?
	//data[1] = player who ended their turn
	//data[3] = your player number
	int num = session.check_number_of_players();
	char tempdata[3] = { 0, };
	tempdata[0] = session.check_current_player();	//current player number
	tempdata[1] = prev_player;
	for (int x = 1; x < num + 1; x++)
	{
		tempdata[2] = x;		//the receipient's player number
		send_packet(session, x, tempdata, END_TURN, 3, servv);		//make END_TURN be start turn when received from server?
	}
	return(num);
}

int send_start_game(game session, tcpserver servv)
{
	int num = session.check_number_of_players();
	int num1 = session.check_current_player();
	char temp[2] = { 0, };
	temp[1] = num1 + 1;	//ensure its a valid player number.
	for (int x = 1; x < num + 1; x++)
	{
		temp[0] = x;
		send_packet(session, x, temp, START_GAME, 2, servv);
	}
	return(num);
}

int send_dev_cards(game session, int playern, int numcards, int success, tcpserver servv)
{
	int arptr[5];
	char datatosendar[7] = { 0, };
	int tempval = 0;
	session.get_current_dv_cards(arptr, playern);
	if (debug_text)
	{
		cout << endl << "Current dev cards:" << endl;
		cout << "Knights:        " << arptr[0] << endl;
		cout << "Victory Points: " << arptr[1] << endl;
		cout << "Year of plenty: " << arptr[2] << endl;
		cout << "Monopoly:       " << arptr[3] << endl;
		cout << "Build Roads:    " << arptr[4] << endl;
	}
	if (success > 0)	//if greater than 0, then a card was bought and no card buys failed
		datatosendar[0] = 1;
	datatosendar[1] = numcards;
	datatosendar[2] = arptr[0] + 1;
	datatosendar[3] = arptr[1] + 1;
	datatosendar[4] = arptr[2] + 1;
	datatosendar[5] = arptr[3] + 1;
	datatosendar[6] = arptr[4] + 1;
	return(send_packet(session, playern, datatosendar, BUY_DV_CARD, 7, servv));
	//send message! data should be as follows
	//data[0] = fail/success buying dev cards
	//data[1] = number of dev cards bought
	//data[2] = num knights
	//data[3] = num vp
	//data[4] = num year of plenty
	//data[5] = num monopoly
	//data[6] = num build roads
}
int use_knight_dv_card(game &session, int playernum, int tilenum, tcpserver servv)
{
	//this needs to query the user about what player to steal from. The client side can handle selecting where to place the robber. then server can verify it
	//data structure:
	//data[0] = dev card type
	//data[1] = tile to place robber on
	//data[2] = 

	//data struct to send
	//data[0] = updated number of that type of dev card
	//data[1] = number of players available to steal from
	//data[2] = player 1
	//data[3] = ....
	//
	//just call place_robber function and add a flag somewhere to indicate who is allowed to place the robber. update it to allow this player.
	session.who_can_place_robber(playernum);	//update the player who can place robber.
	return(place_robber(session, tilenum, playernum, servv));
}

int use_dev_card(game &session, int cardtype, int playernumm, int data, tcpserver servv)
{
	if ((cardtype > 0) && (cardtype < 6))
	{
		if (cardtype == 1)
			use_knight_dv_card(session, playernumm, data, servv);

	}
	return(-1);
}
int steal_card(game &session, int player_taking_card, int player_giving)
{
	int retval = -1;
	if (session.who_can_place_robber(-7) == player_taking_card)
		retval = session.steal_random_card(player_taking_card, player_giving);
	return(retval);
}

int place_robber(game &session, int tilenum, int playernum, tcpserver servv)
{
	int retval = 0;
	int count = 0;
	int temp = 0;
	int temp2 = 0;
	int players_on_tile[3] = { 0,0,0 };
	char datatosend[10] = { 0, };
	cout << "Need to make server catan check and make sure that this player is allowed ton place the robber" << endl;
	if (session.who_can_place_robber(-7) == playernum)
	{	//data struct to send
		//data[0] = number of players available to steal from
		//data[1] = player 1
		//data[2] = ....
		retval = session.update_robber_position(tilenum);
/*		for (int i = 0; i < 6; i++)
		{
			temp = session.check_corner_owner(i, tilenum);
			temp2 = temp;
			for (int x = 0; x < count; x++)
				if (temp == players_on_tile[x % 3])
					temp2 = 0;
			if (temp2 != 0)
			{
				players_on_tile[count] = temp2;
				datatosend[1 + count] = temp2;
				count += 1;
				temp = 0;
				temp2 = 0;
			}
		}
		datatosend[0] = count;
		send_packet(session, playernum, datatosend, STEAL_CARD_ROBBER, count + 1, servv);
*/

//		send_packet(session, playernum, datatosend, PLACE_ROBBER_PACKET, count + 1, servv);
	}
	return(count);
	cout << "need to make function place_robber in server_catan send a packet to current player to tell them to place the robber and what player to steal a resource card from" << endl;
}

unsigned int read_dice_roll(game session)
{
	return(session.get_current_roll());
}


int send_dice_roll(game session, tcpserver servv)
{
	int temp = read_dice_roll(session);
	for (int x = 1; x < session.check_number_of_players()+1; x++)
		send_packet(session, x, temp, SEND_DICE_ROLL, servv);
	return(temp);
}

/*		This function should never be uncommented! the dice rolling is handled at the beginning of every turn, and should only be read from the game session at this level.
int roll_dice()
{
	int temp = (rand()*rand()) % 11 + 2;	//rand() is seeded whenever board is build, aka start of game.
	return(read_dice_roll(69, temp));
}
*/

int send_board_info(game session, tcpserver servv)
{
	string data_out;
	data_out = session.get_board_info();
	for (int x = 1; x < session.check_number_of_players()+1; x++)
	{
		send_packet(session, x, data_out, GET_BOARD_INFO, servv);		//send board info to all players
	}
	//needs to format and send all data associated with the board. need to define the format!
	return(0);
}

int send_resources(game session, int playernum, tcpserver servv)
{
	int temp[5] = { 0,0,0,0,0 };
	string datastr;
	char tempc[5] = { 0, };
	int temp2 = 0;
	ostringstream convert;
	for (int x = 0; x < 5; x++)
	{
		temp[x] = session.check_resources(playernum, x+1);
		tempc[x] = temp[x];
//		convert << temp2;
	}
//	datastr = convert.str();
//	char *tempc = new char[datastr.length() + 1];
//	for (int x = 0; x < datastr.length(); x++)
//		tempc[x] = datastr[x];
//	strcpy(tempc, datastr.c_str());
	temp2 = send_packet(session, playernum, tempc, READ_RESOURCES,5,servv);
	return(temp2);
}

int send_resources_all_players(game session, tcpserver servv)
{
	int num = session.check_number_of_players();
	for(int x = 1; x <= num; x++)
		send_resources(session, x, servv);
	return(1);	
}

int get_qty_roads_remaining(game session, int player_number)
{
	return(session.get_num_roads(player_number));
}

int get_qty_settlements_left(game session, int player_number)
{
	return(session.get_num_settlements(player_number));
}

int get_qty_cities_left(game session, int player_number)
{
	return(session.get_num_cities(player_number));
}

int join_game(game &session, int &player_number, string name, tcpserver servv, SOCKET sock)
{
	if (game_status == 0)	//if game not started, then allow new players to join
	{
//		player_number = session.check_number_of_players() + 1;
		player_number = session.add_player(session.check_number_of_players()+1, name, sock);
//		tempplayer->set_client_address()
		//needs to figure out what the next player number is, save the clientsocket off to player data, tell the client that, and keep waiting for more players until game is started
	}
	else
		return(-99);
	return(game_status);
	//needs to figure out what the next player number is, save the clientsocket off to player data, tell the client that, and keep waiting for more players until game is started
}

int packetHandler(SOCKET tempsock, char buffer[], int size, tcpserver servv)
{
	char *temp = new char [4096];
	int tempsize = ((size & 0x07F80) >> 7);
	temp[0] = 'S';
	temp[1] = 8;
	temp[2] = 53;
	temp[3] = 'p';
	temp[4] = tempsize;
	temp[5] = (size & 0x07F);
	cout << "Size & 0x0FF: " << (size & 0x0FF) << endl << "Size - (tempsize * 256): " << (size - (tempsize * 256)) << endl;
	for (int x = 0; x < size; x++)
		temp[x + 6] = buffer[x];
//	strcat(temp, buffer);
	if (debug_text)
	{
		cout << "Data to send: " << endl;
		for (int x = 0; x < size + 6; x++)
			cout << +temp[x] << " ";
		cout << endl << endl;
	}
	for (int x = 0; x < size+6; x++)
		txdatabuff[x] = temp[x];
	return(servv.sendPacket(tempsock, temp, size+6));
	delete[] temp;
	Sleep(100);
	return(0);
}

int send_packet(game session, int player_num, int data_to_send, int packet_type, tcpserver servv)
{
//	string datastr;
	SOCKET tempsock;
//	ostringstream convert;
	int retval = 0;
//	convert << packet_type;
//	convert << player_num;
//	convert << data_to_send;
//	datastr = convert.str();
//	char *temp = new char [datastr.length() + 2];
	char temp[3] = { 0, };
	temp[0] = packet_type;
	temp[1] = player_num;
	temp[2] = data_to_send;
//	for (int x = 1; x < 3; x++)
//		temp[x] = datastr.c_str()[x];
//	strcpy(temp, datastr.c_str());
	tempsock = session.get_player_socket(player_num);
	retval = packetHandler(tempsock, temp, 3, servv);
//	retval = serv.sendPacket(tempsock, temp);
//	delete[] temp;
	return(retval);
	//return(sendPacket())
}

int send_packet(game session, int player_num, string data_to_send, int packet_type, tcpserver servv)
{
	string datastr;
	SOCKET tempsock;
	int retval = 0;
	//char tempchar[1] = { packet_type };	// , player_num, data_to_send.length()
	char temp[4096];
	temp[0] = packet_type;
	temp[1] = player_num;
	for (int x = 0; x < data_to_send.length() % 4096; x++)
		temp[x + 2] = data_to_send[x];
//	char *temp = new char[data_to_send.length() + 1];
//	strcpy(temp, tempchar);
//	strcat(temp, data_to_send.c_str());
	tempsock = session.get_player_socket(player_num);
	retval = packetHandler(tempsock, temp, (data_to_send.length()%4096) + 2, servv);
//	retval = serv.sendPacket(tempsock, temp);
//	delete[] temp;
	return(retval);
}

int send_packet(game session, int player_num, char *data_to_send, int packet_type, int length, tcpserver servv)
{
	SOCKET tempsock;
	int retval = 0;
	char temp[4096];
	temp[0] = packet_type;
	temp[1] = player_num;
	for (int x = 0; x < length; x++)
		temp[2 + x] = data_to_send[x];
//	strcat(temp, data_to_send);
	tempsock = session.get_player_socket(player_num);
	retval = packetHandler(tempsock, temp, length + 2, servv);
//	retval = serv.sendPacket(tempsock, temp);
	return(retval);
}
//Notes
/*
How will a game be started?
	a new game should be started when requested by the GUI. For now, I can assume the user will provide the address of the opponent(s).
	upon the request for a new game, the requester will become the server and initiate the connection with clients. when receiving the 
	'start game' packet, the program must assume it is a client and load the senders address as the server. for now this will need to be manually set before the game starts?
	The person who starts the game will also be the host, but the 'server' address should just be local host to simplify the code so there isnt cases to handle if a player is the server or not.
	once the client accepts, it should notify the server. once the server is notified the game has been accepted or declined from all players, it should generate the
	play sequence and then allow the users to build the first settlement/road. once placement phase is over, it will notify the client that it is their turn

How will a turn be handled?
	A turn needs to be handled by sending the client a notification it is their turn! then the client should request a dice roll (when the user tells it to?). 
	the server should precalculate the dice roll whenever it sends the start_turn packet. It should also generate the resources when the start_turn is sent,
	but the server shouldnt update the clients resources until the player requests get_dice_roll

How will end_turn be handled?
	end turn should be handled by confirming with client their turn is over and notify all other players the turn is over. it should then notify the next player it is their turn

How will trades be handled?
	trades should be handled by having a chat window to work out the deal? and then allow the player to send a propose_trade function to the other player with their offer.
	that player can then accept or reject the trade. if it is accepted, the server should detect the resources accordingly and then update the users of their respective resources

How will time limits be handled?
	Shit if I know...
	maybe have a local timer that sends the server a message that the limit has been reached, and end the turn on the players side. the server should then notify all players the turn has ended and start the next turn

How will build/upgrades be handled?
	A player can request the build/upgrade during their turn, and then the server will check if the player can build/upgrade the requested thing. the server should
	then notify the player of the status of the build. if not enough roads/buildings return -1, if not enough resources return -2? if successful, return the number of things they have left.

How will get_board_info be handled?
	the server will send all of the board info (all tile objects and their data) to the requesting player. (this should be sent to all players at the beginning of each turn, or maybe after anything gets built?)
	the server will need to divide the board data into multiple packets more than likely. limit the data to 504 bytes, have byte 510 = something constant like 56?
		and then send a nonzero number if there are more packets to come. 

*/

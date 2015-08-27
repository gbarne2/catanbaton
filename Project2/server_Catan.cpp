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
Will need to have the initialization period set up a listen socket for some port, and then keep listening for new clients until game is started!
Launch this operation in another thread, and once the start game command is received, then rejoin threads and start game.
-have every functio that sends data add its packet type to the begining of the message! 
-make all packets be formatted to whatever the standard format will be. Maybe have sendpacket(...) call a function in this file rahter than tcpserver.cpp. this function would format the data and then call the sendpacket(...) function from tcpserver.cpp.
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
#define CONNECT						50
#define START_TURN					51

//Message format:
/*
index 0-3 = {"S", 8, 53, "p"}		-> validity bits to verify its a real packet!
index 4   = data type (see #defines above)
index 5   = player number of request
index 6 - (size_of_data - 1) = data

*/
using namespace std;

game catan;
map<int, player> playermap;
static int game_status = 0;
tcpserver serv(" ");
//function prototypes
int get_qty_cities_left(int player_number);
int get_qty_settlements_left(int player_number);
int get_qty_roads_remaining(int player_number);
int send_board_info(game session, char *datain, int size_of_datain);
int join_game(string name);

//functions
int framehandler(char *datain, int size_of_data)
{

	//the datain field will need to be pulled in from tcpserver->receivebuffer or whateevr its called. 
	//some higer level function will need to manage this and allow it to receive data and indicate when
	//there is data to be read. when that indicator goes off, the frame handler function should be called
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
		switch (datatype)
		{
		case PROPOSE_TRADE:
			break;
		case ACCEPT_REJECT_TRADE:
			break;
		case GET_PLAYER_INFO:
			break;
		case SEND_DICE_ROLL:
			retval = send_dice_roll();
			//maybe just send dice roll to every client?
			break;
		case GET_QTY_ROADS_LEFT:
			retval = get_qty_roads_remaining(player_number);
			send_packet(player_number, retval, GET_QTY_ROADS_LEFT);
			//data field:
			break;
		case GET_QTY_SETTLEMENTS_LEFT:
			retval = get_qty_settlements_left(player_number); 
			send_packet(player_number, retval, GET_QTY_SETTLEMENTS_LEFT);
			//data field:
			break;
		case GET_QTY_CITIES_LEFT:
			retval = get_qty_cities_left(player_number);
			send_packet(player_number, retval, GET_QTY_CITIES_LEFT);
			//data field:
			break;
		case BUILD_ROAD:
			//data field:
			//data[0]	=	tile number
			//data[1]	=	road index?
			retval = catan.build_roads(datain[dataptr], player_number, datain[dataptr + 1]);
			if (retval >= 0)		//if a success, then send player new board layout!
				send_board_info(catan, nulptr, 0);
			else
				send_packet(player_number, -31, BUILD_ROAD);
			//add some error handling if the road was not able to be built! should tell client so they can make another move
			//if successful, send the board info to all players.
			break;
		case BUILD_SETTLEMENT:
			//data field:
			//data[0]	=	tile number
			//data[1]	=	corner index
			retval = catan.build_settlement(datain[dataptr], player_number, datain[dataptr + 1]);
			if (retval >= 0)		//if a success, then send player new board layout!
				send_board_info(catan, nulptr, 0);
			else
				send_packet(player_number, -32, BUILD_SETTLEMENT);
			break;
		case UPGRADE_SETTLEMENT:
			//data field:
			//data[0]	=	tile number
			//data[1]	=	corner index
			retval = catan.upgrade_settlement(datain[dataptr], player_number, datain[dataptr + 1]);
			if (retval >= 0)		//if a success, then send player new board layout!
				send_board_info(catan, nulptr, 0);
			else
				send_packet(player_number, -32, UPGRADE_SETTLEMENT);
			break;
		case BUY_DV_CARD:		//should allow user to buy more than 1 at once?
			cout << "Need to add ability to buy dev cards in game.cpp and in server_catan.cpp!" << endl;
			//data field:
			//data[0]	=	number of DV cards to buy
			break;
		case READ_RESOURCES:
			send_resources(player_number);
			//data field:
			break;
		case GET_BOARD_INFO:
			send_board_info(catan, nulptr, 0);
			//data field:
			break;
		case GET_TIME_LIMIT:
			break;
		case START_GAME:
			//data field:
			//**** need a way to get all player names that will be playing in memory before this function. maybe to a join game case to get player info
			break;
		case ACCEPT_GAME:
			break;
		case JOIN_GAME:
			//needs to send player info to server and the server needs to save the following player info off into static fields
			//data[0]	=	number of characters in name
			//data[1-data[0]]	=	name
			//player name
			//player IP address
			//??
			//need to tell player their player number
			tempdata = datain[dataptr];
			dataptr += 1;
			for (int x = 0; x < tempdata; x++)
			{
				tempstring += datain[dataptr];
				dataptr += 1;
			}
			retval = join_game(player_number, tempstring);
			break;
		case END_TURN:
			retval = catan.next_player();
			send_board_info(catan, nulptr, 0);
			send_packet(retval, 0, END_TURN);		//make END_TURN be start turn when received from server?
			//need to make this send the command to clients to inform players its someone elses turn! probably should also send board data now
			break;
		case END_GAME:
			break;
		case START_TURN:
			retval = catan.start_turn(0);
			read_dice_roll(69, retval);
			//Data format:
			//1:		dice roll
			//2 - 7:	resource amnts
			//8:		current players turn
			//retval = send_dice_roll();
			//need to make this do a bunch more stuff probably. like update each user of their respective resources and what not.
			break;
		default:
			cout << "ERROR: INVALID DATA TYPE! This should probably send an error back to the client saying something like please contact your system administrator" << endl;
			break;
		}
	}
}

int read_dice_roll(int opt, int opt2)
{
	static int roll_val = 0;
	if (opt == 69)
		roll_val = opt2;
	return(roll_val);
}
int send_dice_roll()
{
	int temp = read_dice_roll(0, 0);
	for (int x = 1; x < catan.check_number_of_players() + 1; x++)
		send_packet(x, temp, SEND_DICE_ROLL);
	return(temp);
}

int roll_dice()
{
	int temp = (rand()*rand()) % 11 + 2;	//rand() is seeded whenever board is build, aka start of game.
	return(read_dice_roll(69, temp));
}

int send_board_info(game session, char *datain, int size_of_datain)
{
	string data_out;
	data_out = session.get_board_info();
	for (int x = 1; x < session.check_number_of_players()+1; x++)
	{
		send_packet(x, data_out, GET_BOARD_INFO);		//send board info to all players
	}
	//needs to format and send all data associated with the board. need to define the format!
}

int send_resources(int playernum)
{
	int temp[5] = { 0,0,0,0,0 };
	string datastr;
	int temp2 = 0;
	ostringstream convert;
	for (int x = 1; x < 7; x++)
	{
		temp2 = catan.check_resources(playernum, x);
		convert << temp2;
	}
	datastr = convert.str();
	char *tempc = new char[datastr.length() + 1];
	strcpy(tempc, datastr.c_str());
	temp2 = send_packet(playernum, tempc, READ_RESOURCES);
	return(temp2);
}
int get_qty_roads_remaining(int player_number)
{
	return(catan.get_num_roads(player_number));
}

int get_qty_settlements_left(int player_number)
{
	return(catan.get_num_settlements(player_number));
}

int get_qty_cities_left(int player_number)
{
	return(catan.get_num_cities(player_number));
}

int join_game(int player_number, string name)
{
	if (game_status == 0)	//if game not started, then allow new players to join
	{
		catan.add_player(catan.check_number_of_players() + 1, name);

//		tempplayer->set_client_address()
		//needs to figure out what the next player number is, save the clientsocket off to player data, tell the client that, and keep waiting for more players until game is started
	}
	else
		return(-99);
	//needs to figure out what the next player number is, save the clientsocket off to player data, tell the client that, and keep waiting for more players until game is started
}

int send_packet(int player_num, int data_to_send, int packet_type)
{
	string datastr;
	SOCKET tempsock;
	ostringstream convert;
	int retval = 0;
	convert << packet_type;
	convert << data_to_send;
	datastr = convert.str();
	char *temp = new char [datastr.length() + 2];
	strcpy(temp, datastr.c_str());
	tempsock = catan.get_player_socket(player_num);
	retval = serv.sendPacket(tempsock, temp);
	delete[] temp;
	return(retval);
	//return(sendPacket())
}

int send_packet(int player_num, string data_to_send, int packet_type)
{
	string datastr;
	SOCKET tempsock;
	int retval = 0;
	char tempchar[1] = { packet_type };
	char *temp = new char[data_to_send.length() + 2];
	strcpy(temp, tempchar);
	strcat(temp, data_to_send.c_str());
	tempsock = catan.get_player_socket(player_num);
	retval = serv.sendPacket(tempsock, temp);
	delete[] temp;
	return(retval);
}

int send_packet(int player_num, char *data_to_send, int packet_type)
{
	string datastr;
	SOCKET tempsock;
	ostringstream convert;
	int retval = 0;
	char temp[4096];
	temp[0] = packet_type;
	strcat(temp, data_to_send);
	tempsock = catan.get_player_socket(player_num);
	retval = serv.sendPacket(tempsock, temp);
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
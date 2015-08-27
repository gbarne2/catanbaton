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

#include "tcpserver.h"
#include <thread>
#include "tcpserver.h""
#include <iostream>
#include "game.h"
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
index 5   = player number of request?
index 6 - (size_of_data - 1) = data

*/
int framehandler(char *datain, int size_of_data)
{

	//the datain field will need to be pulled in from tcpserver->receivebuffer or whateevr its called. 
	//some higer level function will need to manage this and allow it to receive data and indicate when
	//there is data to be read. when that indicator goes off, the frame handler function should be called

	char datatype = datain[4];
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
			//maybe just send dice roll to every client?
			break;
		case GET_QTY_ROADS_LEFT:
			//data field:
			//data[0]	=	player number
			break;
		case GET_QTY_SETTLEMENTS_LEFT:
			//data field:
			//data[0]	=	player number
			break;
		case GET_QTY_CITIES_LEFT:
			//data field:
			//data[0]	=	player number
			break;
		case BUILD_ROAD:
			//data field:
			//data[0]	=	player number
			//data[1]	=	tile number
			//data[2]	=	road index?
			break;
		case BUILD_SETTLEMENT:
			//data field:
			//data[0]	=	player number
			//data[1]	=	tile number
			//data[2]	=	corner index
			break;
		case UPGRADE_SETTLEMENT:
			//data field:
			//data[0]	=	player number
			//data[1]	=	tile number
			//data[2]	=	corner index
			break;
		case BUY_DV_CARD:		//should allow user to buy more than 1 at once?
			//data field:
			//data[0]	=	player number
			//data[1]	=	number of DV cards to buy
			break;
		case READ_RESOURCES:
			//data field:
			//data[0]	=	player number
			break;
		case GET_BOARD_INFO:
			//data field:
			//data[0]	=	player number	-> may just not have any data and may just force it to be sent to all players?
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
			//player name
			//player IP address
			//player number (needs to generate it!)
			//??
			break;
		case END_TURN:
			break;
		case END_GAME:
			break;
		default:
			cout << "ERROR: INVALID DATA TYPE! This should probably send an error back to the client saying something like please contact your system administrator" << endl;
			break;
		}
	}
}

int send_board_info(game session, char *datain, int size_of_datain)
{
	string data_out;
	data_out = session.get_board_info();
	//needs to format and send all data associated with the board. need to define the format!
}

int get_qty_roads_remaining(int player_number)
{

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
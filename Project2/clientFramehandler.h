#pragma once

#include "Tile_client.h"
#include <vector>
#include "playerClient.h"

#define INVALID_TRADE				-49
#define ACTIVE_NUM_TILES_CLIENT		19

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
#define JOIN_GAME						46
#define END_TURN						47
#define END_GAME						48
#define STEAL_CARD_ROBBER				49
#define CONNECT							50
#define START_TURN						51
#define MAX_PACKET_VAL					51
#define RESET_STATIC_VAR_IN_FUNCTION	-57


#define APPROVE_TRADE 					1
#define DENY_TRADE						-43

struct client_trade_cards_offer
{
	unsigned int qty_wood_to_trade;
	unsigned int qty_wood_to_receive;
	unsigned int qty_ore_to_trade;
	unsigned int qty_ore_to_receive;
	unsigned int qty_brick_to_trade;
	unsigned int qty_brick_to_receive;
	unsigned int qty_wheat_to_trade;
	unsigned int qty_wheat_to_receive;
	unsigned int qty_sheep_to_trade;
	unsigned int qty_sheep_to_receive;
};

static int flag_rx_packet_needs_processing = 0;		//when set to 1, this will indicate that something has been received that needs processing.
static tileclient board[ACTIVE_NUM_TILES_CLIENT];
static int debug_text = 1;

/* Globals	*/
static client_trade_cards_offer trade_to_process;
static playerClient playerdata;

int clientFrameHandler(char* datain);
vector<int> get_num_active_tiles(int data);
int update_board_info(char* data, int datasize);
int dice_roll(int data);
int trade_status(int data);
int last_packet_sent(int data);
int request_player_trade(int player_action);
int time_limit(int data);

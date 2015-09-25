#pragma once

#define INVALID_TRADE				-49
#define ACTIVE_NUM_TILES_CLIENT		19

struct trade_cards_offer
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


vector<int> get_num_active_tiles(int data);
int update_board_info(char* data, int datasize);
int dice_roll(int data);
int trade_status(int data);
int last_packet_sent(int data);
int request_player_trade(int player_action);
int time_limit(int data);
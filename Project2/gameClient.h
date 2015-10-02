#pragma once

#include "Tile_client.h"
#include "playerClient.h"
#include <vector>

#ifndef MAX_NUM_ACTIVE_TILES
#define MAX_NUM_ACTIVE_TILES 19
#endif

#ifndef active_num_tiles
#define active_num_tiles 19
#endif

#ifndef FLAGS
#define FLAGS				0
#define F_TURN_START		1
#define F_TURN_END			2
#define F_BUILD_ROAD		3
#define F_BUILD_SETTLEMENT	4
#define F_BUILD_CITY		5
#endif

class gameClient
{
	int players;
	tileclient board[MAX_NUM_ACTIVE_TILES];
	vector<playerClient> player_list;
	int assign_resources();
	int build_board();
	//Flag definitions
	static int FLAG_TURN_START;
	static int FLAG_MY_TURN;
	static int FLAG_BUILD_ROAD;
	static int FLAG_BUILD_CITY;
	static int FLAG_BUILD_SETTLEMENT;
public:
	playerClient playerinfo;
	gameClient();
	~gameClient();
	int build_road(int tile, int corner);
	int build_settlement(int tile, int corenr);
	int build_city(int tile, int corner);
	int rolldice();
	int check_current_player();
	int start_turn();
	int end_turn();
	int get_player_num();
	int refresh_cards();
	int update_flag(int, int);
	int update_board(char*, int, int, int);
};




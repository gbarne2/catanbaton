#pragma once

#include "Tile_client.h"
#include "playerClient.h"
#include <vector>
#include "tcpclient.h"

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

#ifndef MAX_NUM_PLAYERS
#define MAX_NUM_PLAYERS		4
#endif

extern tcpclient clienttcp;
extern int num_dev_cards_bought;

class gameClient
{
    int current_player;
    int players;
    int current_roll;
    vector<playerClient> player_list;
    int assign_resources();
    int build_board();
    int rolldice();
    //Flag definitions
    int FLAG_TURN_START;
    int FLAG_MY_TURN;
    int FLAG_BUILD_ROAD;
    int FLAG_BUILD_CITY;
    int FLAG_BUILD_SETTLEMENT;
    int FLAG_PLACE_ROBBER;
public:
    tileclient board[MAX_NUM_ACTIVE_TILES];
    playerClient playerinfo;
    gameClient();
    ~gameClient();
    SOCKET initsocketthing();

 //flags
    int resources_flag;
    int dice_roll_flag;
    int init_game_placement;
    int begin_turn_init_placement;

 //functions
    int startGame();
    int place_initial_settlement_road(int tileset, int corner, int tileroad, int road);
    int check_player_resource_amt(int);
    int build_road(int tile, int corner);
    int build_settlement(int tile, int corenr);
    int build_city(int tile, int corner);
    int check_current_player();
    int set_current_player(int);
    int check_num_players();
    int check_corner_owner(int corner, int tilenum) {return(board[tilenum].check_corner_owner(corner));}
    vector<int> check_players_on_tile(int tilenum);
    int check_corner_building_type(int corner, int tilenum) { return(board[tilenum].check_corner_building_type(corner));}
    int get_dice_roll(int tilenum)  {return(board[tilenum].check_dice_roll());}
    int start_turn();
    int end_turn();
    int get_player_num();
    int check_tile_dice_roll(int tilenum);
    int Get_dice_roll();
    int update_dice_roll(int);
    int refresh_cards();
    int update_flag(int, int);
    int update_board(char*, int, int, int);
    int check_resource(int);
    int get_board_info();
    int set_player_number(int);
    int get_corner_owner(int, int);
    int get_corner_bldg_type(int tilenum, int corner);
    int get_road_owner(int road, int tilenum);
    int rx_packet_checker(int packet_type);
    int place_robber(int, int);
    int buy_dv_cardd(int);
    int get_qty_dv_cardd(int);
    int use_dev_cardd(int);
    int joinGame();
};



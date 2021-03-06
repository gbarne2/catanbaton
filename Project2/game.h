#pragma once

#include <vector>
#include "player.h"
#include "Tile.h"
#include "serverMain.h"

#define APPROVE_TRADE 	1
#define DENY_TRADE	-43

#ifndef START_DECK_QTY
#define START_DECK_QTY			1
#define START_KNIGHTS			15
#define START_YEAR_OF_PLENTY	2
#define START_BUILD_ROADS		2
#define START_MONOPOLY			1
#define START_VICTORY_POINTS	5
#define START_QTY_DEV_CARDS		25
#endif

#ifndef COORDS_AND_TILE_DATA
#define COORDS_AND_TILE_DATA	1
#define X_tiles 5
#define Y_tiles 5
#define active_num_tiles 19

#define cornA	0
#define cornB	1
#define cornC	2
#define cornD	3
#define cornE	4
#define cornF	5

#define roadAB	0
#define roadBC	1
#define roadCD	2
#define roadDE	3
#define roadEF	4
#define roadFA	5

#define coordAx	0
#define coordAy	2
#define coordBx	2
#define coordBy	2
#define coordCx	1
#define coordCy	2
#define coordDx	2
#define coordDy	1
#define coordEx	0
#define coordEy	1
#define coordFx	1
#define coordFy	1

#define roadABx 0
#define roadABy 2
#define roadBCx 2
#define roadBCy 2
#define roadCDx 2
#define roadCDy 0
#define roadDEx 0
#define roadDEy 1
#define roadEFx 1
#define roadEFy 1
#define roadFAx 1
#define roadFAy 0
#endif

struct dev_card_deck_struct
{
	unsigned int qty_victory_points_left;
	unsigned int qty_build_roads_left;
	unsigned int qty_year_of_plenty_left;
	unsigned int qty_knights_left;
	unsigned int qty_monopoly_left;
	vector<unsigned int> dev_card_deck;
};

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

class game
{
private:
	dev_card_deck_struct DV_deck;
	int* player_turn_array;
	tile pieces[X_tiles][Y_tiles]; //data[x][y], invalid indexes will need to be checked against! ((0,3),(0,4),(1,3),(3,1),(4,0),(4,1) dont exist)
//	vector<tile> pieces;
	int players;
	int which_player_can_place_robber;
	int current_robber_tile;
	unsigned int current_roll;
	vector<player>::iterator player_ptr;
	int assign_resources();
	int current_player;
	int rolldice();
	int tile_number[X_tiles][Y_tiles];
	int x_index[active_num_tiles];
	int y_index[active_num_tiles];
	int longest_road;
	int largest_army;
	int create_player_order();
	int determine_if_neighbor_tile_occupied(int corner_numbz, int tile_number, int playernum, int& xcoord1, int& ycoord1, int& corner1, int& xcoord2, int& ycoord2, int& corner2);
	int determine_tile_from_index(int, int);
	int determine_x_index_from_tile(int);
	int determine_y_index_from_tile(int);
	int check_resources_settlement(int playernum);
	int deduct_resources_settlement(int playernum);
	int deduct_resources_city(int playernum);
	int deduct_resources_road(int playernum);
	int check_resources_road(int playernum);
	int check_number_roads_left(int);
	int check_number_settlements_left(int);
	int check_number_cities_left(int);
	void get_corners_from_road(int, int&, int&);
	int deduct_resources_devcard(int playernum);
	int determine_neighbor_tile_road(int, int, int, int&, int&, int&);
	int deduct_resources(int, int, int);
	int deduct_resources_trade(trade_cards_offer, int, int);
	int check_resources_trade(trade_cards_offer, int, int);
	int calculate_card_to_steal(int playernum);
	int deduct_resources_trade_low(int, int, int);
	void initialize_dev_card_deck();
	int placements_per_player = 0;
	int set_docks();

public:
	game(void); 
	int reset_placements_per_player() { placements_per_player = 1; return(placements_per_player); }
	int check_tile_resource_type(int);
	int next_player(void);
	int get_dock_type(int tile, int corner);
	int check_current_player(void);
	int check_number_of_players(void); 
//	int create_player_order(int);
	int delete_player(int);
	vector<player> player_list;			//,make this private!
	int get_num_roads(int);
	int get_num_settlements(int);
	int get_num_cities(int);
	int check_corner_owner(int corner, int tilenum);
	int get_road_owner(int, int);
	int check_resources_city(int playernum);
	int upgrade_settlement(int tilenum, int playernum, int cornernum);
	int check_resources_devcard(int playernum);
	int purchase_DV_card(int playernum);
	int redeem_DV_card(int playernum, int dvcard);
	int use_port_to_trade_cards(int playernum, int type, int qty, int requested_card);
	int check_num_docks_by_player(int playernum);
	int get_dock_by_index(int playernum, int index);
	int who_can_place_robber(int);
	void get_current_dv_cards(int (&arrayarray)[5], int player_number);
	int check_corner_building_type(int corner, int tilenum);
	~game(void);
	int build_std_board(int);		//this function needs to create the appropriate number of tiles and index them so that they can be easily manuvered around
	int verify_valid_coordinates(int x_coord, int y_coord);
	int build_roads(int, int, int);
	int build_settlement(int tile_number, int playernum, int corner_numbz);
	int delete_settlement(int tilenum, int playernum, int corner_numbz);
	int add_player(int player_num, string player_name, SOCKET);
	int check_resources(int player, int type);
	int steal_random_card(int playernum, int player_to_steal_from);
	int start_turn(int);
	int start_game(int);
	int get_dice_roll(int);			//used to get the dice roll of a tile.
	unsigned int get_current_roll(void);		//used to read what the current dice roll is for the turn.
	int trade_with_player(trade_cards_offer, int, int, int);
	string get_board_info(void);
	SOCKET get_player_socket(int);
	int update_robber_position(int new_pos);
//	void trade_cards(int player1, int player2, vector<int> offer, vector<int> request);	//player1 offers x to player2 for request.
			//trade_cards function needs to check if each player has the request. it should also let player1 offer something and not
			//request something. if request is empty, then player2 can make an offer and then player1 can accept. dont allow free trades.
			//also should let player2 make counteroffer
	int temp_build_settlement(int tile_number, int playernum, int corner_numbz)		//remove this function!
	{
		//make this function check the neighboring tiles corresponding corner in addition to this tles ones. if tile x,y coord invalid, there is not a neighbor!
		//need to come up with way to check neighboring tile!!
//		static int placements_per_player = 0;
		static int max_player_number = 0;
		int retval	= 0;
		int xcoord	= 0;
		int ycoord	= 0;
		int temp	= 0;	
		int temp1	= 0;
		int xcoord1	= 0;
		int ycoord1	= 0;
		int corner1	= -1;
		int xcoord2	= 0;			//2nd neighbor to check
		int ycoord2	= 0;			//2nd neighbor to check
		int corner2	= -1;
		static int start = 1;
		if (playernum > max_player_number)
		{	//if this is true, then we havnt heard from all players and placements_per_player cant be 2
			max_player_number = playernum;
			placements_per_player = 1;
			start = 1;
		}
		xcoord = determine_x_index_from_tile(tile_number);
		ycoord = determine_y_index_from_tile(tile_number);
		temp = determine_if_neighbor_tile_occupied(corner_numbz, tile_number,playernum,xcoord1,ycoord1,corner1,xcoord2,ycoord2,corner2);
		temp1 = check_resources_settlement(playernum);
		if ((temp > 0) && (temp1 > 0))
		{
			pieces[xcoord][ycoord].build_settlement(corner_numbz, playernum);
			if ((pieces[xcoord][ycoord].get_dock_type(corner_numbz) != 0) && (pieces[xcoord][ycoord].get_dock_type(corner_numbz) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
			{
				player_ptr = player_list.begin() + playernum % (players + 1);
				player_ptr->add_dock_to_player(pieces[xcoord][ycoord].get_dock_type(corner_numbz));
			}
			Sleep(50);
			if ((xcoord1 != xcoord) || (ycoord1 != ycoord))
			{
				pieces[xcoord1][ycoord1].build_settlement(corner1, playernum);
				if ((pieces[xcoord1][ycoord1].get_dock_type(corner1) != 0) && (pieces[xcoord1][ycoord1].get_dock_type(corner1) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
				{
					player_ptr = player_list.begin() + playernum % (players + 1);
					player_ptr->add_dock_to_player(pieces[xcoord1][ycoord1].get_dock_type(corner1));
				}
				Sleep(50);
			}
			if (((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1)))
			{
				pieces[xcoord2][ycoord2].build_settlement(corner2, playernum);
				if ((pieces[xcoord2][ycoord2].get_dock_type(corner2) != 0) && (pieces[xcoord2][ycoord2].get_dock_type(corner2) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
				{
					player_ptr = player_list.begin() + playernum % (players + 1);
					player_ptr->add_dock_to_player(pieces[xcoord2][ycoord2].get_dock_type(corner2));
				}
				Sleep(50);
			}
			temp = deduct_resources_settlement(playernum);
			if (!start && (playernum == 1))
			{
				placements_per_player = 2;
			}
			else if ((placements_per_player == 2) && (playernum == max_player_number))
				initial_placement_phase = 0;
			start = 0;
		}
		else
			temp = -37;
		return(temp);

	}

};


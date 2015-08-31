#pragma once

#include <vector>
#include "player.h"
#include "Tile.h"

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
class game
{
private:
	tile pieces[X_tiles][Y_tiles]; //data[x][y], invalid indexes will need to be checked against! ((0,3),(0,4),(1,3),(3,1),(4,0),(4,1) dont exist)
//	vector<tile> pieces;
	int players;
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
	int determine_if_neighbor_tile_occupied(int corner_numbz, int tile_number, int playernum, int& xcoord1, int& ycoord1, int& corner1, int& xcoord2, int& ycoord2, int& corner2);
	int determine_tile_from_index(int, int);
	int determine_x_index_from_tile(int);
	int determine_y_index_from_tile(int);
	int check_resources_settlement(int playernum);
	int deduct_resources_settlement(int playernum);
	int deduct_resources_city(int playernum);
	int deduct_resources_road(int playernum);
	void get_corners_from_road(int, int&, int&);
	int deduct_resources_devcard(int playernum);
	int determine_neighbor_tile_road(int, int, int, int&, int&, int&);
public:
	game(void);
	int next_player(void);
	int check_current_player(void);
	int check_number_of_players(void);
	vector<player> player_list;			//,make this private!
	int get_num_roads(int);
	int get_num_settlements(int);
	int get_num_cities(int);
	int check_corner_owner(int corner, int tilenum);
	int check_resources_city(int playernum);
	int upgrade_settlement(int tilenum, int playernum, int cornernum);
	int check_resources_devcard(int playernum);
	int check_corner_building_type(int corner, int tilenum);
	~game(void);
	int build_std_board(int);		//this function needs to create the appropriate number of tiles and index them so that they can be easily manuvered around
	int verify_valid_coordinates(int x_coord, int y_coord);
	int build_roads(int, int, int);
	int build_settlement(int tile_number, int playernum, int corner_numbz);
	int add_player(int player_num, string player_name);
	int check_resources(int player, int type);
	int start_turn(int);
	int start_game(int, vector<string>);
	int get_dice_roll(int);			//used to get the dice roll of a tile.
	int get_current_roll(void);		//used to read what the current dice roll is for the turn.
	string get_board_info(void);
	SOCKET get_player_socket(int);
	void trade_cards(int player1, int player2, vector<int> offer, vector<int> request);	//player1 offers x to player2 for request.
			//trade_cards function needs to check if each player has the request. it should also let player1 offer something and not
			//request something. if request is empty, then player2 can make an offer and then player1 can accept. dont allow free trades.
			//also should let player2 make counteroffer
	int temp_build_settlement(int tile_number, int playernum, int corner_numbz)		//remove this function!
	{
		//make this function check the neighboring tiles corresponding corner in addition to this tles ones. if tile x,y coord invalid, there is not a neighbor!
		//need to come up with way to check neighboring tile!!
		int retval = 0;
		int xcoord	= 0;
		int ycoord	= 0;
		int temp	= 0;	
		int xcoord1	= 0;
		int ycoord1	= 0;
		int corner1	= -1;
		int xcoord2	= 0;			//2nd neighbor to check
		int ycoord2	= 0;			//2nd neighbor to check
		int corner2	= -1;
		xcoord = determine_x_index_from_tile(tile_number);
		ycoord = determine_y_index_from_tile(tile_number);
		determine_if_neighbor_tile_occupied(corner_numbz, tile_number,playernum,xcoord1,ycoord1,corner1,xcoord2,ycoord2,corner2);
		pieces[xcoord][ycoord].build_settlement(corner_numbz, playernum);
		if ((xcoord1 != xcoord) || (ycoord1 != ycoord))
			pieces[xcoord1][ycoord1].build_settlement(corner1, playernum);
		if (((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1)))
			pieces[xcoord2][ycoord2].build_settlement(corner2, playernum);
//		temp = deduct_resources_settlement(playernum);
		return(temp);

	}

};


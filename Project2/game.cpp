#include <iostream>
#include <iomanip>
#include "player.h"
#include "Tile.h"
#include "game.h"
#include <ctime>
#include <algorithm>

using namespace std;
/*
*to do:
**see about changing all return errors for functions that dont need to return a value to throws and catch at highest level?
**add ability to handle a dice roll of 7 to start_turn function!
add ability to buy dev card
*/
const int const_valid_x_index_array[active_num_tiles] = {0,0,0,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4};
const int const_valid_y_index_array[active_num_tiles] = {0,1,2,0,1,2,3,0,1,2,3,4,1,2,3,4,2,3,4};
const int max_y = 4;
const int max_x = 4;

const int tile_resources_array[active_num_tiles] = {WHEAT, WHEAT, WHEAT, WHEAT, ORE, ORE, ORE, WOOD, 
													WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP,
													BRICK, BRICK, BRICK, DESERT};
//need to make the tile generation function randomly assign the resource types to the tiles without repeating any of the indexes.
//may need to redo this.
/*
						   coordinates are (x,y)
							    /�������\
							   /  (4,4)	 \
                      /�������\\   18	 //�������\						To find touching corners on other hexs, subtract the following to the x,y coordinate of current tile (if you are on corner A, and looking for corner C, add C's values)
		             /  (3,4)  \\_______//	(4,3)  \						A-C-E Navigation matrix:
			/�������\\	  15   //�������\\	  17   //�������\				A = (0,2)
		   /  (2,4)	 \\_______//  (3,3)	 \\_______//  (4,2)	 \				C = (1,2)
		   \    11	 //�������\\	14	 //�������\\   16	 /				E = (0,1)
		    \_______//	(2,3)  \\_______//	(3,2)  \\_______/
			/�������\\	 10	   //�������\\	 13    //�������\
		   /  (1,3)	 \\_______//  (2,2)	 \\_______//  (3,1)	 \				B-D-F Navigation matrix:
		   \	6	 //�������\\	9	 //�������\\	12	 /				B = (2,2)
		    \_______//	(1,2)  \\_______//	(2,1)  \\_______/				D = (2,1)
			/�������\\	  5	   //�������\\	  8	   //�������\				F = (1,1)
		   /  (0,2)	 \\_______//  (1,1)	 \\_______//  (2,0)	 \
		   \	2	 //�������\\	4	 //�������\\	7	 /
		    \_______//	(0,1)  \\_______//	(1,0)  \\_______/
					 \	  1    //�������\\	  3    /
					  \_______//  (0,0)	 \\_______/
							   \	0	 /
							    \_______/

*/

//this function generates the dice rolls! it keeps looping until it generates a unique number.

int game::next_player()
{
	current_player = (current_player + 1) % (players + 1);
	if (current_player == 0)
		current_player += 1;
	return(player_turn_array[current_player]);
}

int game::check_current_player()
{
//	return(player_turn_array[current_player]);
	return(current_player);
}

int game::rolldice()
{
	static int dice_rolls[active_num_tiles] = {2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
	int temp = 0;
startagainrolldice:
	temp = (rand()*rand()) % 11 + 2;
	for(int x = 0; x < active_num_tiles; x++)
	{
		if(temp == dice_rolls[x])		//if random number = one in dice_rolls, then assign it to tile
		{
			dice_rolls[x] = 0;
			goto exit_diceroll;
		}
	}
	goto startagainrolldice;
//	__nop();
exit_diceroll:
	return(temp);
}

//calling function must check if resource == desert. if it does,m then it needs to assign a dice roll of 0. or 1. whatever
//generates pseudo random resources based on what resources have not been allocated yet
static int rand_resources[active_num_tiles] = {WHEAT, WHEAT, WHEAT, WHEAT, ORE, ORE, ORE, WOOD, 
													WOOD, WOOD, WOOD, SHEEP, SHEEP, SHEEP, SHEEP,
													BRICK, BRICK, BRICK, DESERT};

int game::assign_resources()
{
	int temp = 0;
startagain_assign_resources:
	temp = (rand()*rand()) % 6 + 1;
	for(int x = 0; x < active_num_tiles; x++)
	{
		if(temp == rand_resources[x])		//if random number = one in dice_rolls, then assign it to tile
		{
			rand_resources[x] = 0;
			goto exit_assign_resources;
		}
	}
	goto startagain_assign_resources;
//	__nop();
exit_assign_resources:
	return(temp);
}

int game::create_player_order()
{
	int temp = 0;
	int* tempturns = new int[players];
	player_turn_array = new int[players];
	for (int y = 0; y < players; y++)
	{
		player_turn_array[y] = 0;
		tempturns[y] = y + 1;
	}
	for (int z = 0; z < players; z++)
	{
	start_player_order_again:
		temp = (rand()*rand()) % players + 1;
		for (int x = 0; x < players; x++)
		{
			if (tempturns[x] == temp)
			{
				tempturns[x] = 0;
				player_turn_array[z] = temp;
				goto exit_player_ordering;
			}
		}
	exit_player_ordering:
		__nop;
	}
	delete[] tempturns;
	for (int x = 0; x < players; x++)
	{
		if (player_turn_array[x] == 0)
		{
			return(-78);
		}
	}
	
	return(1);
}

int game::delete_player(int playertoremove)
{
	int retval = 1;
	if (player_list.size() >= playertoremove)	//if this player actually is in the game...
	{
		player_list.erase(player_list.begin() + playertoremove);
		players -= 1;
	}
	else
		retval = -1;
	return(retval);
}
//this function needs to create the appropriate number of tiles and index them so that they can be easily manuvered around
int game::build_std_board(int size)
{
	int resource = 0;
	int dice = 0;
	srand(time(0));
	current_player = 1;
	//must create tile, put it into pieces array, and update tile number in other array
	set_docks();
	for(int x = 0; x < active_num_tiles; x++)
	{
		x_index[x] = const_valid_x_index_array[x];
		y_index[x] = const_valid_y_index_array[x];
		tile_number[x_index[x]][y_index[x]] = x;
//		temptile.
		resource = assign_resources();
		if (resource == DESERT)
		{
			dice = 1;
			current_robber_tile = x;
		}
		else
			dice = rolldice();
		pieces[x_index[x]][y_index[x]].set_resource_type(resource);
		pieces[x_index[x]][y_index[x]].set_dice_roll(dice);
	}
	return(1);
}

int game::check_tile_resource_type(int tilenum)
{
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	return(pieces[xcoord][ycoord].check_tile_resource_type());
}

int game::determine_tile_from_index(int x, int y)
{
	int tile_numberlocal = -1;
	for(int i = 0; i < active_num_tiles; i++)
	{
		if((x == x_index[i]) && (y == y_index[i]))
		{
			tile_numberlocal = i;
			break;
		}
	}
	return(tile_numberlocal);
}

int game::determine_x_index_from_tile(int tile_number)
{
	int temp = -1;
	if(tile_number < active_num_tiles)
		temp = x_index[tile_number];
	return(temp);
}

int game::determine_y_index_from_tile(int tile_number)
{
	int temp = -1;
	if(tile_number < active_num_tiles)
		temp = y_index[tile_number];
	return(temp);

}

//retval of 0 = error, invalid!
int game::verify_valid_coordinates(int x_coord, int y_coord)
{
	//check if x_coord is between min and max, y coord is between min and max, and that x,y isnt prohibited
	return(((x_coord >= 0) && (x_coord < 5)) && ((y_coord >= 0) && (y_coord < 5)) && (!(((x_coord == 0) && (y_coord == 3)) || ((x_coord == 0) && (y_coord == 4)) || 
		((x_coord == 1) && (y_coord == 4)) || ((x_coord == 3) && (y_coord == 0)) || ((x_coord == 4) && (y_coord == 0)) || ((x_coord == 4) && (y_coord == 1)))));
}


//this needs to check if a settlement can be built. if it can, build one. if not, return error.

int game::determine_neighbor_tile_road(int road_numb, int tile_number, int playernum, int& xcoord1, int& ycoord1, int& road1)
{
	int tempx = 0;
	int tempy = 0;
	int retval = 0;
	xcoord1 = determine_x_index_from_tile(tile_number);
	ycoord1 = determine_y_index_from_tile(tile_number);
	
	tempx = xcoord1;
	tempy = ycoord1;

	switch(road_numb)
	{
	case roadAB:	xcoord1 += (roadABx - roadDEx);
					ycoord1 += (roadABy - roadDEy);
					road1 = roadDE;
					break;
	case roadBC:	xcoord1 += (roadBCx - roadEFx);
					ycoord1 += (roadBCy - roadEFy);
					road1 = roadEF;
					break;
	case roadCD:	xcoord1 += (roadCDx - roadFAx);
					ycoord1 += (roadCDy - roadFAy);
					road1 = roadFA;
					break;
	case roadDE:	xcoord1 += (roadDEx - roadABx);
					ycoord1 += (roadDEy - roadABy);
					road1 = roadAB;
					break;
	case roadEF:	xcoord1 += (roadEFx - roadBCx);
					ycoord1 += (roadEFy - roadBCy);
					road1 = roadBC;
					break;
	case roadFA:	xcoord1 += (roadFAx - roadCDx);
					ycoord1 += (roadFAy - roadCDy);
					road1 = roadCD;
					break;
	default:
		break;
	}
	
	//if invalid index, there is no neighboring tile!
	//if no tile, then make the xcoord, ycoord, and corner the same as the original to be updated.
	if((xcoord1 < 0) || (ycoord1 < 0) || (xcoord1 > max_x) || (ycoord1 > max_y))
	{
		xcoord1 = tempx;
		ycoord1 = tempy;
		road1 = road_numb%6;
	}
	//do a sanity check. if neighbor tile road owner != the specified road owner, make them equal to the nonzero value.
	
	retval = pieces[xcoord1][ycoord1].get_road_owner(road1);		//if 0, a road is not currently here.
	if (retval != pieces[tempx][tempy].get_road_owner(road_numb))
	{
		if (retval == 0)	//if the other tile has no owner but the main one does, then update this one
		{
			pieces[xcoord1][ycoord1].roads[road_numb] = pieces[tempx][tempy].get_road_owner(road_numb);
			retval = pieces[tempx][tempy].get_road_owner(road_numb);
		}
		else if (pieces[tempx][tempy].get_road_owner(road_numb) == 0)
			pieces[tempx][tempy].roads[road_numb] = retval;
	}
	return(retval);
		//check_neighbors(corner1, playernum) && pieces[xcoord2][ycoord1].check_neighbors(corner2, playernum));
}
SOCKET game::get_player_socket(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return(player_ptr->get_client_socket());
}

int game::determine_if_neighbor_tile_occupied(int corner_numbz, int tile_number, int playernum, int& xcoord1, int& ycoord1, int& corner1, int& xcoord2, int& ycoord2, int& corner2)
{
	int tempx = 0;
	int tempy = 0;
	int tempcorner = 0;
	xcoord1 = determine_x_index_from_tile(tile_number);
	ycoord1 = determine_y_index_from_tile(tile_number);
	tempx = xcoord1;
	tempy = ycoord1;
	tempcorner = corner_numbz;
	xcoord2 = xcoord1;
	ycoord2 = ycoord1;
	//if at corner A, must check C and E. need to do current coordinates + current corner values - new corner coords
	switch(corner_numbz)
	{
	case cornA:		xcoord1 += (coordAx - coordCx);
					ycoord1 += (coordAy - coordCy);
					corner1 = cornC;
					corner2 = cornE;
					xcoord2 += (coordAx - coordEx);
					ycoord2 += (coordAy - coordEy);
					break;
	case cornB:		xcoord1 += (coordBx - coordDx);
					ycoord1 += (coordBy - coordDy);
					xcoord2 += (coordBx - coordFx);
					ycoord2 += (coordBy - coordFy);
					corner1 = cornD;
					corner2 = cornF;
					break;
	case cornC:		xcoord1 += (coordCx - coordAx);
					ycoord1 += (coordCy - coordAy);
					xcoord2 += (coordCx - coordEx);
					ycoord2 += (coordCy - coordEy);
					corner1 = cornA;
					corner2 = cornE;
					break;
	case cornD:		xcoord1 += (coordDx - coordBx);
					ycoord1 += (coordDy - coordBy);
					xcoord2 += (coordDx - coordFx);
					ycoord2 += (coordDy - coordFy);
					corner1 = cornB;
					corner2 = cornF;
					break;
	case cornE:		xcoord1 += (coordEx - coordAx);
					ycoord1 += (coordEy - coordAy);
					xcoord2 += (coordEx - coordCx);
					ycoord2 += (coordEy - coordCy);
					corner1 = cornA;
					corner2 = cornC;
					break;
	case cornF:		xcoord1 += (coordFx - coordBx);
					ycoord1 += (coordFy - coordBy);
					xcoord2 += (coordFx - coordDx);
					ycoord2 += (coordFy - coordDy);
					corner1 = cornB;
					corner2 = cornD;
					break;
	default:	return(-1);
		break;
	}
	//if invalid index, there is no neighboring tile!
	//if no tile, then make the xcoord, ycoord, and corner the same as the original to be updated.
	if ((xcoord1 < 0) || (ycoord1 < 0) || (xcoord1 > max_x) || (ycoord1 > max_y))
	{
		xcoord1 = tempx;
		ycoord1 = tempy;
		corner1 = tempcorner;
	}
	if((xcoord2 < 0) || (ycoord2 < 0) || (xcoord2 > max_x) || (ycoord2 > max_y))
	{
		xcoord2 = tempx;
		ycoord2 = tempy;
		corner2 = tempcorner;
	}
	return(pieces[xcoord1][ycoord1].check_neighbors(corner1, playernum) && pieces[xcoord2][ycoord2].check_neighbors(corner2, playernum));
}

int game::get_num_roads(int player_num)
{
	player_ptr = player_list.begin() + player_num;
	return(player_ptr->roads_left());
}

int game::get_num_settlements(int player_num)
{
	player_ptr = player_list.begin() + player_num;
	return(player_ptr->settlements_left());
}

int game::get_num_cities(int player_num)
{
	player_ptr = player_list.begin() + player_num;
	return(player_ptr->cities_left());	
}
//check if the player has enough resources to build a settlement
int game::check_resources_settlement(int playernum)
{
	//it costs 1 wheat, 1 brick, 1 wood, and 1 sheep to build a settlement
	player_ptr = player_list.begin() + playernum;
	return((player_ptr->check_resource_amount(WHEAT) > 0) && (player_ptr->check_resource_amount(WOOD) > 0) && 
		(player_ptr->check_resource_amount(SHEEP) > 0) && (player_ptr->check_resource_amount(BRICK) > 0));
}

int game::check_resources_city(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return((player_ptr->check_resource_amount(WHEAT) >= 2) && (player_ptr->check_resource_amount(ORE) >= 3));
}
int game::check_resources(int playernum, int type)
{
	int temp = 0;
//	if(playernum == 0)
	player_ptr = player_list.begin() + playernum;
//	else
//		player_ptr = player_list.begin() + playernum - 1;
	temp = player_ptr->check_resource_amount(type);
	return(temp);
}
void game::initialize_dev_card_deck()
{
	DV_deck.qty_build_roads_left = START_BUILD_ROADS;
	DV_deck.qty_knights_left = START_KNIGHTS;
	DV_deck.qty_monopoly_left = START_MONOPOLY;
	DV_deck.qty_victory_points_left = START_VICTORY_POINTS;
	DV_deck.qty_year_of_plenty_left = START_YEAR_OF_PLENTY;
	DV_deck.dev_card_deck.erase(DV_deck.dev_card_deck.begin(), DV_deck.dev_card_deck.end());
	int flag_no_more_vp = 0;
	int flag_no_more_year_plenty = 0;
	int flag_no_more_monopoly = 0;
	int flag_no_more_roads = 0;
	int flag_no_more_knights = 0;
	int tempcard = 0;
	int cards_left[5] = { START_KNIGHTS, START_VICTORY_POINTS, START_YEAR_OF_PLENTY, START_MONOPOLY, START_BUILD_ROADS };
	for (int x = 0; x < 5; x++)
	{
		for (int i = 0; i < cards_left[x]; i++)
		{
			DV_deck.dev_card_deck.push_back(x+1);
		}
	}
	std::random_shuffle(DV_deck.dev_card_deck.begin(), DV_deck.dev_card_deck.end());
/*
	for (int x = 0; x < START_QTY_DEV_CARDS; x++)
	{
		if (!flag_no_more_year_plenty || !flag_no_more_vp || !flag_no_more_monopoly || !flag_no_more_roads)	//if any cards other than knights left, then do this.
		{
		loop_determine_next_card:
			tempcard = (rand()*rand() * 100) % 5;	//generate temp card index for card array with qty left
			if (cards_left[tempcard] > 0)	//if any more of this type of card, then we can add the card to the deck.
			{
				cards_left[tempcard] -= 1;
				DV_deck.dev_card_deck.push_back(tempcard + 1);	//tempcard + 1 = the card type number!
			}
			else
			{
				switch (tempcard)
				{
				case 0:	flag_no_more_knights = 1;
					break;
				case 1: flag_no_more_vp = 1;
					break;
				case 2: flag_no_more_year_plenty = 1;
					break;
				case 3: flag_no_more_monopoly = 1;
					break;
				case 4:	flag_no_more_roads = 1;
					break;
				default: tempcard = 0;
					break;
				}
				goto loop_determine_next_card;
			}
		}
		else	//no more cards other than knights, so just push back a knight and let the loop finish
		{
			DV_deck.dev_card_deck.push_back(1);
		}
	}
*/
}

int game::check_resources_devcard(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return((player_ptr->check_resource_amount(WHEAT) >= 1) && (player_ptr->check_resource_amount(ORE) >= 1) && (player_ptr->check_resource_amount(SHEEP) >= 1));

}

int game::purchase_DV_card(int playernum)
{
	int retval = -26;
	if (DV_deck.dev_card_deck.size() > 0)
	{
		if (check_resources_devcard(playernum) > 0) //enough resources to do it!
		{
			player_ptr = player_list.begin() + (playernum % (players + 1));
			//need some function to generate and manage the devcard deck!
			retval = DV_deck.dev_card_deck.back();		//get next devcard.
			player_ptr->purchase_dev_card(retval);
			DV_deck.dev_card_deck.pop_back();
			deduct_resources_devcard(playernum);
		}
	}
	else
		retval = -27;
	return(retval);
}

int game::redeem_DV_card(int playernum, int dvcard)
{
	//this needs to handle all the fun dev card implementation crap...
	cout << "make redeem_DC_card handle each dev card! in game.cpp" << endl;
	return(0);
}

int game::use_port_to_trade_cards(int playernum, int type, int qty, int requested_card)
{
	player_ptr = player_list.begin() + playernum;
	return(player_ptr->use_dock_to_trade(type, qty, requested_card));
}

int game::check_num_docks_by_player(int playernum)
{
	player_ptr = player_list.begin() + (playernum % (players + 1));
	return(player_ptr->get_number_of_docks());
}

int game::get_dock_by_index(int playernum, int index)
{
	player_ptr = player_list.begin() + (playernum % (players + 1));
	return(player_ptr->get_dock_by_index(index));
}

int game::who_can_place_robber(int playernumm)
{
	if ((playernumm >= 0) && (playernumm <= player_list.size()+1))
		which_player_can_place_robber = playernumm;
	return(which_player_can_place_robber);
}

void game::get_current_dv_cards(int (&arrayarray)[5], int player_number)
{
	player_ptr = player_list.begin() + player_number;
	arrayarray[0] = player_ptr->check_qty_devcard(1);
	arrayarray[1] = player_ptr->check_qty_devcard(2);
	arrayarray[2] = player_ptr->check_qty_devcard(3);
	arrayarray[3] = player_ptr->check_qty_devcard(4);
	arrayarray[4] = player_ptr->check_qty_devcard(5);
}
int game::check_resources_road(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return((player_ptr->check_resource_amount(WOOD) >= 1) && (player_ptr->check_resource_amount(BRICK) >= 1));
}

int game::check_number_of_players(void)
{
	return(players);
}

int game::deduct_resources_settlement(int playernum)
{
	int retval = 0;
	vector<player>::iterator player_ptr1;
	//it costs 1 wheat, 1 brick, 1 wood, and 1 sheep to build a settlement
	player_ptr1 = player_list.begin() + playernum;
	retval  = player_ptr1->update_resources(WHEAT,	-1);
	retval += player_ptr1->update_resources(WOOD,	-1);
	retval += player_ptr1->update_resources(SHEEP,	-1);
	retval += player_ptr1->update_resources(BRICK,	-1);
	player_ptr1->update_settlements(-1);
	return(retval);
}

int game::deduct_resources_city(int playernum)
{
	int retval = 0;
	//it costs 2 wheat and 3 ore to upgrade to a city
	player_ptr = player_list.begin() + playernum;
	retval  = player_ptr->update_resources(WHEAT,	-2);
	retval += player_ptr->update_resources(ORE,	-3);
	player_ptr->update_cities(-1);
	player_ptr->update_settlements(1);
	return(retval);
}

int game::deduct_resources_road(int playernum)
{
	int retval = 0;
	//it costs 1 wood and 1 brick to build a road
	player_ptr = player_list.begin() + playernum;
	retval  = player_ptr->update_resources(WOOD,	-1);
	retval += player_ptr->update_resources(BRICK,	-1);
	player_ptr-> update_roads(-1);
	return(retval);
}

int game::deduct_resources_devcard(int playernum)
{
	int retval = 0;
	//it costs 1 wheat, 1 ore, and 1 sheep for a devcard
	player_ptr = player_list.begin() + playernum;
	retval  = player_ptr->update_resources(WHEAT,	-1);
	retval += player_ptr->update_resources(ORE,	-1);
	retval += player_ptr->update_resources(SHEEP,	-1);
	return(retval);
}
int game::check_number_cities_left(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return(player_ptr->cities_left());
}

int game::upgrade_settlement(int tilenum, int playernum, int cornernum)
{
	int retval = -99;
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	int xcoord1, xcoord2, ycoord1, ycoord2, corner1, corner2;
	determine_if_neighbor_tile_occupied(cornernum, tilenum, playernum, xcoord1, ycoord1, corner1, xcoord2, ycoord2, corner2);
	if (check_resources_city(playernum) && (check_number_cities_left(playernum) >= 1))
	retval = check_corner_owner(cornernum, tilenum);
	if (retval == playernum)	//if the player owns the corner in question, proceed!
	{
		retval = check_corner_building_type(cornernum, tilenum);
		if (retval == 1)	//if there is currently a settlement, then player is allowed to upgrade.
		{
			if (check_resources_city(playernum))
			{
				retval = pieces[xcoord][ycoord].upgrade_settlement(cornernum, playernum);
				if ((xcoord1 != xcoord) || (ycoord1 != ycoord))
					pieces[xcoord1][ycoord1].upgrade_settlement(corner1, playernum);
				if (((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1)))
					pieces[xcoord2][ycoord2].upgrade_settlement(corner2, playernum);
				deduct_resources_city(playernum);
			}

				//build the city
			else
				retval = -35;
		}
		else
			retval = -34;
	}
	else
		retval = -33;
	return(retval);

}

int game::get_road_owner(int road, int tilenum)
{
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	return(pieces[xcoord][ycoord].read_road(road));
}
int game::check_corner_owner(int corner, int tilenum)
{
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	return(pieces[xcoord][ycoord].check_corner_owner(corner));
}

int game::check_corner_building_type(int corner, int tilenum)
{
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	return(pieces[xcoord][ycoord].check_corner_building_type(corner));
}

//this function must check if neighboring cell can have a road built. it will probably end up handling roads by number, and
//it must determine which tile that road corresponds to.

void game::get_corners_from_road(int road_numb, int& corner1, int& corner2)
{
	switch(road_numb)
	{
	case roadAB:	corner1 = 0;
					corner2 = 1;
					break;
	case roadBC:	corner1 = 1;
					corner2 = 2;
					break;
	case roadCD:	corner1 = 2;
					corner2 = 3;
					break;
	case roadDE:	corner1 = 3;
					corner2 = 4;
					break;
	case roadEF:	corner1 = 4;
					corner2 = 5;
					break;
	case roadFA:	corner1 = 5;
					corner2 = 0;
					break;
	default:
		break;
	}
}
int game::check_number_roads_left(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return(player_ptr->roads_left());
}

int game::build_roads(int tile_number, int playernum, int road_numb)
{
	int road1 = 0;
	int xcoord1 = 0;
	int ycoord1 = 0;
	int retval = 0;
	int retval1 = 0;
	int xcoord2 = 0;
	int ycoord2 = 0;
	int xcoord3 = 0;
	int ycoord3 = 0;
	int corner1 = 0;
	int corner2 = 0;
	vector<Corner>::iterator localptr;
	int cornn1 = -1;
	int cornn2 = -1;
	int tempx = determine_x_index_from_tile(tile_number);
	int tempy = determine_y_index_from_tile(tile_number);
	if(check_resources_road(playernum) && (check_number_roads_left(playernum) >= 1))
	{
		get_corners_from_road(road_numb, cornn1, cornn2);
		retval = determine_neighbor_tile_road(road_numb, tile_number, playernum, xcoord1, ycoord1, road1);
		//***A road touches 6 corners, not 2! (two of the main hex, two of the parallel hex, 1 of the hex on either end of road). 
		//must update all four, not just two!
		//if what im thinking is true, i should be able to build a settlement at 3.0 by building roads 0.4, 0.3, and 3.5
		if (retval == 0)		//if no road present on neighboring tile...
		{
			retval = pieces[tempx][tempy].build_road(cornn1, cornn2, playernum);
			if (retval > 0)
			{
/*				localptr = pieces[tempx][tempy].cornersz.begin() + cornn1;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);		//update who is connected to each corner
				localptr = pieces[tempx][tempy].cornersz.begin() + cornn2;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
*/
				//update the 2 other touching corners for the end of the road going to cornn1
				determine_if_neighbor_tile_occupied(cornn1, tile_number, playernum, xcoord2, ycoord2, corner1, xcoord3, ycoord3, corner2);	//use this to get the three corners to update on either side of the road
				if ((xcoord2 != tempx) || (ycoord2 != tempy))
				{
					localptr = pieces[xcoord2][ycoord2].cornersz.begin() + corner1;
					localptr->road_connected += 1;
					localptr->players_connected.push_back(playernum);
				}
				if (((xcoord3 != tempx) || (ycoord3 != tempy)) && ((xcoord2 != xcoord3) || (ycoord2 != ycoord3)))
				{
					localptr = pieces[xcoord3][ycoord3].cornersz.begin() + corner2;
					localptr->road_connected += 1;
					localptr->players_connected.push_back(playernum);
				}
				//update the other 2 touching corners for the end of the road going to cornn2
				//only update if the touching corner is actually for a different tile...
				determine_if_neighbor_tile_occupied(cornn2, tile_number, playernum, xcoord2, ycoord2, corner1, xcoord3, ycoord3, corner2);	//use this to get the three corners to update on either side of the road
				if (((xcoord2 != tempx) || (ycoord2 != tempy)) && ((xcoord2 <= max_x) && (ycoord2 <= max_y)))
				{
					localptr = pieces[xcoord2][ycoord2].cornersz.begin() + corner1;
					localptr->road_connected += 1;
					localptr->players_connected.push_back(playernum);
				}
				if (((xcoord3 != tempx) || (ycoord3 != tempy)) && ((xcoord2 != xcoord3) || (ycoord2 != ycoord3)) && (xcoord3 <= max_x) && (ycoord3 <= max_y))
				{
					localptr = pieces[xcoord3][ycoord3].cornersz.begin() + corner2;
					localptr->road_connected += 1;
					localptr->players_connected.push_back(playernum);
				}

				//build road on parallel tile? (only needed if one exists!)
				if (((xcoord1 != tempx) || (ycoord1 != tempy)) && ((xcoord1 <= max_x) && (ycoord1 <= max_y)))
				{
					get_corners_from_road(road1, cornn1, cornn2);
					retval1 = pieces[xcoord1][ycoord1].build_road(cornn1, cornn2, playernum);		//may need to go after the code below
//					localptr = pieces[xcoord1][ycoord1].cornersz.begin() + cornn1;
//					localptr->road_connected += 1;
//					localptr->players_connected.push_back(playernum);		//update who is connected to each corner
//					localptr = pieces[xcoord2][ycoord2].cornersz.begin() + cornn2;
//					localptr->road_connected += 1;
//					localptr->players_connected.push_back(playernum);
				}

			}
			else
				cerr << "I cant build a road for you." << endl;		//if retval = or < 0, then road was unable to be built
			if ((retval > 0) || (retval1 > 0))
				deduct_resources_road(playernum);
		}
		else
			cerr << "I cant build a road for you." << endl;
	}
	else
		cerr << "I cant build a road for you." << endl;
	return(retval);
}

int game::check_number_settlements_left(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return(player_ptr->settlements_left());
}

int game::delete_settlement(int tilenum, int playernum, int corner_numbz)
{
	int xcoord = 0;
	int ycoord = 0;
	int xcoord1 = 0;
	int ycoord1 = 0;
	int corner1 = -1;
	int xcoord2 = 0;			//2nd neighbor to check
	int ycoord2 = 0;			//2nd neighbor to check
	int corner2 = -1;
	int temp = 0;

	xcoord = determine_x_index_from_tile(tilenum);
	ycoord = determine_y_index_from_tile(tilenum);
	temp = determine_if_neighbor_tile_occupied(corner_numbz, tilenum, playernum, xcoord1, ycoord1, corner1, xcoord2, ycoord2, corner2);

	pieces[xcoord][ycoord].remove_settlement(corner_numbz, playernum);
	if (((xcoord1 != xcoord) || (ycoord1 != ycoord)) && (xcoord1 <= max_x) && (ycoord1 <= max_y))
		pieces[xcoord1][ycoord1].remove_settlement(corner1, playernum);
	if ((((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1))) && (xcoord2 <= max_x) && (ycoord2 <= max_y))
		pieces[xcoord2][ycoord2].remove_settlement(corner2, playernum);
	return(0);
}

int game::build_settlement(int tile_number, int playernum, int corner_numbz)
{
	//make this function check the neighboring tiles corresponding corner in addition to this tles ones. if tile x,y coord invalid, there is not a neighbor!
	//need to come up with way to check neighboring tile!!
	int retval = 0;
	int xcoord	= 0;
	int ycoord	= 0;
	int temp	= 0;		
	int temp1 = -1;	
	int temp2 = -1;
	int temp3 = -1;
	int temp4 = -1;
	int xcoord1	= 0;
	int ycoord1	= 0;
	int corner1	= -1;
	int xcoord2	= 0;			//2nd neighbor to check
	int ycoord2	= 0;			//2nd neighbor to check
	int corner2	= -1;
	xcoord = determine_x_index_from_tile(tile_number);
	ycoord = determine_y_index_from_tile(tile_number);
	temp2 = determine_if_neighbor_tile_occupied(corner_numbz, tile_number,playernum,xcoord1,ycoord1,corner1,xcoord2,ycoord2,corner2);
	temp3 = check_resources_settlement(playernum);
	temp1 = pieces[xcoord][ycoord].check_build_settlement_tile(corner_numbz, playernum);
	temp4 = check_number_settlements_left(playernum);
/*	if(pieces[xcoord][ycoord].check_build_settlement_tile(corner_numbz, playernum) && 
		determine_if_neighbor_tile_occupied(corner_numbz, tile_number,playernum,xcoord1,ycoord1,corner1,xcoord2,ycoord2,corner2)
		&& check_resources_settlement(playernum))
*/
	if ((temp1 > 0) && (temp2 > 0) && (temp3 > 0) && (temp4 > 0))
	{
		pieces[xcoord][ycoord].build_settlement(corner_numbz, playernum);
		if ((pieces[xcoord][ycoord].get_dock_type(corner_numbz) != 0) && (pieces[xcoord][ycoord].get_dock_type(corner_numbz) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
		{
			player_ptr = player_list.begin() + playernum % (players + 1);
			player_ptr->add_dock_to_player(pieces[xcoord][ycoord].get_dock_type(corner_numbz));
		}
		if (((xcoord1 != xcoord) || (ycoord1 != ycoord)) && (xcoord1 <= max_x) && (ycoord1 <= max_y))
		{
			pieces[xcoord1][ycoord1].build_settlement(corner1, playernum);
			if ((pieces[xcoord1][ycoord1].get_dock_type(corner1) != 0) && (pieces[xcoord1][ycoord1].get_dock_type(corner1) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
			{
				player_ptr = player_list.begin() + playernum % (players + 1);
				player_ptr->add_dock_to_player(pieces[xcoord1][ycoord1].get_dock_type(corner1));
			}
		}
		if ((((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1))) && (xcoord2 <= max_x) && (ycoord2 <= max_y))
		{
			pieces[xcoord2][ycoord2].build_settlement(corner2, playernum);
			if ((pieces[xcoord2][ycoord2].get_dock_type(corner2) != 0) && (pieces[xcoord2][ycoord2].get_dock_type(corner2) != NO_DOCK))	//if there is a dock on this tile, then update the player object.
			{
				player_ptr = player_list.begin() + playernum % (players + 1);
				player_ptr->add_dock_to_player(pieces[xcoord2][ycoord2].get_dock_type(corner2));
			}
		}
		temp = deduct_resources_settlement(playernum);
	}
	else if (temp4 == 0)
		temp = -37;
	return(temp);
}
//0,3;0,4;1,4;3,0;4,0;4,1


int game::add_player(int player_num, string player_name, SOCKET sock)
{
	player_list.push_back(player(player_num, player_name, sock));
	players += 1;
	return(players);
}

/*
void game::trade_cards(int player1, int player2, vector<int> offer, vector<int> request)	//player1 offers x to player2 for request.
			//trade_cards function needs to check if each player has the request. it should also let player1 offer something and not
			//request something. if request is empty, then player2 can make an offer and then player1 can accept. dont allow free trades.
			//also should let player2 make counteroffer
{
	cout << "Trading is prohibited you dirty girl" << endl;
	throw("Trading has not been implemented yet");
}
*/

int game::get_dice_roll(int tilenum)
{
	tilenum = tilenum % active_num_tiles;
	return(pieces[determine_x_index_from_tile(tilenum)][determine_y_index_from_tile(tilenum)].check_dice_roll());
}


int game::set_docks()
{
	int retval = 0;
	//this function needs to sassign all the docks!
	for (int i = 0; i < active_num_tiles; i++)
	{
		cout << "Make game::set_docks() set all the dock values for tiles! right now it can just be static! make sure it matches the layout!" << endl;
	}
	return(retval);
}

//this function returns the dock type from the specified tile and corner.
int game::get_dock_type(int tilenum, int corner)
{
	return(pieces[determine_x_index_from_tile(tilenum)][determine_y_index_from_tile(tilenum)].get_dock_type(corner));
}

//this function needs to roll the dice, compute the resources awarded to each player, give those resources to player, and handle the case of 7!
int game::start_turn(int rollnumva)
{
	int temppppp = 0;
//	int rollnumvalamt = 0;
	int retval = 0;
	int tempres = 0;
	int rollnumvalamt = 0;
	srand(time(0));
	rollnumvalamt = (rand()*rand()) % 11 + 2;
	current_roll = rollnumvalamt;				//save current_roll value. this is the only place that this variable should ever be changed!
	
	for(int i = 1; i < players+1; i++)
	{
		player_ptr = player_list.begin() + i;
		for(int x = 0; x < active_num_tiles; x++)
		{
			if(rollnumvalamt == get_dice_roll(x))
			{
				temppppp = pieces[const_valid_x_index_array[x]][const_valid_y_index_array[x]].calculate_resources_from_roll_by_player(i, tempres);
//				tempres = pieces[const_valid_x_index_array[x]][const_valid_y_index_array[x]].check_tile_resource_type();	//not needed, previous function passes tempres by reference and gets this data
				retval = player_ptr->update_resources(tempres, temppppp);
			}
		}
		player_ptr += 1;
	}
	cout << "make game.cpp start_turn function handle when a 7 is rolled!" << endl;
	return(rollnumvalamt);
}

//this function sets up the board on the server side and adds players to the game (with their names)
//the return value contains the number of players in a game
int game::start_game(int size)
{//this needs to initialize the game. this function could also ahndle the initial placements, but for now ill have a higher level
	//function handle that (one that can actually communicate with the clients)
	int resource = 0;
	int dice = 0;
	SOCKET tempsock = INVALID_SOCKET;
	int x = 1;
	int temp_num_players = 0;
	srand(time(0));
	build_std_board(active_num_tiles);
	initialize_dev_card_deck();
	create_player_order();
	//must create tile, put it into pieces array, and update tile number in other array
/*
	for (int x = 0; x < size; x++)
	{
		x_index[x] = const_valid_x_index_array[x];
		y_index[x] = const_valid_y_index_array[x];
		tile_number[x_index[x]][y_index[x]] = x;
		//		temptile.
		resource = assign_resources();
		if (resource == DESERT)
			dice = 1;
		else
			dice = rolldice();
		pieces[x_index[x]][y_index[x]].set_resource_type(resource);
		pieces[x_index[x]][y_index[x]].set_dice_roll(dice);
	}
*/
//	add_player(0, "", tempsock);		//make empty player so indexes are protected. player 0 is used as a null.
	//need to handle adding players here?
	x = 1;
	temp_num_players = players;

//	for (vector<string>::iterator tempptr = player_names.begin(); tempptr < player_names.end(); tempptr++)
//	{
//		temp_num_players = add_player(x, tempptr->c_str(), ) - 1;
//		x++;
//	}
	return(temp_num_players);
}

int game::deduct_resources(int player, int resource, int qty)
{
	player_ptr = player_list.begin() + player;
	if (qty > 0)		//if non-negative number, assume they meant to deduct the amount and multiply by neg1
		qty = qty*(-1);
	return(player_ptr->update_resources(resource, qty));
}

int game::deduct_resources_trade_low(int player, int resource, int qty)
{
	int retval = 0;
	player_ptr = player_list.begin() + player;
	if (((player_ptr->check_resource_amount(resource) >= qty) && qty < 0) || (qty > 0))		//if dudcting resources, make sure there are enough. or add resources
		retval = player_ptr->update_resources(resource, qty);
	else
		retval = -46;
	return(player_ptr->update_resources(resource, qty));
}

int game::deduct_resources_trade(trade_cards_offer trade_card, int playernum, int req_player)
{
	int retval = 0;
	retval = check_resources_trade(trade_card, playernum, req_player);
	if(retval >= 0)
	{
		retval = deduct_resources_trade_low(playernum, WOOD, trade_card.qty_wood_to_trade);
		retval = deduct_resources_trade_low(playernum, ORE, trade_card.qty_ore_to_trade);
		retval = deduct_resources_trade_low(playernum, BRICK, trade_card.qty_brick_to_trade);
		retval = deduct_resources_trade_low(playernum, WHEAT, trade_card.qty_wheat_to_trade);
		retval = deduct_resources_trade_low(playernum, SHEEP, trade_card.qty_sheep_to_trade);
		retval = deduct_resources_trade_low(req_player, WOOD, trade_card.qty_wood_to_receive);
		retval = deduct_resources_trade_low(req_player, ORE, trade_card.qty_ore_to_receive);
		retval = deduct_resources_trade_low(req_player, BRICK, trade_card.qty_brick_to_receive);
		retval = deduct_resources_trade_low(req_player, WHEAT, trade_card.qty_wheat_to_receive);
		retval = deduct_resources_trade_low(req_player, SHEEP, trade_card.qty_sheep_to_receive);
	}
	return(retval);
}
//-40 = current player doesnt have enough resources
//-41 = requested player doesnt have enough resources
//-42 = neither player has enough resources
int game::check_resources_trade(trade_cards_offer trade, int playernum, int req_player)
{
	int tempcheck = 1;
	int tempcheckreq = 1;
	int retval = 1;
	vector<player>::iterator req_player_ptr;
	req_player_ptr = player_list.begin() + req_player;
	player_ptr = player_list.begin() + playernum;
	//for all checks below, if either player does not have enough cards to fulfill trade, then return error that tells which player doesnt have enough cards
	if(trade.qty_wood_to_trade < player_ptr->check_resource_amount(WOOD))
		tempcheck = -40;
	if(trade.qty_ore_to_trade < player_ptr->check_resource_amount(ORE))
		tempcheck = -40;		
	if(trade.qty_brick_to_trade < player_ptr->check_resource_amount(BRICK))
		tempcheck = -40;
	if(trade.qty_wheat_to_trade < player_ptr->check_resource_amount(WHEAT))
		tempcheck = -40;
	if(trade.qty_sheep_to_trade < player_ptr->check_resource_amount(SHEEP))
		tempcheck = -40;
	if(trade.qty_wood_to_receive < req_player_ptr->check_resource_amount(WOOD))
		tempcheckreq = -41;
	if(trade.qty_ore_to_receive < req_player_ptr->check_resource_amount(ORE))
		tempcheckreq = -41;
	if(trade.qty_brick_to_receive < req_player_ptr->check_resource_amount(BRICK))
		tempcheckreq = -41;
	if(trade.qty_wheat_to_receive < req_player_ptr->check_resource_amount(WHEAT))
		tempcheckreq = -41;
	if(trade.qty_sheep_to_receive < req_player_ptr->check_resource_amount(SHEEP))
		tempcheckreq = -41;
	if(tempcheck == 1)
		if(tempcheckreq == 1)		//if both = 1, then both players had enough resources
			retval = 0;
		else				//if tempcheck = 1 but tempcheckreq != 1, then req player doesnt have enough resources
			retval = tempcheckreq;
	else if(tempcheckreq == 1)		//if tempcheck == 0, current player who started trade doesnt have enough resources. check is req player does. if neither do return -42.
		retval = tempcheck;	//if tempcheckreq = 1, then only the player who initialized trade doesnt have enough
	else
		retval = -42;
	return(retval);
}

int game::update_robber_position(int new_tile)
{
	int xcoord = 0;
	int ycoord = 0;
	int retval = -60;
	int temp = 0;
	int players_on_tile[3] = { 0,0,0 };
	if (current_robber_tile != new_tile)
	{
		xcoord = determine_x_index_from_tile(new_tile);
		ycoord = determine_y_index_from_tile(new_tile);
		temp = pieces[xcoord][ycoord].check_robber();
		//2nd check because why not?
		if (!temp)		//if no robber, then we can place the robber on this tile.
		{
			pieces[xcoord][ycoord].place_robber();
			retval = 1;
			current_robber_tile = new_tile;
		}
		else
		{
			cout << "Robber already placed on this tile!" << endl;
			retval = -61;

		}
	}
	return(retval);
}

unsigned int game::get_current_roll()
{
	return(current_roll);
}

//trade_cards is a struct defined in game.h
//player will contain the player number that started the trade request. if this number does not match the current player number, the trade is invalid
//if the requested player is different than player and current player, then continue
//this function should be called after checking with the requested player if they accept the trade, which means the calling function should call check_resources_trade before querying user.
int game::trade_with_player(trade_cards_offer trade, int playernum, int requested_player, int status_of_trade)
{
	int retval = 0;
	if(status_of_trade == APPROVE_TRADE)
	{
		//the two lines below are done in the deduct_resources_trade functoin to ensure the resources cant be deducted maliciously
	//	retval = check_resources_trade(trade, playernum, requested_player);	
	//	if(retval >= 0)
		retval = deduct_resources_trade(trade, playernum, requested_player);		//checks if resources are sufficient and then deducts them if they are
		retval = APPROVE_TRADE;
	}
	else if(status_of_trade == DENY_TRADE)	//if the trade failed, this function should return an error code. see error_codes.txt?
		retval = DENY_TRADE;
	else
		retval = -49;
	return(retval);
}

int game::calculate_card_to_steal(int playernum)
{
	int numcards = 0;
	int card_to_steal = -1;
	int card = -1;
	int numwheat, numore, numwood, numsheep, numbrick;
	//all numbers should be between 
	numwheat = check_resources(playernum, WHEAT);
	numore = check_resources(playernum, ORE);
	numwood = check_resources(playernum, WOOD);
	numsheep = check_resources(playernum, SHEEP);
	numbrick = check_resources(playernum, BRICK);
	numcards = numwheat + numore + numwood + numsheep + numbrick;
	card_to_steal = (rand()*100) % numcards + 1;
	if (numcards > 0)
	{
		if ((numwheat > 0) && (card_to_steal <= numwheat))
			card = WHEAT;
		else if ((numore > 0) && (card_to_steal <= (numwheat + numore)))
			card = ORE;
		else if ((numwood > 0) && (card_to_steal <= (numwheat + numore + numwood)))
			card = WOOD;
		else if ((numsheep > 0) && (card_to_steal <= (numwheat + numore + numwood + numsheep)))
			card = SHEEP;
		else if ((numbrick > 0) && (card_to_steal > (numwheat + numore + numwood + numsheep)))
			card = BRICK;
		else
			card = -45;			//unknown card steal error, no card will be stolen.
	}
	else
		card = -44;		//error code to indicate that the player doesnt have enough cards to steal
	return(numcards);
}

//private steal function should be used by internal functions (need to add DV card play and add this to robber)
int game::steal_random_card(int playernum, int player_to_steal_from)
{
	int retval = 0;
	int card_to_steal = 0;
	//need to check how many cards the player to steal from has, generate a random # % #cards, and then take that card.	
	//retval = deduct_resources_trade(trade, playernum, requested_player);		//checks if resources are sufficient and then deducts them if they are
	card_to_steal = calculate_card_to_steal(playernum);
	if(card_to_steal > 0)	//if no error, steal card
		retval = deduct_resources_trade_low(player_to_steal_from, card_to_steal, -1);
/*	
	if (retval < 0)
	{
		cout << "oh no! fatal error! the player stolen from has negative cards. adding one card back." << endl;
		deduct_resources_trade_low(player_to_steal_from, card_to_steal, 1);
		return(retval);
	}
	else
		retval = deduct_resources_trade_low(playernum, card_to_steal, 1);	
*/
	return(retval);
}
/*
int game::deduct_resources_settlement(int playernum)
{
	int retval = 0;
	vector<player>::iterator player_ptr1;
	//it costs 1 wheat, 1 brick, 1 wood, and 1 sheep to build a settlement
	player_ptr1 = player_list.begin() + playernum;
	retval  = player_ptr1->update_resources(WHEAT,	-1);
	retval += player_ptr1->update_resources(WOOD,	-1);
	retval += player_ptr1->update_resources(SHEEP,	-1);
	retval += player_ptr1->update_resources(BRICK,	-1);
	return(retval);
}
*/
string game::get_board_info()
{
//data to send
	//players		-> # of players
	//longest road
	//largest army
	//current robber tile
	//pieces
		//send all tile data by tile number

	string data_out;
	int tempy = 0;
	int tempx = 0;
	char tempchar = (current_robber_tile + 1);
	data_out = players;
	data_out += (longest_road + 1);
	data_out += (largest_army + 1);
	data_out += tempchar;
//	data_out = players + longest_road + largest_army + tempchar;
	for (int x = 0; x < active_num_tiles; x++)
	{
		tempy = determine_y_index_from_tile(x);
		tempx = determine_x_index_from_tile(x);
		data_out += pieces[tempx][tempy].get_tile_data_string(x);
	}
	cout << "Size of board data is: " << data_out.length() << endl;
	return(data_out);
}
game::game(void)
{
	string tempstr = "";
	player tempplr(0, tempstr);
	player_list.push_back(tempplr);
//	Corner temps;
//	vector<int> tempvec;

		//	corner_index = xy;
/*	for(int x = 0; x <= max_x; x++)
	{
		for(int y = 0; y <= max_y; y++)
		{
			for(int xy = 0; xy < 6; xy++)
			{
				temps.corner_index = xy;
				temps.players_connected = tempvec;
				temps.property_owner = 0;
				temps.property_type = 0;
				temps.road_connected = 0;
			}
//			pieces[x][y].init_corners(temps);
		}
	}
	*/
}

game::~game(void)
{
}

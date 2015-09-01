#include <iostream>
#include <iomanip>
#include "player.h"
#include "Tile.h"
#include "game.h"
#include <ctime>

using namespace std;
/*
*to do:
**see about changing all return errors for functions that dont need to return a value to throws and catch at highest level?
**implement trading function.
**make build_road also check player resources
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
	return(current_player);
}

int game::check_current_player()
{
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


//this function needs to create the appropriate number of tiles and index them so that they can be easily manuvered around
int game::build_std_board(int size)
{
	int resource = 0;
	int dice = 0;
	srand(time(0));
	//must create tile, put it into pieces array, and update tile number in other array
	for(int x = 0; x < active_num_tiles; x++)
	{
		x_index[x] = const_valid_x_index_array[x];
		y_index[x] = const_valid_y_index_array[x];
		tile_number[x_index[x]][y_index[x]] = x;
//		temptile.
		resource = assign_resources();
		if(resource == DESERT)
			dice = 1;
		else
			dice = rolldice();
		pieces[x_index[x]][y_index[x]].set_resource_type(resource);
		pieces[x_index[x]][y_index[x]].set_dice_roll(dice);
	}
	return(1);
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
	if((xcoord1 < 0) || (ycoord1 < 0))
	{
		xcoord1 = tempx;
		ycoord1 = tempy;
		road1 = road_numb%6;
	}
	return(pieces[xcoord1][ycoord1].get_road_owner(road1));		//if 0, a road is not currently here.
		
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
	if((xcoord1 < 0) || (ycoord1 < 0))
	{
		xcoord1 = tempx;
		ycoord1 = tempy;
		corner1 = tempcorner;
	}
	if((xcoord2 < 0) || (ycoord2 < 0))
	{
		xcoord2 = tempx;
		ycoord2 = tempy;
		corner2 = tempcorner;
	}
	return(pieces[xcoord1][ycoord1].check_neighbors(corner1, playernum) && pieces[xcoord2][ycoord1].check_neighbors(corner2, playernum));
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

int game::check_resources_devcard(int playernum)
{
	player_ptr = player_list.begin() + playernum;
	return((player_ptr->check_resource_amount(WHEAT) >= 1) && (player_ptr->check_resource_amount(ORE) >= 1) && (player_ptr->check_resource_amount(SHEEP) >= 1));

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
	return(retval);
}

int game::deduct_resources_city(int playernum)
{
	int retval = 0;
	//it costs 2 wheat and 3 ore to upgrade to a city
	player_ptr = player_list.begin() + playernum;
	retval  = player_ptr->update_resources(WHEAT,	-2);
	retval += player_ptr->update_resources(ORE,	-3);
	return(retval);
}

int game::deduct_resources_road(int playernum)
{
	int retval = 0;
	//it costs 1 wood and 1 brick to build a road
	player_ptr = player_list.begin() + playernum;
	retval  = player_ptr->update_resources(WOOD,	-1);
	retval += player_ptr->update_resources(BRICK,	-1);
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

int game::upgrade_settlement(int tilenum, int playernum, int cornernum)
{
	int retval = -99;
	int xcoord = determine_x_index_from_tile(tilenum);
	int ycoord = determine_y_index_from_tile(tilenum);
	retval = check_corner_owner(cornernum, tilenum);
	if (retval == playernum)	//if the player owns the corner in question, proceed!
	{
		retval = check_corner_building_type(cornernum, tilenum);
		if (retval == 1)	//if there is currently a settlement, then player is allowed to upgrade.
		{
			if (check_resources_city(playernum))
			{
				retval = pieces[xcoord][ycoord].upgrade_settlement(cornernum, playernum);
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
	get_corners_from_road(road_numb, cornn1, cornn2);
	retval = determine_neighbor_tile_road(road_numb, tile_number, playernum, xcoord1, ycoord1, road1);

	//***A road touches 6 corners, not 2! (two of the main hex, two of the parallel hex, 1 of the hex on either end of road). 
	//must update all four, not just two!
	//if what im thinking is true, i should be able to build a settlement at 3.0 by building roads 0.4, 0.3, and 3.5
	if(retval == 0)		//if no road present on neighboring tile...
	{
		retval = pieces[tempx][tempy].build_road(cornn1, cornn2, playernum);
		if (retval > 0)
		{
			localptr = pieces[tempx][tempy].cornersz.begin() + cornn1;
			localptr->road_connected += 1;
			localptr->players_connected.push_back(playernum);		//update who is connected to each corner
			localptr = pieces[tempx][tempy].cornersz.begin() + cornn2;
			localptr->road_connected += 1;
			localptr->players_connected.push_back(playernum);
	
			//update the 2 other touching corners for the end of the road going to cornn1
			determine_if_neighbor_tile_occupied(cornn1, tile_number,playernum,xcoord2,ycoord2,corner1,xcoord3,ycoord3,corner2);	//use this to get the three corners to update on either side of the road
			if((xcoord2 != tempx) || (ycoord2 != tempy))
			{
				localptr = pieces[xcoord2][ycoord2].cornersz.begin() + corner1;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
			}
			if(((xcoord3 != tempx) || (ycoord3 != tempy)) && ((xcoord2 != xcoord3) || (ycoord2 != ycoord3)))
			{
				localptr = pieces[xcoord3][ycoord3].cornersz.begin() + corner2;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
			}

			//update the other 2 touching corners for the end of the road going to cornn2
			//only update if the touching corner is actually for a different tile...
			determine_if_neighbor_tile_occupied(cornn2, tile_number,playernum,xcoord2,ycoord2,corner1,xcoord3,ycoord3,corner2);	//use this to get the three corners to update on either side of the road
			if((xcoord2 != tempx) || (ycoord2 != tempy))
			{
				localptr = pieces[xcoord2][ycoord2].cornersz.begin() + corner1;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
			}
			if(((xcoord3 != tempx) || (ycoord3 != tempy)) && ((xcoord2 != xcoord3) || (ycoord2 != ycoord3)))
			{
				localptr = pieces[xcoord3][ycoord3].cornersz.begin() + corner2;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
			}
			//build road on parallel tile? (only needed if one exists!)
			if ((xcoord1 != tempx) || (ycoord1 != tempy))
			{
				get_corners_from_road(road1, cornn1, cornn2);
				retval1 = pieces[xcoord1][ycoord1].build_road(cornn1, cornn2, playernum);		//may need to go after the code below
				localptr = pieces[xcoord1][ycoord1].cornersz.begin() + cornn1;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);		//update who is connected to each corner
				localptr = pieces[xcoord1][ycoord1].cornersz.begin() + cornn2;
				localptr->road_connected += 1;
				localptr->players_connected.push_back(playernum);
				}
		}
		if ((retval > 0) || (retval1 > 0))
			deduct_resources_road(playernum);
	}
	else
		cerr << "I cant build a road for you." << endl;
	return(retval);
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
/*	if(pieces[xcoord][ycoord].check_build_settlement_tile(corner_numbz, playernum) && 
		determine_if_neighbor_tile_occupied(corner_numbz, tile_number,playernum,xcoord1,ycoord1,corner1,xcoord2,ycoord2,corner2)
		&& check_resources_settlement(playernum))
*/
	if((temp1 > 0) && (temp2 > 0) && (temp3 > 0))
	{
		pieces[xcoord][ycoord].build_settlement(corner_numbz, playernum);
		if ((xcoord1 != xcoord) || (ycoord1 != ycoord))
			pieces[xcoord1][ycoord1].build_settlement(corner1, playernum);
		if (((xcoord2 != xcoord) || (ycoord2 != ycoord)) && ((xcoord2 != xcoord1) || (ycoord2 != ycoord1)))
			pieces[xcoord2][ycoord2].build_settlement(corner2, playernum);
		temp = deduct_resources_settlement(playernum);
	}
	return(temp);
}
//0,3;0,4;1,4;3,0;4,0;4,1


int game::add_player(int player_num, string player_name)
{
	player_list.push_back(player(player_num, player_name));
	players += 1;
	return(players);
}
void game::trade_cards(int player1, int player2, vector<int> offer, vector<int> request)	//player1 offers x to player2 for request.
			//trade_cards function needs to check if each player has the request. it should also let player1 offer something and not
			//request something. if request is empty, then player2 can make an offer and then player1 can accept. dont allow free trades.
			//also should let player2 make counteroffer
{
	cout << "Trading is prohibited you dirty girl" << endl;
	throw("Trading has not been implemented yet");
}

int game::get_dice_roll(int tilenum)
{
	tilenum = tilenum % active_num_tiles;
	return(pieces[determine_x_index_from_tile(tilenum)][determine_y_index_from_tile(tilenum)].check_dice_roll());
}

//this function needs to roll the dice, compute the resources awarded to each player, give those resources to player, and handle the case of 7!
int game::start_turn(int rollnumvalam)
{
	int temppppp = 0;
//	int rollnumvalamt = 0;
	int retval = 0;
	int tempres = 0;
	int rollnumvalamt = (rand()*rand()) % 11 + 2;
	current_roll = rollnumvalamt;				//save current_roll value. this is the only place that this variable should ever be changed!
	player_ptr = player_list.begin();
	for(int i = 0; i < players; i++)
	{
		for(int x = 0; x < active_num_tiles; x++)
		{
			if(rollnumvalamt == get_dice_roll(x))
			{
				temppppp = pieces[const_valid_x_index_array[x]][const_valid_y_index_array[x]].calculate_resources_from_roll_by_player(i, tempres);
				tempres = pieces[const_valid_x_index_array[x]][const_valid_y_index_array[x]].check_tile_resource_type();
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
int game::start_game(int size, vector<string> player_names)
{//this needs to initialize the game. this function could also ahndle the initial placements, but for now ill have a higher level
	//function handle that (one that can actually communicate with the clients)
	int resource = 0;
	int dice = 0;
	int x = 1;
	int temp_num_players = 0;
	srand(time(0));
	//must create tile, put it into pieces array, and update tile number in other array
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
	add_player(0, "");		//make empty player so indexes are protected. player 0 is used as a null.
	//need to handle adding players here?
	x = 1;
	for (vector<string>::iterator tempptr = player_names.begin(); tempptr < player_names.end(); tempptr++)
	{
		temp_num_players = add_player(x, tempptr->c_str()) - 1;
		x++;
	}
	return(temp_num_players);
}

int game::deduct_resources(int player, int resource, int qty)
{
	player_ptr = player_list.begin() + player;
	if(qty > 0)		//if non-negative number, assume they meant to deduct the amount and multiply by neg1
		qty = qty*(-1);
	return(player_ptr.update_resources(resource, qty));
}

int game::deduct_resources_trade(trade_cards, int playernum, int req_player)
{
	int retval = 0;
	retval = check_resources_trade(trade_cards, playernum, req_player);
	if(retval >= 0)
	{
		retval = deduct_resources(playernum, WOOD, trade_cards.qty_wood_to_trade);
		retval = deduct_resources(playernum, ORE, trade_cards.qty_ore_to_trade);
		retval = deduct_resources(playernum, BRICK, trade_cards.qty_brick_to_trade);
		retval = deduct_resources(playernum, WHEAT, trade_cards.qty_wheat_to_trade);
		retval = deduct_resources(playernum, SHEEP, trade_cards.qty_sheep_to_trade);
		retval = deduct_resources(req_player, WOOD, trade_cards.qty_wood_to_receive);
		retval = deduct_resources(req_player, ORE, trade_cards.qty_ore_to_receive);
		retval = deduct_resources(req_player, BRICK, trade_cards.qty_brick_to_receive);
		retval = deduct_resources(req_player, WHEAT, trade_cards.qty_wheat_to_receive);
		retval = deduct_resources(req_player, SHEEP, trade_cards.qty_sheep_to_receive);
	}
	return(retval);
}
//-40 = current player doesnt have enough resources
//-41 = requested player doesnt have enough resources
//-42 = neither player has enough resources
int game::check_resources_trade(trade_cards trade, int playernum, int req_player)
{
	int tempcheck = 1;
	int tempcheckreq = 1;
	int retval = 1;
	vector<player>::iterator req_player_ptr;
	req_player_ptr = player_list.begin() + req_player;
	player_ptr = player_list.begin() + playernum;
	//for all checks below, if either player does not have enough cards to fulfill trade, then return error that tells which player doesnt have enough cards
	if(trade_cards.qty_wood_to_trade < player_ptr.check_resource_amount(WOOD))
		tempcheck = -40;
	if(trade_cards.qty_ore_to_trade < player_ptr.check_resource_amount(ORE))
		tempcheck = -40;		
	if(trade_cards.qty_brick_to_trade < player_ptr.check_resource_amount(BRICK))
		tempcheck = -40;
	if(trade_cards.qty_wheat_to_trade < player_ptr.check_resource_amount(WHEAT))
		tempcheck = -40;
	if(trade_cards.qty_sheep_to_trade < player_ptr.check_resource_amount(SHEEP))
		tempcheck = -40;
	if(trade_cards.qty_wood_to_receive < req_player_ptr.check_resource_amount(WOOD))
		tempcheckreq = -41;
	if(trade_cards.qty_ore_to_receive < req_player_ptr.check_resource_amount(ORE))
		tempcheckreq = -41;
	if(trade_cards.qty_brick_to_receive < req_player_ptr.check_resource_amount(BRICK))
		tempcheckreq = -41;
	if(trade_cards.qty_wheat_to_receive < req_player_ptr.check_resource_amount(WHEAT))
		tempcheckreq = -41;
	if(trade_cards.qty_sheep_to_receive < req_player_ptr.check_resource_amount(SHEEP))
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

unsigned int game::get_current_roll()
{
	return(current_roll);
}

//trade_cards is a struct defined in game.h
//player will contain the player number that started the trade request. if this number does not match the current player number, the trade is invalid
//if the requested player is different than player and current player, then continue
//this function should be called after checking with the requested player if they accept the trade, which means the calling function should call check_resources_trade before querying user.
int game::trade_with_player(trade_cards trade, int playernum, int requested_player, int status_of_trade)
{
	int retval = 0;
	if(status_of_trade == APPROVE_TRADE)
	{
		//the two lines below are done in the deduct_resources_trade functoin to ensure the resources cant be deducted maliciously
	//	retval = check_resources_trade(trade, playernum, requested_player);	
	//	if(retval >= 0)
		retval = deduct_resources_trade(trade, playernum, requested_player);		//checks if resources are sufficient and then deducts them if they are
	}
	else if(status_of_trade == REJECT_TRADE)	//if the trade failed, this function should return an error code. see error_codes.txt?
		retval = -43;
	else
		retval = -49;
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
	//pieces
		//send all tile data by tile number

	string data_out;
	int tempy = 0;
	int tempx = 0;
	data_out = players + longest_road + largest_army;
	for (int x = 0; x < active_num_tiles; x++)
	{
		tempy = determine_y_index_from_tile(x);
		tempx = determine_x_index_from_tile(x);
		data_out += pieces[tempx][tempy].get_tile_data_string();
	}
	return(data_out);
}
game::game(void)
{
	Corner temps;
	vector<int> tempvec;

		//	corner_index = xy;
	for(int x = 0; x <= max_x; x++)
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
			pieces[x][y].init_corners(temps);
		}
	}
}

game::~game(void)
{
}

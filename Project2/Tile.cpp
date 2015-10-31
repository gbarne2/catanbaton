#include "Tile.h"
#include <iomanip>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;


/*

Tile layout:
			B       C
			/̅ ̅ ̅ ̅ ̅ ̅ ̅ \
		 A /	     \ D
		   \		 / 
		    \_______/ 
			F		E

*/
tile::tile(void)
{
	A = 0;
	B = 0;
	C = 0;
	Corner temp;
	temp.road_connected = 0;
	temp.property_owner = 0;
	temp.property_type = 0;
	robber_on_tile = 0;
//	temp.players_connected.push_back(0);
	resource_type = 0;
	D = 0;
	E = 0;
	F = 0;
	roadAB = 0;
	roadBC = 0;
	roadCD = 0;
	roadDE = 0;
	roadEF = 0;
	roadFA = 0;
	for(int i = 0; i < 6; i++)
	{
		temp.corner_index = i;
		roads[i] = 0;
		cornersz.push_back(temp);
	}
	roll = 0;
}

tile::tile(int resource_type_init, int dice_roll_val)
{
	A = 0;
	B = 0;
	C = 0;
	Corner temp;
	temp.road_connected = 0;
	temp.property_owner = 0;
	temp.property_type = 0;
	//temp.players_connected.push_back(0);
	resource_type = resource_type_init;
	robber_on_tile = 0;
	D = 0;
	E = 0;
	F = 0;
	roadAB = 0;
	roadBC = 0;
	roadCD = 0;
	roadDE = 0;
	roadEF = 0;
	roadFA = 0;
	for(int i = 0; i < 6; i++)
	{
		temp.corner_index = i;
		roads[i] = 0;
		cornersz.push_back(temp);
	}
	roll = dice_roll_val;
}



tile::~tile(void)
{
}

/*
to do:
-need to make a function that just checks neighbors for the setup part of the game
*/ 
//this function needs to check its adjacent corners and determine if they are occupied (does it need to check who is occupying it?)
//if no neighbors, then return 1. if there are neighbors, return 0.
int tile::check_neighbors(int corner, int player)
{
	int temp = 0;
	cout << (corner-1)%6 << endl;
	vector<Corner>::iterator ptr1 = cornersz.begin() +  corner%6;
	vector<Corner>::iterator ptr2 = cornersz.begin() + (corner+1)%6;
	vector<Corner>::iterator ptr3;
	if((corner-1)%6 < 0) 
		ptr3 = cornersz.begin() + 5%6;
	else
		ptr3 = cornersz.begin() + (corner-1)%6;
/*
	for(int x = 0; x < corner%6; x++)
		ptr1++;
	for(int x = 0; x < (corner+1)%6; x++)
		ptr2++;
	for(int x = 0; x < (corner-1)%6; x++)
		ptr3++;
	ptr3 = ptr3 + ((corner-1)%6);
	*/
	return((ptr1->property_type == 0) && (ptr2->property_type == 0) && (ptr3->property_type == 0));
	//corner is the corner to check. there are 6 corners, so it needs to check corner+1 and corner-1, and also that there isnt already a building there
//	return((cornersz.data[(corner+1)%6] == 0) && (corners[(corner-1)%6] == 0) && (corners[corner%6] == 0));		//modulus operator wraps the array index around between 0 and 5, making it easy to index neighbors for corner A or F
}

//this should check for neighboring roads to see if it is legal to build a settlement. it is illegal to build a settlement on another players road.
//this function needs to check whether or not there is a road that the player can build on. if there isnt a road, or it isnt their road, then the settlement cannot be built.
int tile::check_roads_settlement(int corner, int player)	
{
	vector<int>::iterator temp_ptr;
	int status = 0;
	int temp = 0;
	int debugtemp = -1;
	ptr = cornersz.begin() + corner;
	//if the player has any roads from another tile that connect to either of these corners, then you can build a road if other conditions are met.
	for(temp_ptr = ptr->players_connected.begin(); temp_ptr < ptr->players_connected.end(); temp_ptr++)
	{
		debugtemp = *temp_ptr;
		if(*temp_ptr == player)
			status = 1;
		else if((*temp_ptr > 0) && (*temp_ptr == temp))			//if another player has 2 roads connected to this node, then the requesting player cannot build a settlement here!
		{
			status = 0;
			break;
		}
		else
			temp = *temp_ptr;									//assign current road owner to temp (if no owner, value stays 0...)
	}
	return((ptr->road_connected > 0) && status);
}

//build_settlement needs to check if it is legal for player to build a settlement here, based on roads and other neighboring structures. 
//if not legal, return 0, if legal, then return 1
///******This function is not enough to actually say its okay! the neighboring tiles also must be checked!
int tile::check_build_settlement_tile(int corner, int player)
{
	return((check_roads_settlement(corner, player) > 0) && (check_neighbors(corner, player) > 0));
}


//for now, this function assumes it is legal to build a settlement! the calling function must make sure its legal.
//the calling function also must update the neighboring tiles to show there is a building, and should probably do something to indicate to the user it was done!
//callig function must check to see if the player has enough resources as well
int tile::check_dice_roll()
{
	return(roll);
}

//this function will deleter the last player connected to a settlement/corner. this should only be used while doing the initial build phase when building settlements and roads together. if the road fails to build, call thiss
int tile::remove_settlement(int corner, int player)
{
	int retval = -39;
	vector<Corner>::iterator ptr3 = cornersz.begin() + (corner) % 6;
	if (player == ptr3->property_owner)
	{
		ptr3->property_type = 0;
		ptr3->property_owner = 0;
		ptr3->players_connected.pop_back();	//delete the last player connected.
		retval = 1;
	}
	return(retval);
}
void tile::build_settlement(int corner, int player)
{	
	vector<Corner>::iterator ptr3 = cornersz.begin() + (corner)%6;
	if(ptr3->property_type == 0)
	{
		ptr3->property_type = 1;
		ptr3->property_owner = player;
		ptr3->players_connected.push_back(player);
	}
}


int tile::upgrade_settlement(int corner, int player)
{
	ptr = cornersz.begin() + (corner%6);
	if((ptr->property_owner == player) && (ptr->property_type == 1))
	{
		ptr->property_type = 2;
		return(1);
	}
	return(0);
}
//This function needs to check if either one of the corners is occupied by the players building, and doesnt already have a road.
//if a road can be built, this returns a 1
int tile::check_roads(int corner1, int corner2, int player)
{
	vector<int>::iterator temp_ptr;
	int road_to_check = 0;
	int status = 0;
	int temp = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	corner1 = corner1%6;
	corner2 = corner2%6;
	if(((corner1) < (corner2)) && !((corner1 == 0) && (corner2 == 5)))		//if corner1 has higher index than corner 2, then road index = corner 1 (going A->B, B->C, etc.). if corner2 > corner1, then road index = corner2, since its going B->A, A->F, etc
		road_to_check = corner1;
	else
		road_to_check = corner1;
	ptr = cornersz.begin();
	//if the player has any roads from another tile that connect to either of these corners, then you can build a road if other conditions are met.
	for(temp_ptr = (ptr+corner1)->players_connected.begin(); temp_ptr < (ptr+corner1)->players_connected.end(); temp_ptr++)
		if(*temp_ptr == player)
			status = 1;
	for(temp_ptr = (ptr+corner2)->players_connected.begin(); temp_ptr < (ptr+corner2)->players_connected.end(); temp_ptr++)
		if(*temp_ptr == player)
			temp = 1;
	ptr = cornersz.begin() + corner1;
	if(ptr->property_owner == player)
		temp1 = 1;
	if(ptr->road_connected > 0)
		temp2 = 1;
	ptr = cornersz.begin() + corner2;
	if(ptr->property_owner == player)
		temp3 = 1;
	if(ptr->road_connected > 0)
		temp4 = 1;
	return((temp1 || (temp2 && status) || temp3 || (temp4 && temp)) && (roads[road_to_check] == 0));
//	return((roads[road_to_check] == 0)	//if the current road is empty, and	(player has property on either end of road OR has a road connected from another tile)
//		&& (((ptr+(corner1))->property_owner == player) || ((ptr+(corner2))->property_owner == player) || (((ptr+corner1)->road_connected > 0) && status) || 
//		(((ptr+corner2)->road_connected > 0) && temp)));
}


int tile::check_corner_owner(int corner)
{
	ptr = cornersz.begin() + corner%6;
	return(ptr->property_owner);
}

int tile::check_corner_building_type(int corner)
{
	ptr = cornersz.begin() + corner%6;
	return(ptr->property_type);
}

int tile::build_road(int corner1, int corner2, int player)
{
	int retval = -99;
	int road_index = 0;
	corner1 = corner1%6;
	corner2 = corner2%6;
	retval = check_roads(corner1, corner2, player);
	//if corners are adjacent, AB, BC, CD, DE, EF, FA, then its a valid road. if corner2 = corner1 + 1 OR corner2 == 5 AND corner1 = 0
	if (((corner2 == (corner1 + 1)) || (corner2 == (corner1 - 1)) || ((corner2 == 5) && (corner1 == 0)) || ((corner2 == 0) && (corner1 == 5))) && retval || (retval == player))	//if valid road
	{
		if ((corner2 - corner1) == 1)	//if AB, BC, CD, DE, EF, then index = corner1. otherwise, its AF and index = corner2 = 5
			road_index = corner1;
		else if ((corner1 == 5) && (corner2 == 0))
			road_index = 5;	//if user inputs 5-0 or 0-5, its road FA!
		else
			road_index = corner2;
		roads[road_index] = player;
		retval = road_index+1;
//		roads[road_index] = player;
		ptr = cornersz.begin() + corner1;
		ptr->road_connected += 1;
		ptr->players_connected.push_back(player);		//update who is connected to each corner
		ptr = cornersz.begin() + corner2;
		ptr->road_connected += 1;
		ptr->players_connected.push_back(player);
	}
	else
		retval = 0;
	return(retval);
}

int tile::get_road_owner(int roadnum)
{
	return(roads[roadnum%6]);
}

int tile::read_corner_owner(int corner)
{
	ptr = cornersz.begin();
	ptr += (corner%6);
	return(ptr->property_owner);
}


int tile::read_corner_type(int corner)
{
	ptr = cornersz.begin();
	ptr += (corner%6);
	return(ptr->property_type);
}

int tile::check_tile_resource_type()
{
	return(resource_type);
}

int tile::read_road(int road)
{
	return(roads[road%6]);
}


int tile::set_resource_type(int res_type)
{
	int retval = 1;
	if(resource_type == 0)
		resource_type = res_type;
	else
		retval = -7;
	return(retval);
}
	
int tile::set_dice_roll(int dicerollval)
{
	int retval = 1;
	if(roll == 0)
		roll = dicerollval;
	else
		retval = -2;
	return(retval);
}


//needs to sum up multiplier of # res player gets for this tile and return to user. should also pass resource type by reference to user.
//int tile::calculate_resources_from_roll_by_player(int rollval, int playernum, int& res_typee)
int tile::calculate_resources_from_roll_by_player(int playernum, int& res_typee)
{
	int retval = 0;
	if (!check_robber())		//if the robber is not on this tile, then players get resources
	{
		res_typee = resource_type;
		for (ptr = cornersz.begin(); ptr < cornersz.end(); ptr++)
		{
			if (ptr->property_owner == playernum)
				retval += ptr->property_type;		//this will be the number of cards the player gets. if for some reason they 'own' the corner but dont have a settlement, it will just add 0, so its safe!
		}
	}
	return(retval);
}

int tile::check_robber()
{
	return(robber_on_tile);
}

void tile::place_robber()
{
	robber_on_tile = 1;
}

void tile::remove_robber()
{
	robber_on_tile = 0;
}

string tile::get_tile_data_string(int tilenum)
{
	string temp_string = "";
	int count = 0;
	char temp[512];
	//need to send the following data 
	//roll
	//resource type
	//roads
	//cornersz data

	//Data format
	//"S"		-> beginning of each tile object.
	//datasize
	//tilenumber
	//resource type
	//roll
	//roads[0]		5
	//roads[1]		6
	//roads[2]		7
	//roads[3]		8
	//roads[4]		9
	//roads[5]		10
	//cornersz data		
		//corner_index
		//road_connected
		//property_owner
		//property_type
		//number of players connected
			//1st player connected
			//2nd player connected
			//.... 
			//(number of players connected) player connected
//	string data_outtemp = "";
	temp[0] = 'S';
//	temp[1] = 
	temp[2] = tilenum;
	temp[3] = resource_type;
	temp[4] = roll;
//	std::stringstream strStream;
//	strStream = std::stringstream();	//flush string stream...
//	strStream << tilenum << resource_type << roll;
//	data_outtemp += tilenum + resource_type + roll;
//	string data_out_final = "S";
	string data_out_final = "";
	int tempnum = 0;
	count = 3;
	int z = 11;
	for (int x = 0; x < 6; x++)
	{
		count++;
		temp[x + 5] = roads[x];
	}
		//		data_outtemp += roads[x];
	for (ptr = cornersz.begin(); ptr < cornersz.end(); ptr++)
	{
		temp[z++] = ptr->corner_index;
		temp[z++] = ptr->road_connected;
		temp[z++] = ptr->property_owner;
		temp[z++] = ptr->property_type;
		temp[z++] = ptr->players_connected.size();
		count += 5;
//		strStream << ptr->corner_index << ptr->road_connected << ptr->property_owner << ptr->property_type;
		//data_outtemp += ptr->corner_index + ptr->road_connected + ptr->property_owner
//			+ptr->property_type;
//		strStream << ptr->players_connected.size();
		cout << "players connected size in Tile.cpp: " << ptr->players_connected.size() << endl;
//		for (vector<int>::iterator tempptr = ptr->players_connected.begin(); tempptr < ptr->players_connected.end(); tempptr++)
		for (int y = 0; y < ptr->players_connected.size(); y++)
		{
//			strStream << ptr->players_connected.at(y);
			temp[z++] = ptr->players_connected.at(y);
			count++;
		}
//		strStream << count << temp_string;
//		data_outtemp += count;
//		data_outtemp += temp_string;
	}
//	tempnum = strStream.str().length();
//	data_out_final.push_back(tempnum);
//	data_out_final.append(strStream.str().c_str());
//	data_out_final += tempnum + strStream.str().c_str();
	temp[1] = count;
	for (int x = 0; x < count + 2; x++)
		data_out_final += temp[x];
	return(data_out_final);
}
/*
						coordinates are (x,y)
							    /¯¯¯¯¯¯¯\
							   /  (4,4)	 \
                      /¯¯¯¯¯¯¯\\		 //¯¯¯¯¯¯¯\
		             /  (3,4)  \\_______//	(4,3)  \
			/¯¯¯¯¯¯¯\\	       //¯¯¯¯¯¯¯\\		   //¯¯¯¯¯¯¯\
		   /  (2,4)	 \\_______//  (3,3)	 \\_______//  (4,2)	 \
		   \		 //¯¯¯¯¯¯¯\\		 //¯¯¯¯¯¯¯\\		 /
		    \_______//	(2,3)  \\_______//	(3,2)  \\_______/
			/¯¯¯¯¯¯¯\\		   //¯¯¯¯¯¯¯\\		   //¯¯¯¯¯¯¯\
		   /  (1,3)	 \\_______//  (2,2)	 \\_______//  (3,1)	 \
		   \		 //¯¯¯¯¯¯¯\\		 //¯¯¯¯¯¯¯\\		 /
		    \_______//	(1,2)  \\_______//	(2,1)  \\_______/
			/¯¯¯¯¯¯¯\\		   //¯¯¯¯¯¯¯\\		   //¯¯¯¯¯¯¯\
		   /  (0,2)	 \\_______//  (1,1)	 \\_______//  (2,0)	 \
		   \		 //¯¯¯¯¯¯¯\\		 //¯¯¯¯¯¯¯\\		 /
		    \_______//	(0,1)  \\_______//	(1,0)  \\_______/
					 \		   //¯¯¯¯¯¯¯\\		   /
					  \_______//  (0,0)	 \\_______/
							   \		 /
							    \_______/

*/

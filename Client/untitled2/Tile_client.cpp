#include "Tile_client.h"
#include <iomanip>
#include <iostream>
//#include "clientBaton.h"

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
tileclient::tileclient(void)
{
	A = 0;
	B = 0;
	C = 0;
	ClientCorner temp;
	temp.road_connected = 0;
	temp.property_owner = 0;
	temp.property_type = 0;
	robber_on_tile = 0;
	temp.players_connected.push_back(0);
	resource_type = 0;
	D = 0;
	E = 0;
	F = 0;
//	roadAB = 0;
//	roadBC = 0;
//	roadCD = 0;
//	roadDE = 0;
//	roadEF = 0;
//	roadFA = 0;
	for(int i = 0; i < 6; i++)
	{
		temp.corner_index = i;
		roads[i] = 0;
		Clientcornersz.push_back(temp);
	}
	roll = 0;
}

tileclient::tileclient(int resource_type_init, int dice_roll_val)
{
	A = 0;
	B = 0;
	C = 0;
	ClientCorner temp;
	temp.road_connected = 0;
	temp.property_owner = 0;
	temp.property_type = 0;
	temp.players_connected.push_back(0);
	resource_type = resource_type_init;
	robber_on_tile = 0;
	D = 0;
	E = 0;
	F = 0;
//	roadAB = 0;
//	roadBC = 0;
//	roadCD = 0;
//	roadDE = 0;
//	roadEF = 0;
//	roadFA = 0;
	for(int i = 0; i < 6; i++)
	{
		temp.corner_index = i;
		roads[i] = 0;
		Clientcornersz.push_back(temp);
	}
	roll = dice_roll_val;
}



tileclient::~tileclient(void)
{
}

/*
to do:
-need to make a function that just checks neighbors for the setup part of the game
*/ 
//this function needs to check its adjacent corners and determine if they are occupied (does it need to check who is occupying it?)
//if no neighbors, then return 1. if there are neighbors, return 0.
int tileclient::check_neighbors(int corner, int player)
{
	int temp = 0;
	cout << (corner-1)%6 << endl;
	vector<ClientCorner>::iterator ptr1 = Clientcornersz.begin() +  corner%6;
	vector<ClientCorner>::iterator ptr2 = Clientcornersz.begin() + (corner+1)%6;
	vector<ClientCorner>::iterator ptr3;
	if((corner-1)%6 < 0) 
		ptr3 = Clientcornersz.begin() + 5%6;
	else
		ptr3 = Clientcornersz.begin() + (corner-1)%6;
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
int tileclient::check_roads_settlement(int corner, int player)	
{
	vector<int>::iterator temp_ptr;
	int status = 0;
	int temp = 0;
	int debugtemp = -1;
	ptrr = Clientcornersz.begin() + corner;
	//if the player has any roads from another tile that connect to either of these corners, then you can build a road if other conditions are met.
	for(temp_ptr = ptrr->players_connected.begin(); temp_ptr < ptrr->players_connected.end(); temp_ptr++)
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
	return((ptrr->road_connected > 0) && status);
}

//build_settlement needs to check if it is legal for player to build a settlement here, based on roads and other neighboring structures. 
//if not legal, return 0, if legal, then return 1
///******This function is not enough to actually say its okay! the neighboring tiles also must be checked!
/*
int tileclient::check_build_settlement_tile(int corner, int player)
{
	return((check_roads_settlement(corner, player) > 0) && (check_neighbors(corner, player) > 0));
}
*/

//vector<ClientCorner> tileclient::update_board_info_from_server(char* datain, int datasize, int &start_index)
int tileclient::update_board_info_from_server(char* datain, int datasize, int& start_index)
{
	int tempnum = 0;
	int temp = 0;
	int index = start_index;
	int retval = 0;
	char tempchar[1] = { 0 };
	char tempchar1[1] = { 0 };
	char tempchar2[1] = { 0 };
	char tempchar3[1] = { 0 };
	char tempchar4[1] = { 0 };
	char tempchar5[1] = { 0 };
	if (datain[index++] == 'S')	//if start of a new corner, then process data.
	{

		//Data format
		//"S"		-> beginning of each tile object.
		//datasize
		//tilenumber
        //resource type
		//roll
		//roads[0]
		//roads[1]
		//roads[2]
		//roads[3]
		//roads[4]
		//roads[5]
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

		tempnum = datain[index++];
		index++;		//should skip past the tilenum.
//		tempchar[0] = datain[index++];
        resource_type = datain[index++];
        roll = datain[index++];
//		resource_type = atoi(tempchar);
//		tempchar[0] = datain[index++];
//		roll = (atoi(tempchar));// & 0x00FF);

		for (int x = 0; x < 6; x++)
		{
			tempchar[0] = datain[index++];
            roads[x] = tempchar[0];
		}

//		for (vector<ClientCorner>::iterator ctptr = Clientcornersz.begin(); ctptr < Clientcornersz.end(); ctptr++)
        vector<ClientCorner>::iterator ctptr;
        for(int z = 0; z < 6; z++)  //6 corners...
        {
            ctptr = Clientcornersz.begin() + z;
			tempchar1[0] = datain[index++];
			tempchar2[0] = datain[index++];
			tempchar3[0] = datain[index++];
			tempchar4[0] = datain[index++];
			tempchar5[0] = datain[index++];

            ctptr->corner_index = tempchar1[0];
            ctptr->road_connected = tempchar2[0];
            ctptr->property_owner = tempchar3[0];
            ctptr->property_type = tempchar4[0];
            temp = tempchar5[0];
            if(temp > 0) //if at least 1 new entry, clear the old data out.
                ctptr->players_connected.clear();	//clear out old data so it doesnt just keep getting bigger and bigger
			for (int x = 0; x < temp; x++)
			{
				tempchar[0] = datain[index++];
//				tempnum = tempchar[0];
                ctptr->players_connected.push_back(tempchar[0]);		//add # players connected to each corner
			}
		}
		retval = index - start_index;	//retval is how much to increment start_index by in calling function.
	}
	else
		retval = INVALID_BOARD_INFO_HEADER;
    start_index = index;
//    return(Clientcornersz);
    return(retval);
}

//for now, this function assumes it is legal to build a settlement! the calling function must make sure its legal.
//the calling function also must update the neighboring tiles to show there is a building, and should probably do something to indicate to the user it was done!
//callig function must check to see if the player has enough resources as well
int tileclient::check_dice_roll()
{
	return(roll);
}


/*
void tileclient::build_settlement(int corner, int player)
{	
	vector<Corner>::iterator ptr3 = cornersz.begin() + (corner)%6;
	if(ptr3->property_type == 0)
	{
		ptr3->property_type = 1;
		ptr3->property_owner = player;
		ptr3->players_connected.push_back(player);
	}
}
*/
/*
int tileclient::upgrade_settlement(int corner, int player)
{
	ptrr = cornersz.begin() + (corner%6);
	if((ptrr->property_owner == player) && (ptrr->property_type == 1))
	{
		ptrr->property_type = 2;
		return(1);
	}
	return(0);
}
*/

//This function needs to check if either one of the corners is occupied by the players building, and doesnt already have a road.
//if a road can be built, this returns a 1
int tileclient::check_roads(int corner1, int corner2, int player)
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
		road_to_check = corner2;
	ptrr = Clientcornersz.begin();
	//if the player has any roads from another tile that connect to either of these corners, then you can build a road if other conditions are met.
	for(temp_ptr = (ptrr+corner1)->players_connected.begin(); temp_ptr < (ptrr+corner1)->players_connected.end(); temp_ptr++)
		if(*temp_ptr == player)
			status = 1;
	for(temp_ptr = (ptrr+corner2)->players_connected.begin(); temp_ptr < (ptrr+corner2)->players_connected.end(); temp_ptr++)
		if(*temp_ptr == player)
			temp = 1;
	ptrr = Clientcornersz.begin() + corner1;
	if(ptrr->property_owner == player)
		temp1 = 1;
	if(ptrr->road_connected > 0)
		temp2 = 1;
	ptrr = Clientcornersz.begin() + corner2;
	if(ptrr->property_owner == player)
		temp3 = 1;
	if(ptrr->road_connected > 0)
		temp4 = 1;
	return((temp1 || (temp2 && status) || temp3 || (temp4 && temp)) && (roads[road_to_check] == 0));
//	return((roads[road_to_check] == 0)	//if the current road is empty, and	(player has property on either end of road OR has a road connected from another tile)
//		&& (((ptrr+(corner1))->property_owner == player) || ((ptrr+(corner2))->property_owner == player) || (((ptrr+corner1)->road_connected > 0) && status) || 
//		(((ptrr+corner2)->road_connected > 0) && temp)));
}


int tileclient::check_corner_owner(int corner)
{
	ptrr = Clientcornersz.begin() + corner%6;
	return(ptrr->property_owner);
}

int tileclient::check_corner_building_type(int corner)
{
	ptrr = Clientcornersz.begin() + corner%6;
	return(ptrr->property_type);
}

/*
int tileclient::build_road(int corner1, int corner2, int player)
{
	int retval = -99;
	int road_index = 0;
	corner1 = corner1%6;
	corner2 = corner2%6;
	retval = check_roads(corner1, corner2, player);
	//if corners are adjacent, AB, BC, CD, DE, EF, FA, then its a valid road. if corner2 = corner1 + 1 OR corner2 == 5 AND corner1 = 0
	if (((corner2 == (corner1 + 1)) || (corner2 == (corner1 - 1)) || ((corner2 == 5) && (corner1 == 0)) || ((corner2 == 0) && (corner1 == 5))) && retval)	//if valid road
	{
		if ((corner2 - corner1) == 1)	//if AB, BC, CD, DE, EF, then index = corner1. otherwise, its AF and index = corner2 = 5
			road_index = corner1;
		else if ((corner1 == 5) && (corner2 == 0))
			road_index = 5;	//if user inputs 5-0 or 0-5, its road FA!
		else
			road_index = corner2;
		roads[road_index] = player;
		retval = road_index+1;
	}
	else
		retval = 0;

//		roads[road_index] = player;
//		ptrr = cornersz.begin() + corner1;
//		ptrr->road_connected += 1;
//		ptrr->players_connected.push_back(player);		//update who is connected to each corner
//		ptrr = cornersz.begin() + corner2;
//		ptrr->road_connected += 1;
//		ptrr->players_connected.push_back(player);

	return(retval);
}
*/

int tileclient::get_road_owner(int roadnum)
{
	return(roads[roadnum%6]);
}

int tileclient::read_corner_owner(int corner)
{
	ptrr = Clientcornersz.begin();
	ptrr += (corner%6);
	return(ptrr->property_owner);
}


int tileclient::read_corner_type(int corner)
{
	ptrr = Clientcornersz.begin();
	ptrr += (corner%6);
	return(ptrr->property_type);
}

int tileclient::check_tile_resource_type()
{
	return(resource_type);
}

int tileclient::read_road(int road)
{
	return(roads[road%6]);
}

int tileclient::set_resource_type(int res_type)
{
	int retval = 1;
	if(resource_type == 0)
		resource_type = res_type;
	else
		retval = -7;
	return(retval);
}

int tileclient::set_dice_roll(int dicerollval)
{
	int retval = 1;
	if((roll == 0) && (resource_type != DESERT))
		roll = dicerollval;
	else
		retval = -2;
	return(retval);
}

//needs to sum up multiplier of # res player gets for this tile and return to user. should also pass resource type by reference to user.
//int tileclient::calculate_resources_from_roll_by_player(int rollval, int playernum, int& res_typee)
int tileclient::calculate_resources_from_roll_by_player(int playernum, int& res_typee)
{
	int retval = 0;
	if (!check_robber())		//if the robber is not on this tile, then players get resources
	{
		res_typee = resource_type;
		for (ptrr = Clientcornersz.begin(); ptrr < Clientcornersz.end(); ptrr++)
		{
			if (ptrr->property_owner == playernum)
				retval += ptrr->property_type;		//this will be the number of cards the player gets. if for some reason they 'own' the corner but dont have a settlement, it will just add 0, so its safe!
		}
	}
	return(retval);
}

int tileclient::check_robber()
{
	return(robber_on_tile);
}

/*
void tileclient::place_robber()
{
	robber_on_tile = 1;
}
*/

/*
void tileclient::remove_robber()
{
	robber_on_tile = 0;
}
*/

/*
string tileclient::get_tile_data_string()
{
	string temp_string;
	int count = 0;
	//need to send the following data 
	//roll
	//resource type
	//roads
	//cornersz data

	//Data format
	//"S"		-> beginning of each tile object.
	//resource type
	//roll
	//roads[0]
	//roads[1]
	//roads[2]
	//roads[3]
	//roads[4]
	//roads[5]
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
	string data_out = "S" + resource_type + roll;
	for (int x = 0; x < 6; x++)
		data_out += roads[x];
	for (ptrr = cornersz.begin(); ptrr < cornersz.end(); ptrr++)
	{
		data_out += ptrr->corner_index + ptrr->road_connected + ptrr->property_owner + ptrr->property_type;
		count = 0;
		for (vector<int>::iterator tempptr = ptrr->players_connected.begin(); tempptr < ptrr->players_connected.end(); tempptr++)
		{
			temp_string += *tempptr;
			count++;
		}
		data_out += count;
		data_out += temp_string;
	}
	return(data_out);
}
*/

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

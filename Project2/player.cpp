#include "player.h"
#include "Tile.h"
#include <string>

using namespace std;

player::player(void)
{
	qty_brick = START_BRICK;		//start off with enough resources to build two roads and two settlements
	qty_ore = START_ORE;
	qty_sheep = START_SHEEP;		//start off with enough resources to build two roads and two settlements
	qty_wheat = START_WHEAT;		//start off with enough resources to build two roads and two settlements
	qty_wood = START_WOOD;		//start off with enough resources to build two roads and two settlements
	roads_to_place = START_ROADS;
	settlements_to_build = START_SETTLEMENTS;
	cities_to_build = START_CITIES;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
	ClientSocket = INVALID_SOCKET;
}

player::player(int IDset, string p_name)
{
	player_ID = IDset;
	Player_name = p_name;
	qty_brick = START_BRICK;		//start off with enough resources to build two roads and two settlements
	qty_ore = START_ORE;
	qty_sheep = START_SHEEP;		//start off with enough resources to build two roads and two settlements
	qty_wheat = START_WHEAT;		//start off with enough resources to build two roads and two settlements
	qty_wood = START_WOOD;		//start off with enough resources to build two roads and two settlements
	roads_to_place = START_ROADS;
	settlements_to_build = START_SETTLEMENTS;
	cities_to_build = START_CITIES;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
	ClientSocket = INVALID_SOCKET;
}

player::player(int IDset, string p_name, SOCKET tempsock)
{
	player_ID = IDset;
	Player_name = p_name;
	qty_brick = START_BRICK;		//start off with enough resources to build two roads and two settlements
	qty_ore = START_ORE;
	qty_sheep = START_SHEEP;		//start off with enough resources to build two roads and two settlements
	qty_wheat = START_WHEAT;		//start off with enough resources to build two roads and two settlements
	qty_wood = START_WOOD;		//start off with enough resources to build two roads and two settlements
	roads_to_place = START_ROADS;
	settlements_to_build = START_SETTLEMENTS;
	cities_to_build = START_CITIES;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
	ClientSocket = tempsock;
}

player::~player(void)
{
}

void player::set_client_address(sockaddr Client_addr, SOCKET clientsock)
{
	client_address = Client_addr;
}

int player::roads_left()
{
	return(roads_to_place);
}

int player::settlements_left()
{
	return(settlements_to_build);
}

int player::cities_left()
{
	return(cities_to_build);
}

int player::update_resources(int type, int amount)
{
	switch(type)
	{
	case WHEAT:	if((amount < 0) && (qty_wheat < abs(amount)))
					type = -21;
				else
					qty_wheat	+= amount;
				break;
	case ORE:	if((amount < 0) && (qty_ore < abs(amount)))
					type = -22;
				else
					qty_ore		+= amount;
				break;
	case WOOD:	if((amount < 0) && (qty_wood < abs(amount)))
					type = -23;
				else
					qty_wood	+= amount;
				break;
	case SHEEP:	if((amount < 0) && (qty_sheep < abs(amount)))
					type = -24;
				else
					qty_sheep	+= amount;
				break;
	case BRICK:	if((amount < 0) && (qty_brick < abs(amount)))
					type = -25;
				else
					qty_brick	+= amount;
				break;
	default:	type = -20;
				break;
	}
	return(type);
}
	
int player::check_resource_amount(int type)
{
	int temp;
	switch(type)
	{
	case WHEAT:	temp = qty_wheat;
		break;
	case ORE:	temp = qty_ore;
		break;
	case WOOD:	temp = qty_wood;
		break;
	case SHEEP:	temp = qty_sheep;
		break;
	case BRICK:	temp = qty_brick;
		break;
	default:	temp = -1;
		break;
	}
	cout << "resource type: " << type << endl << "Amount available: " << temp << endl;
	return(temp);
}

int player::update_roads(int roadnum)
{
//	if (roadnum > 0)	//if adding a road, increase roads left by one, if less than 1 deduct one
//		roads_to_place +=- 1;
//	else if (roadnum < 0)
	//cant add roads
	if (roadnum < 0)
	roads_to_place -= 1;
	return(roads_to_place);
}

int player::update_settlements(int settlementnum)
{
	if (settlementnum > 0)
		settlements_to_build += 1;
	else if (settlementnum < 0)
		settlements_to_build -= 1;
	return(settlements_to_build);
}

int player::update_cities(int citiesnum)
{
	//cant add cities
//	if (citiesnum > 0)
//		cities_to_build += 1;
//	else if (citiesnum < 0)
	if (citiesnum < 0)
			cities_to_build -= 1;
	return(cities_to_build);
}
/*
int qty_knights;			//1
int qty_victory_points;		//2
int qty_year_of_plenty;		//3
int qty_monopoly;			//4
int qty_build_roads;		//5
*/
int player::check_qty_devcard(int card)
{
	int retval = 0;
	switch (card)
	{
	case 1:	retval = DC.qty_knights;
		break;
	case 2: retval = DC.qty_victory_points;
		break;
	case 3: retval = DC.qty_year_of_plenty;
		break;
	case 4: retval = DC.qty_monopoly;
		break;
	case 5:	retval = DC.qty_build_roads;
		break;
	default: retval = -50;
		break;
	}
	return(retval);
}

//tell it what card to buy and it will return either the number of that card you now have OR an error code of -50, indicating the input was invalid.
int player::purchase_dev_card(int card)
{
	int retval = 0;
	switch (card)
	{
	case 1: DC.qty_knights += 1;
		retval = DC.qty_knights;
		break;
	case 2: DC.qty_victory_points += 1;
		retval = DC.qty_victory_points;
		break;
	case 3: DC.qty_year_of_plenty += 1;
		retval = DC.qty_year_of_plenty;
		break;
	case 4: DC.qty_monopoly += 1;
		retval = DC.qty_monopoly;
		break;
	case 5: DC.qty_build_roads += 1;
		retval = DC.qty_build_roads;
		break;
	default:
		retval = -50;
		break;
	}
	return(retval);
}

int player::use_dev_card(int card)
{
	int retval = 0;
	switch (card)
	{
	case 1: if (DC.qty_knights > 0) {
		DC.qty_knights -= 1;
		retval = DC.qty_knights;
	}
			else
				retval = -51;
		break;
	case 2: //dont reduce this number! it is used to calculate victory points!
		retval = DC.qty_victory_points;
		break;
	case 3: if (DC.qty_year_of_plenty > 0) {
		DC.qty_year_of_plenty -= 1;
		retval = DC.qty_year_of_plenty;
	}
			else
				retval = -51;
		break;
	case 4: if (DC.qty_monopoly > 0) {
		DC.qty_monopoly -= 1;
		retval = DC.qty_monopoly;
	}
			else
				retval = -51;
		break;
	case 5: if (DC.qty_build_roads > 0) {
		DC.qty_build_roads -= 1;
		retval = DC.qty_build_roads;
	}
			else
				retval = -51;
		break;
	default:
		retval = -50;
		break;
	}
	return(retval);

}

//int input is to be used to determine victory points to see if there is a winner or to display to other users. if displaying to other users it shouldnt include DV victory points
int player::calculate_victory_points(int for_win)
{
	int vp = 0;
	vp = (START_SETTLEMENTS - settlements_to_build) + (START_CITIES - cities_to_build) * 2;		//need to deal with longest road and largest army at a higher level. no support for those at this level...
	if (for_win == 1)	//if for victory, include the DV victory points
		vp += DC.qty_victory_points;
	return(vp);
}

int player::get_victory_points(int for_win)
{
	return(calculate_victory_points(for_win));
}

SOCKET player::get_client_socket(void)
{
	return(ClientSocket);
}


void player::add_RX_bytes(int bytes)
{
	if (bytes >= 0)
	{
		numRXbytes += bytes;
	}
}

void player::add_TX_bytes(int bytes)
{
	if (bytes >= 0)
		numTXbytes += bytes;
}

long int player::check_num_rx_bytes()
{
	return(numRXbytes);
}

long int player::check_num_tx_bytes()
{
	return(numTXbytes);
}

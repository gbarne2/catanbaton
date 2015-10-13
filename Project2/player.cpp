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
	roads_to_place = 15;
	settlements_to_build = 5;
	cities_to_build = 4;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
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
	roads_to_place = 15;
	settlements_to_build = 5;
	cities_to_build = 4;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
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
	roads_to_place = 15;
	settlements_to_build = 5;
	cities_to_build = 4;
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

SOCKET player::get_client_socket(void)
{
	return(ClientSocket);
}
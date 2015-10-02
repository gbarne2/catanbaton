#include "playerClient.h"
#include "tcpclient.h"
#include "Tile.h"
#include <string>

using namespace std;

playerClient::playerClient(void)
{
	qty_brick = 4;		//start off with enough resources to build two roads and two settlements
	qty_ore = 0;
	qty_sheep = 2;		//start off with enough resources to build two roads and two settlements
	qty_wheat = 2;		//start off with enough resources to build two roads and two settlements
	qty_wood = 4;		//start off with enough resources to build two roads and two settlements
	roads_to_place = 15;
	settlements_to_build = 5;
	cities_to_build = 4;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
}

playerClient::playerClient(string p_name)
{
	player_ID = -72;
	Player_name = p_name;
	qty_brick = 4;		//start off with enough resources to build two roads and two settlements
	qty_ore = 0;
	qty_sheep = 2;		//start off with enough resources to build two roads and two settlements
	qty_wheat = 2;		//start off with enough resources to build two roads and two settlements
	qty_wood = 4;		//start off with enough resources to build two roads and two settlements
	roads_to_place = 15;
	settlements_to_build = 5;
	cities_to_build = 4;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
}

playerClient::~playerClient(void)
{
}

void playerClient::set_server_address(sockaddr Server_addr, SOCKET clientsock)
{
	server_address = Server_addr;
}

int playerClient::roads_left()
{
	return(roads_to_place);
}

int playerClient::settlements_left()
{
	return(settlements_to_build);
}

int playerClient::cities_left()
{
	return(cities_to_build);
}

int playerClient::update_resources(int type, int amount)
{
	switch(type)
	{
	case WHEAT:	if((amount < 0) && (qty_wheat < abs(amount)))
					type = -21;
				else
					qty_wheat	= amount;
				break;
	case ORE:	if((amount < 0) && (qty_ore < abs(amount)))
					type = -22;
				else
					qty_ore		= amount;
				break;
	case WOOD:	if((amount < 0) && (qty_wood < abs(amount)))
					type = -23;
				else
					qty_wood	= amount;
				break;
	case SHEEP:	if((amount < 0) && (qty_sheep < abs(amount)))
					type = -24;
				else
					qty_sheep	= amount;
				break;
	case BRICK:	if((amount < 0) && (qty_brick < abs(amount)))
					type = -25;
				else
					qty_brick	= amount;
				break;
	default:	type = -20;
				break;
	}
	return(type);
}
	
int playerClient::check_resource_amount(int type)
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

int playerClient::update_roads(int roadnum)
{
//	if (roadnum > 0)	//if adding a road, increase roads left by one, if less than 1 deduct one
//		roads_to_place +=- 1;
//	else if (roadnum < 0)
	//cant add roads
//	if (roadnum < 0)
	roads_to_place = roadnum;
	return(roads_to_place);
}

int playerClient::update_settlements(int settlementnum)
{
	settlements_to_build = settlementnum;
	/*
	if (settlementnum > 0)
		settlements_to_build += 1;
	else if (settlementnum < 0)
		settlements_to_build -= 1;
		*/
	return(settlements_to_build);
}

int playerClient::update_cities(int citiesnum)
{
	cities_to_build = citiesnum;
	//cant add cities
//	if (citiesnum > 0)
//		cities_to_build += 1;
//	else if (citiesnum < 0)
	/*
	if (citiesnum < 0)
			cities_to_build -= 1;
	*/
	return(cities_to_build);
}

SOCKET playerClient::get_server_socket(void)
{
	return(ServerSocket);
}

void playerClient::set_player_num(int num)
{
	if ((player_ID == -72) && (num > 0))
		player_ID = num;
	return;
}

int playerClient::get_player_num()
{
	return(player_ID);
}
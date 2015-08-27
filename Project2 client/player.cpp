#include "player.h"
#include "Tile.h"
#include <string>

using namespace std;

player::player(void)
{
	qty_brick = 0;
	qty_ore = 0;
	qty_sheep = 0;
	qty_wheat = 0;
	qty_wood = 0;
	roads_to_place = 15;
	settlements_to_build = 4;
	cities_to_build = 8;
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
	qty_brick = 0;
	qty_ore = 0;
	qty_sheep = 0;
	qty_wheat = 0;
	qty_wood = 0;
	roads_to_place = 15;
	settlements_to_build = 4;
	cities_to_build = 8;
	DC.qty_build_roads = 0;
	DC.qty_knights = 0;
	DC.qty_monopoly = 0;
	DC.qty_victory_points = 0;
	DC.qty_year_of_plenty = 0;
}

player::~player(void)
{
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

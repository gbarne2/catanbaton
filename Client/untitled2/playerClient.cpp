#include "playerClient.h"
#include "tcpclient.h"
#include "Tile_client.h"
#include <string>

using namespace std;

playerClient::playerClient(void)
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
    player_ID = -72;
    Player_name = "";
}

playerClient::playerClient(string p_name)
{
    player_ID = -72;
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

playerClient::~playerClient(void)
{
}

void playerClient::set_server_address(sockaddr Server_addr, SOCKET clientsock)
{
    if(ServerSocket == INVALID_SOCKET)
    {
        server_address = Server_addr;
        ServerSocket = clientsock;
    }
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
//	cout << "resource type: " << type << endl << "Amount available: " << temp << endl;
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

void playerClient::update_docks(int docktype)
{
    int flag = 1;
    if((docktype <= 0) || (docktype > 7))   //if invalid dock type, dont add it!
        flag = 0;
    else
    {
        for(int x = 0; x < docks.size(); x++)
        {
            if(docks.at(x) == docktype)
                flag = 0;
        }
    }
    if(flag)
        docks.push_back(docktype);
    return;
}

vector<int> playerClient::check_docks()
{
    return(docks);
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

void playerClient::set_player_name(std::string name)
{
    Player_name = name;
}

std::string playerClient::get_player_name()
{
    return(Player_name);
}

int playerClient::update_dev_cards(int devcard, int qty)
{
    int retval = qty;
    switch(devcard)
    {
    case 1: DC.qty_knights = qty;
        break;
    case 2: DC.qty_victory_points = qty;
        break;
    case 3: DC.qty_year_of_plenty = qty;
        break;
    case 4: DC.qty_monopoly = qty;
        break;
    case 5: DC.qty_build_roads = qty;
        break;
    default:
        retval = -51;
        break;
    }
    return(retval);
}

int playerClient::get_qty_dev_card(int card)
{
    int retval = 0;
    switch(card)
    {
    case 1: retval = DC.qty_knights;
        break;
    case 2: retval = DC.qty_victory_points;
        break;
    case 3: retval = DC.qty_year_of_plenty;
        break;
    case 4: retval = DC.qty_monopoly;
        break;
    case 5: retval = DC.qty_build_roads;
        break;
    default:
        retval = -51;
        break;
    }
    return(retval);
}

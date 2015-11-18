#pragma once
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#ifndef START_CARD_VALUES
#define START_CARD_VALUES
#define START_ORE		7
#define START_SHEEP		7
#define START_BRICK		7
#define START_WHEAT		7
#define START_WOOD		7
#endif


#ifndef dockvalues
#define dockvalues		0
#define WHEATDOCK		1
#define OREDOCK			2
#define WOODDOCK		3
#define SHEEPDOCK		4
#define BRICKDOCK		5
#define THREETOONEDOCK	6
#define NO_DOCK			7
#endif

using namespace std;

/*
struct trade_cards
{
    unsigned int qty_wood_to_trade;
    unsigned int qty_wood_to_receive;
    unsigned int qty_ore_to_trade;
    unsigned int qty_ore_to_receive;
    unsigned int qty_brick_to_trade;
    unsigned int qty_brick_to_receive;
    unsigned int qty_wheat_to_trade;
    unsigned int qty_wheat_to_receive;
    unsigned int qty_sheep_to_trade;
    unsigned int qty_sheep_to_receive;
};
*/
struct player_dev_cards
{
    int qty_knights;
    int qty_victory_points;
    int qty_year_of_plenty;
    int qty_monopoly;
    int qty_build_roads;
};

class playerClient
{
    sockaddr server_address;
    SOCKET ServerSocket = INVALID_SOCKET;
    int player_ID;
    std::string Player_name;
    int qty_wood;
    int qty_ore;
    int qty_brick;
    int qty_wheat;
    int qty_sheep;
    int roads_to_place;
    int settlements_to_build;
    int cities_to_build;
    player_dev_cards DC;
    vector<int> docks;

public:
    playerClient(void);
    playerClient(std::string);
    void set_server_address(sockaddr, SOCKET);
    SOCKET get_server_socket(void);
    int update_resources(int type, int amount);
    int check_resource_amount(int type);
    int roads_left(void);
    int settlements_left(void);
    void set_player_name(std::string);
    std::string get_player_name();
    int cities_left(void);
    int update_roads(int);
    int update_settlements(int);
    int update_cities(int);
    void update_docks(int);
    vector<int> check_docks();
    void set_player_num(int);		//only updates 1 time!
    int get_player_num();
    ~playerClient(void);
    int update_dev_cards(int devcard, int qty);
    int get_qty_dev_card(int card);
};


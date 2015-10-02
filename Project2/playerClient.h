#pragma once
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

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

public:
	playerClient(void);
	playerClient(std::string);
	void set_server_address(sockaddr, SOCKET);
	SOCKET get_server_socket(void);
	int update_resources(int type, int amount);
	int check_resource_amount(int type);
	int roads_left(void);
	int settlements_left(void);
	int cities_left(void);
	int update_roads(int);
	int update_settlements(int);
	int update_cities(int);
	void set_player_num(int);		//only updates 1 time!
	int get_player_num();
	~playerClient(void);
};


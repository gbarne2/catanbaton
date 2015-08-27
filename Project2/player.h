#pragma once
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

struct dev_cards
{
	int qty_knights;
	int qty_victory_points;
	int qty_year_of_plenty;
	int qty_monopoly;
	int qty_build_roads;
};

class player
{
	sockaddr client_address;
	SOCKET ClientSocket = INVALID_SOCKET;
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
	dev_cards DC;

public:
	player(void);
	player(int, std::string);
	int set_client_address(sockaddr, SOCKET);
	SOCKET get_client_socket(void);
	int update_resources(int type, int amount);
	int check_resource_amount(int type);
	int roads_left(void);
	int settlements_left(void);
	int cities_left(void);
	~player(void);
};


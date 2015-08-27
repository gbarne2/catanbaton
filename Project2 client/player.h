#pragma once
#include <string>
#include <iostream>

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
	int client_addr_length;
	int iSendResult;
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
	int update_resources(int type, int amount);
	int check_resource_amount(int type);
	~player(void);
};


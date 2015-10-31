#pragma once
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#ifndef START_CARD_VALUES
#define START_CARD_VALUES
#define START_ORE			50
#define START_SHEEP			50
#define START_BRICK			50
#define START_WHEAT			50
#define START_WOOD			50
#define START_SETTLEMENTS	50
#define START_ROADS			150
#define START_CITIES		4
#endif

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
struct dev_cards
{
	int qty_knights;			//1
	int qty_victory_points;		//2
	int qty_year_of_plenty;		//3
	int qty_monopoly;			//4
	int qty_build_roads;		//5
};

class player
{
	sockaddr client_address;
	SOCKET ClientSocket;
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
	int calculate_victory_points(int);
	long int numRXbytes;
	long int numTXbytes;

public:
	player(void);
	player(int, std::string);
	player(int, std::string, SOCKET);
	void set_client_address(sockaddr, SOCKET);
	SOCKET get_client_socket(void);
	int update_resources(int type, int amount);
	int check_resource_amount(int type);
	int roads_left(void);
	int settlements_left(void);
	int cities_left(void);
	int update_roads(int);
	int update_settlements(int);
	int update_cities(int);
	int check_qty_devcard(int);
	int purchase_dev_card(int);
	int use_dev_card(int);		//if retval >= 0, then it was deducted/valid. if -52, not enough of selected card. if -51, invalid card.
	int get_victory_points(int);
	~player(void);
	void add_RX_bytes(int);
	void add_TX_bytes(int);
	long int check_num_rx_bytes();
	long int check_num_tx_bytes();
};


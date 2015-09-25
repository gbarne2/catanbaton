#include "clientTransmitHandler.h"
#include <iostream>
#include <iomanip>
#include <new>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "game.h"
#include <string>
#include "tcpclient.h"

using namespace std;
#define playernum1	1
int packetAssembler(char& buffer, int size)
{
	cout << "make packetAssembler format all packets to be sent how they should be!" << endl;
	cout << "Data to send: ";
	char *temp = new char[size + 5];
	temp[0] = 'S';
	temp[1] = 8;
	temp[2] = 53;
	temp[3] = 'p';
	temp[4] = playernum1;
	temp[5] = size;
	strcat(temp, buffer);
	for (int x = 0; x < size + 6; x++)
		cout << temp[x];
	cout << endl << endl;
	//use playerClient class to get server address and socket (server_address and ServerSocket)
	return 0;
}

int sendPacketTX(int player, char* data, int length, int packet_type)
{
	char temp[4096];
	temp[0] = packet_type;
	strcat(temp, data, length);
	return(packetAssembler(*temp, length + 1));
}

int sendPacketTX(int player, string data, int packet_type)
{
	int retval = 0;
	char tempchar[1] = packet_type;
	char *temp = new char[data.length() + 1];
	strcpy(temp, tempchar);
	strcat(temp, data.c_str());
	retval = packetAssembler(*temp, temp.length());
	delete[] temp;
	return(retval);
}

int sendPacketTX(int player, int data, int packet_type)
{
	string datastr;
	ostringstream convert;
	int retval = 0;
	convert << packet_type << data;
	datastr = convert.str();
	char *temp = new char[datastr.length()];
	strcpy(temp, datastr.c_str());
	retval = packetAssembler(*temp, datastr.length());
	delete[] temp;
	return(retval);
}

int tx_proposeTrade(int player_to_trade, trade_cards_offer trade)
{
	string data = "";
	data += player_to_trade;
	data += trade_cards_offer.qty_wood_to_trade;
	data += trade_cards_offer.qty_wood_to_receive;
	data += trade_cards_offer.qty_ore_to_trade;
	data += trade_cards_offer.qty_ore_to_receive;
	data += trade_cards_offer.qty_brick_to_trade;
	data += trade_cards_offer.qty_brick_to_receive;
	data += trade_cards_offer.qty_wheat_to_trade;
	data += trade_cards_offer.qty_wheat_to_receive;
	data += trade_cards_offer.qty_sheep_to_trade;
	data += trade_cards_offer.qty_sheep_to_receive;
	return(sendPacketTX(player_to_trade, data, PROPOSE_TRADE));
}

int tx_accept_or_reject_trade(int player_to_trade, int status)
{
	string data = "";
	data += player_to_trade;
	data += status;
	data += "9999999999";
	return(sendPacketTX(player_to_trade, data, ACCEPT_REJECT_TRADE));
}

int tx_start_turn()
{
	string data = "";
	data += START_TURN;
	return(packetAssembler(data.c_str(), data.length()));
}

int tx_end_turn()
{
	return(sendPacketTX(0, 1, END_TURN));
}

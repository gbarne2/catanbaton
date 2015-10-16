#include "clientTransmitHandler.h"
#include <iostream>
#include <iomanip>
#include <new>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <new>
#include <string>
#include "tcpclient.h"
#include "gameClient.h"
#include "mainwindow.h"

using namespace std;
#define playernum1	1
int check_rx_data_buff = 0;
extern char rxdatabuff [4096];

int packetAssembler(char buffer[], int size)
{
    int retval = 0;
	cout << "make packetAssembler format all packets to be sent how they should be!" << endl;
	cout << "Data to send: ";
//	char tempbuf[1];
//	itoa(size, tempbuf, 10);
	char temp[4096];
    int tempsize = ((size & 0x07F80) >> 7);
	temp[0] = 'S';
	temp[1] = '8';
	temp[2] = 53;
	temp[3] = 'p';
	temp[4] = tempsize;
    temp[5] = (size & 0x07F);
	for (int x = 0; x < size; x++)
		temp[x + 6] = buffer[x];
//	strcat(temp, buffer);
	for (int x = 0; x < size + 6; x++)
	{
        cout << +temp[x] << " ";
		rxdatabuff[x] = temp[x];
	}
    retval = clienttcp.sendThenReceive(rxdatabuff, size+6);
//    ZeroMemory(rxdatabuff, )
    for(int x = 0; x < DEFAULT_BUFLEN; x++)
        rxdatabuff[x] = 0;
    for(int x = 0; x < clienttcp.get_rxbuffsize(); x++)
        rxdatabuff[x] = clienttcp.read_receive_buff(x);
    check_rx_data_buff = 1;
	cout << endl << endl;
//	delete[] temp;
	//use playerClient class to get server address and socket (server_address and ServerSocket)
    return(retval);
}

int sendPacketTX(int player, char* data, int length, int packet_type)
{
	char temp[4096];
	temp[0] = packet_type;
	temp[1] = player;
	for (int x = 0; x < length; x++)
		temp[x+2] = data[x];
//	strcat(temp, data);
	return(packetAssembler(temp, length + 2));
}

int sendPacketTX(int player, string data, int packet_type)
{
	int retval = 0;
//	char tempchar[1] = { packet_type };
	char temp[4096];
	temp[0] = packet_type;
	temp[1] = player;
	for (int x = 0; x < data.length(); x++)
		temp[x+2] = data[x];
//	strcpy(temp, tempchar);
//	strcat(temp, data.c_str());
	retval = packetAssembler(temp, data.length()+2);
//	delete[] temp;
	return(retval);
}

int sendPacketTX(int player, int data, int packet_type)
{
	char temp[3] = { 0,0,0 };
	int retval = 0;
	temp[0] = packet_type;
	temp[1] = player;
	temp[2] = data;
	retval = packetAssembler(temp, 3);
/*
	convert << packet_type << player << data;
	datastr = convert.str();
	char *temp = new char[datastr.length()+2];
	for (int x = 0; x < datastr.length(); x++)
		temp[x] = datastr.c_str()[x];
//	strcpy(temp, datastr.c_str());
	retval = packetAssembler(temp, datastr.length());
	delete[] temp;
*/
	return(retval);
}

int tx_proposeTrade(int player_to_trade, client_trade_cards_offer trade)
{
	string data = "";
	data += player_to_trade;
	data += trade.qty_wood_to_trade;
	data += trade.qty_wood_to_receive;
	data += trade.qty_ore_to_trade;
	data += trade.qty_ore_to_receive;
	data += trade.qty_brick_to_trade;
	data += trade.qty_brick_to_receive;
	data += trade.qty_wheat_to_trade;
	data += trade.qty_wheat_to_receive;
	data += trade.qty_sheep_to_trade;
	data += trade.qty_sheep_to_receive;
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
	char data[1] = { START_TURN };
	return(packetAssembler(data, 1));
}

int tx_end_turn()
{
	return(sendPacketTX(0, 1, END_TURN));
}

int txhandler_build_road(int player, int tile, int road)
{
	//data[0] = tile number
	//data[1] = road index
	string tempdata = "";
	tempdata += tile;
	tempdata += road;
	return(sendPacketTX(player, tempdata, BUILD_ROAD));
}

int txhandler_build_settlement(int player, int tile, int corner)
{
	string tempdata = "";
	tempdata += tile;
	tempdata += corner;
	return(sendPacketTX(player, tempdata, BUILD_SETTLEMENT));
}

int txhandler_build_city(int player, int tile, int corner)
{
	string tempdata = "";
	tempdata += tile;
	tempdata += corner;
	return(sendPacketTX(player, tempdata, UPGRADE_SETTLEMENT));
}


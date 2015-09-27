#pragma once
#include "clientFramehandler.h"

int tx_build_road(int player, int tile, int road);
int tx_build_settlement(int player, int tile, int corner);
int tx_build_city(int player, int tile, int corner);
int sendPacketTX(int player, char* data, int length, int packet_type);
int sendPacketTX(int player, string data, int packet_type);
int sendPacketTX(int player, int data, int packet_type);
int tx_proposeTrade(int player_to_trade, client_trade_cards_offer trade);
int tx_accept_or_reject_trade(int player_to_trade, int status);
int tx_start_turn();
int tx_end_turn();

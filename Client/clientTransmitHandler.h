#pragma once


int sendPacketTX(int player, char* data, int length, int packet_type);
int sendPacketTX(int player, string data, int packet_type);
int sendPacketTX(int player, int data, int packet_type);
int proposeTrade(int player_to_trade, trade_cards_offer trade);
int accept_or_reject_trade(int player_to_trade, int status);
int start_turn();
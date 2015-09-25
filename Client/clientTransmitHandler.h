#pragma once


int sendPacketTX(int player, char* data, int length, int packet_type);
int sendPacketTX(int player, string data, int packet_type);
int sendPacketTX(int player, int data, int packet_type);
int tx_proposeTrade(int player_to_trade, trade_cards_offer trade);
int tx_accept_or_reject_trade(int player_to_trade, int status);
int tx_start_turn();
int tx_end_turn();

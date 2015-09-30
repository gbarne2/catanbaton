#pragma once

#include "clientFramehandler.h"
#include "clientTransmitHandler.h"
#include "playerClient.h"
#include "Tile_client.h"

int refresh_cards();
int get_player_num();
int start_turn();
int client_build_road(int tile, int corner);
int client_build_settlement(int tile, int corner);
int client_build_city(int tile, int corner);
int build_board();
void init_game();

//Flag definitions
static int FLAG_TURN_START;
static int FLAG_MY_TURN;
static int FLAG_BUILD_ROAD;
static int FLAG_BUILD_CITY;
static int FLAG_BUILD_SETTLEMENT;

//globals
extern playerClient playerdata;
extern tileclient board[ACTIVE_NUM_TILES_CLIENT];

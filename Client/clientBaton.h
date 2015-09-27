#pragma once

int refresh_cards();
int get_player_num();
int start_turn();
int build_road(int tile, int corner);
int build_settlement(int tile, int corner);

//Flag definitions
static int FLAG_TURN_START;
static int FLAG_MY_TURN;
static int FLAG_BUILD_ROAD;
static int FLAG_BUILD_SETTLEMENT;
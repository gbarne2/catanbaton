#pragma once
#include <vector>
#include <map>

#define WHEAT	1
#define ORE		2
#define WOOD	3
#define SHEEP	4
#define BRICK	5
#define DESERT	6

#define Acorn 0
#define Bcorn 1
#define Ccorn 2
#define Dcorn 3
#define Ecorn 4
#define Fcorn 5


using namespace std;


//format for sending data...
/*
index	|	Data
0			


*/

#ifndef dockvalues
#define dockvalues		0
#define WHEATDOCK		1
#define OREDOCK			2
#define WOODDOCK		3
#define SHEEPDOCK		4
#define BRICKDOCK		5
#define THREETOONEDOCK	6
#define NO_DOCK			7
#endif

struct dock
{
	//define the data members required for the dock tiles here.
	//for now these will be statically set?
};

struct Corner
{
	int corner_index;
	int road_connected;
	vector<int> players_connected;		//1-3 players can have roads connected to the same corner.
	int property_owner;
	int property_type;			//0 = none, 1 = settlement, 2 = city. makes it easy to multiply resources
	int dock_on_corner;	//if > 0, there is a dock here. the number in this var indicated the type of dock.
};

struct Corners_array
{
	vector<Corner> data;
};

class tile
{
private:
	int A,B,C,D,E,F;			//not used?
	int resource_type;
	int robber_on_tile;
	int roadAB, roadBC, roadCD, roadDE, roadEF, roadFA;		//not used
	int roll;
	vector<Corner>::iterator ptr;
	//	int corners[6];		//this will contain all of the corners, and their value will be used to determine which player owns them
										//for now, lets make it be the function that checks if a corner is available (check neighboring corners and desired corner)
	int check_roads_settlement(int corner, int player);
	int check_roads(int corner1, int corner2, int player);
public:
	int check_neighbors(int corner, int player);	//not sure how this is supposed to actually work. my plan was to have this higher I think, so it can check more than 1 tile. 
	tile(void);
	vector<Corner> cornersz;		//move this into the private part!!!! build_roads, ,  all depend on this being public. make update them as well
	tile(int res, int dc);
	~tile(void);
	int get_road_owner(int);
	int check_dice_roll();
	int calculate_resources_from_roll_by_player(int, int&);	//needs to sum up multiplier of # res player gets for this tile and return to user. should also pass resource type by reference to user.
//	int calculate_resources_from_roll_by_player(int, int, int&);	//needs to sum up multiplier of # res player gets for this tile and return to user. should also pass resource type by reference to user.
	int read_corner_owner(int corner);
	int read_corner_type(int corner);
	int read_road(int road);
	int set_dock_type(int corner, int type);
	int get_dock_type(int corner);
//	void init_corners(Corner temp) {cornersz.push_back(temp);}
	int check_build_settlement_tile(int corner, int player);
	int upgrade_settlement(int corner, int player);
	int build_road(int corner1, int corner2, int player);		//must call for both adjacent roads!
	int remove_settlement(int corner, int player);		//1 if successfully removed, -1 if not.
	void build_settlement(int corner, int player);
	int check_tile_resource_type();
	int check_corner_owner(int corner);
	int check_corner_building_type(int corner);
	int set_resource_type(int res_type);
	int set_dice_roll(int dicerollval);
	int check_robber(void);
	void place_robber(void);
	void remove_robber(void);
	string get_tile_data_string(int);
	int roads[6];		//this will contain the status of all 6 possible roads around a tile, and which player owns them.

	//i really need corners to be a struct, and it needs to contain the following info
	//if road is connectect to it from another tile (really just if a road is connected to that point)
	//what players have roads connected to that point
	//if a building is there
	//whose building the it is

};


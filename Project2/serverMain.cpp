#include <fcntl.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include "tcpserver.h"
#include <iomanip>
#include "game.h"
#include <fstream>
#include <sstream>
#include "player.h"
#include "tile.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "server_Catan.h"
#include "serverMain.h"


#define setwval 1
using namespace std;
char* tempaddr = "192.168.0.102";

const int max_clients = 16;
SOCKET socketarray[max_clients];
int initial_placement_phase = 0;
game catan;

static tcpserver serv(" ");
static tcpserver rxserv("");
static int lockrx = 0;
int droll(int tilenumb)
{
	return(catan.get_dice_roll(tilenumb));
}

int corner_info(int corner, int tilenum)
{
	int retval = 0;
	retval = (10 * catan.check_corner_owner(corner, tilenum) + catan.check_corner_building_type(corner, tilenum));
	return(retval);
}

int get_rsrc_type(int tilenum)
{
	return(catan.check_tile_resource_type(tilenum));
}

string print_board()
{
	std::ifstream filein;
	std::stringstream strStream;

	strStream = std::stringstream();	//flush string stream...
										//	strStream.open("tempfile.secret"); 
	strStream << setw(setwval) << "                           /" << setw(2) << corner_info(cornB, 18) << "¯¯¯" << setw(2) << corner_info(cornC, 18) << "\\                         " << endl;
	strStream << setw(setwval) << "                          /" << setw(2) << corner_info(cornA, 18) << " |" << setw(2) << droll(18) << "|" << setw(2) << corner_info(cornD, 18) << "\\               " << endl;
	strStream << setw(setwval) << "                 /" << setw(2) << corner_info(cornB, 15) << "¯¯¯" << setw(2) << corner_info(cornC, 15) << "\\\\   18," << get_rsrc_type(18) << "  //" << setw(2) << corner_info(cornB, 17) << "¯¯¯" << setw(2) << corner_info(cornC, 17) << "\\               " << endl;
	strStream << setw(setwval) << "                /" << setw(2) << corner_info(cornA, 15) << " |" << setw(2) << droll(15) << "|" << setw(2) << corner_info(cornD, 15) << "\\\\" << setw(2) << corner_info(cornF, 18) << "___" << setw(2) << corner_info(cornE, 18) << "//" << setw(2) << corner_info(cornA, 17) << " |" << setw(2) << droll(17) << "|" << setw(2) << corner_info(cornD, 17) << "\\                  " << endl;				//		A-C-E Navigation matrix:
	strStream << setw(setwval) << "       /" << setw(2) << corner_info(cornB, 11) << "¯¯¯" << setw(2) << corner_info(cornC, 11) << "\\\\   15," << get_rsrc_type(15) << "  //" << setw(2) << corner_info(cornB, 14) << "¯¯¯" << setw(2) << corner_info(cornC, 14) << "\\\\   17," << get_rsrc_type(17) << "  //" << setw(2) << corner_info(cornB, 16) << "¯¯¯" << setw(2) << corner_info(cornC, 16) << "\\          " << endl;
	//				/¯¯¯¯¯¯¯\\	  15   //¯¯¯¯¯¯¯\\	  17   //¯¯¯¯¯¯¯\" << endl;		//		A = (0,2)
	strStream << setw(setwval) << "      /" << setw(2) << corner_info(cornA, 11) << " |" << setw(2) << droll(11) << "|" << setw(2) << corner_info(cornD, 11) << "\\\\" << setw(2) << corner_info(cornF, 15) << "___" << setw(2) << corner_info(cornE, 15) << "//" << setw(2) << corner_info(cornA, 14) << " |" << setw(2) << droll(14) << "|" << setw(2) << corner_info(cornD, 14) << "\\\\" << setw(2) << corner_info(cornF, 17) << "___" << setw(2) << corner_info(cornE, 17) << "//" << setw(2) << corner_info(cornA, 16) << " |" << setw(2) << droll(16) << "|" << setw(2) << corner_info(cornD, 16) << "\\        " << endl;
	//			   /  (2,4)	 \\_______//  (3,3)	 \\_______//  (4,2)	 \				C = (1,2)
	strStream << setw(setwval) << "      \\   11," << get_rsrc_type(11) << "  //" << setw(2) << corner_info(cornB, 10) << "¯¯¯" << setw(2) << corner_info(cornC, 10) << "\\\\   14," << get_rsrc_type(14) << "  //" << setw(2) << corner_info(cornB, 13) << "¯¯¯" << setw(2) << corner_info(cornC, 13) << "\\\\   16," << get_rsrc_type(16) << "  /" << endl;
	//		   \   11	 //¯¯¯¯¯¯¯\\	14	 //¯¯¯¯¯¯¯\\   16	 /" << endl;	//		E = (0,1)
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 11) << "___" << setw(2) << corner_info(cornE, 11) << "//" << setw(2) << corner_info(cornA, 10) << " |" << setw(2) << droll(10) << "|" << setw(2) << corner_info(cornD, 10) << "\\\\" << setw(2) << corner_info(cornF, 14) << "___" << setw(2) << corner_info(cornE, 14) << "//" << setw(2) << corner_info(cornA, 13) << " |" << setw(2) << droll(13) << "|" << setw(2) << corner_info(cornD, 13) << "\\\\" << setw(2) << corner_info(cornF, 16) << "___" << setw(2) << corner_info(cornE, 16) << "/         " << endl;
	//				\_______//	(2,3)  \\_______//	(3,2)  \\_______/" << endl;
	strStream << setw(setwval) << "       /" << setw(2) << corner_info(cornB, 6) << "¯¯¯" << setw(2) << corner_info(cornC, 6) << "\\\\   10," << get_rsrc_type(10) << "  //" << setw(2) << corner_info(cornB, 9) << "¯¯¯" << setw(2) << corner_info(cornC, 9) << "\\\\  13," << get_rsrc_type(13) << "   //" << setw(2) << corner_info(cornB, 12) << "¯¯¯" << setw(2) << corner_info(cornC, 12) << "\\     " << endl;
	//				/¯¯¯¯¯¯¯\\	 10	   //¯¯¯¯¯¯¯\\	 13    //¯¯¯¯¯¯¯\'" << endl;
	strStream << setw(setwval) << "      /" << setw(2) << corner_info(cornA, 6) << " |" << setw(2) << droll(6) << "|" << setw(2) << corner_info(cornD, 6) << "\\\\" << setw(2) << corner_info(cornF, 5) << "___" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 9) << " |" << setw(2) << droll(9) << "|" << setw(2) << corner_info(cornD, 9) << "\\\\" << setw(2) << corner_info(cornF, 13) << "___" << setw(2) << corner_info(cornE, 13) << "//" << setw(2) << corner_info(cornA, 12) << " |" << setw(2) << droll(12) << "|" << setw(2) << corner_info(cornD, 12) << "\\     " << endl;
	//			   /  (1,3)	 \\_______//  (2,2)	 \\_______//  (3,1)	 \'" << endl;	//		B-D-F Navigation matrix:
	strStream << setw(setwval) << "      \\   6," << get_rsrc_type(6) << "   //" << setw(2) << corner_info(cornB, 5) << "¯¯¯" << setw(2) << corner_info(cornC, 5) << "\\\\   9," << get_rsrc_type(9) << "   //" << setw(2) << corner_info(cornB, 8) << "¯¯¯" << setw(2) << corner_info(cornC, 8) << "\\\\   12," << get_rsrc_type(12) << "  /" << endl;
	//			   \	6	 //¯¯¯¯¯¯¯\\	9	 //¯¯¯¯¯¯¯\\	12	 /" << endl;	//		B = (2,2)
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 6) << "___" << setw(2) << corner_info(cornE, 6) << "//" << setw(2) << corner_info(cornA, 5) << " |" << setw(2) << droll(5) << "|" << setw(2) << corner_info(cornD, 5) << "\\\\" << setw(2) << corner_info(cornF, 9) << "___" << setw(2) << corner_info(cornE, 9) << "//" << setw(2) << corner_info(cornA, 8) << " |" << setw(2) << droll(8) << "|" << setw(2) << corner_info(cornD, 8) << "\\\\" << setw(2) << corner_info(cornF, 12) << "___" << setw(2) << corner_info(cornE, 12) << "/     " << endl;
	//			    \_______//	(1,2)  \\_______//	(2,1)  \\_______/" << endl;		//		D = (2,1)
	strStream << setw(setwval) << "       /" << setw(2) << corner_info(cornB, 2) << "¯¯¯" << setw(2) << corner_info(cornC, 2) << "\\\\   5," << get_rsrc_type(5) << "   //" << setw(2) << corner_info(cornB, 4) << "¯¯¯" << setw(2) << corner_info(cornC, 4) << "\\\\   8," << get_rsrc_type(8) << "   //" << setw(2) << corner_info(cornB, 7) << "¯¯¯" << setw(2) << corner_info(cornC, 7) << "\\     " << endl;
	//				/¯¯¯¯¯¯¯\\	  5	   //¯¯¯¯¯¯¯\\	  8	   //¯¯¯¯¯¯¯\'" << endl;		//		F = (1,1)
	strStream << setw(setwval) << "      /" << setw(2) << corner_info(cornA, 2) << " |" << setw(2) << droll(2) << "|" << setw(2) << corner_info(cornD, 2) << "\\\\" << setw(2) << corner_info(cornF, 5) << "___" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 4) << " |" << setw(2) << droll(4) << "|" << setw(2) << corner_info(cornD, 4) << "\\\\" << setw(2) << corner_info(cornF, 8) << "___" << setw(2) << corner_info(cornE, 8) << "//" << setw(2) << corner_info(cornA, 7) << " |" << setw(2) << droll(7) << "|" << setw(2) << corner_info(cornD, 7) << "\\     " << endl;
	//			   /  (0,2)	 \\_______//  (1,1)	 \\_______//  (2,0)	 \'" << endl;
	strStream << setw(setwval) << "      \\   2," << get_rsrc_type(2) << "   //" << setw(2) << corner_info(cornB, 1) << "¯¯¯" << setw(2) << corner_info(cornC, 1) << "\\\\   4," << get_rsrc_type(4) << "   //" << setw(2) << corner_info(cornB, 3) << "¯¯¯" << setw(2) << corner_info(cornC, 3) << "\\\\   7," << get_rsrc_type(7) << "   /" << endl;
	//			   \	2	 //¯¯¯¯¯¯¯\\    4    //¯¯¯¯¯¯¯\\	7	 /" << endl;
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 2) << "___" << setw(2) << corner_info(cornE, 2) << "//" << setw(2) << corner_info(cornA, 1) << " |" << setw(2) << droll(1) << "|" << setw(2) << corner_info(cornD, 1) << "\\\\" << setw(2) << corner_info(cornF, 4) << "___" << setw(2) << corner_info(cornE, 4) << "//" << setw(2) << corner_info(cornA, 3) << " |" << setw(2) << droll(3) << "|" << setw(2) << corner_info(cornD, 3) << "\\\\" << setw(2) << corner_info(cornF, 7) << "___" << setw(2) << corner_info(cornE, 7) << "/     " << endl;
	//			    \_______//	(0,1)  \\_______//	(1,0)  \\_______/" << endl;
	strStream << setw(setwval) << "                \\    1," << get_rsrc_type(1) << "  //" << setw(2) << corner_info(cornB, 0) << "¯¯¯" << setw(2) << corner_info(cornC, 0) << "\\\\   3," << get_rsrc_type(3) << "   /" << endl;
	//						 \    1    //¯¯¯¯¯¯¯\\	  3    /" << endl;
	strStream << setw(setwval) << "                 \\" << setw(2) << corner_info(cornF, 1) << "___" << setw(2) << corner_info(cornE, 1) << "//" << setw(2) << corner_info(cornA, 0) << " |" << setw(2) << droll(0) << "|" << setw(2) << corner_info(cornD, 0) << "\\\\" << setw(2) << corner_info(cornF, 3) << "___" << setw(2) << corner_info(cornE, 3) << "/     " << endl;
	//						  \_______//  (0,0)	 \\_______/" << endl;
	strStream << setw(setwval) << "                          \\    0," << get_rsrc_type(0) << "  /     " << endl;

	strStream << setw(setwval) << "                           \\" << setw(2) << corner_info(cornF, 0) << "___" << setw(2) << corner_info(cornE, 0) << "/" << endl;
	//									\_______/" << endl;
	//	filein.open("tempfile.secret");
	//	filein.rdbuf() >> s;
	//	strStream << filein.rdbuf();
	return(strStream.str());
}

SOCKET srvinit(game session, tcpserver &tcpserv)
{
	//will need to spawn a new thread for each players connection. the connection shouldnt be closed until either the end of the game or the player leaves the game
//	cout << "Starting server server" << endl;
	SOCKET tempsocket = INVALID_SOCKET;
	tempsocket = tcpserv.initializeServer(tempsocket);
//	cout << "Server started" << endl;
//	tcpserv.receiveUntilDoneWithEcho(tempsocket);
//	tcpserv.shutDownClientSocket(tempsocket);
//	tcpserv.cleanup(tempsocket);
	return(tempsocket);
}

char databuff[4096] = { 0, };
static int connplayerf = 0;
int current_num_clients = 0;
int updating_session = 0;

void update_session(game &session, int copy)
{
	static game savesession;
	if (copy == 0)
		session = savesession;
	else if (copy == 1)
		savesession = session;
	else
		cout << "invalid input to update_session!" << endl;
}

SOCKET connectPlayers(game &session, tcpserver &tcpserv)
{
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	SOCKET tempsock;
	int retval = 0;
	while (game_status == 0)
	{
		while (updating_session)
			Sleep(5);
		if (!updating_session)
		{
			updating_session = 1;
			update_session(session, 0);

			tempsock = srvinit(session, tcpserv);
			if (tempsock != INVALID_SOCKET)	//!lockrx)
			{
				cout << "Player connected!" << endl;
				socketarray[current_num_clients++] = tempsock;
				lockrx = 1;
				retval = tcpserv.receiveUntilDone(tempsock);
				if (retval > 0)
					retval = framehandler(session, databuff, 4096, tcpserv, tempsock);		//join players and what not.
				else
				{
					//if retval < 0, then the connection was screwed up or something. delete this socket!
					//if retval = 0, then connectin was closed. delete socket
					current_num_clients -= 1;
					socketarray[current_num_clients] = INVALID_SOCKET;
				}
				lockrx = 0;
				Sleep(500);
			}
			update_session(session, 1);
			updating_session = 0;
			Sleep(10);
		}
		/*
				if ((session.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
				{
					for (int i = 1; i < current_num_clients + 1; i++)		//currently player 0 is an empty place holder. dont check this or it will get deleted!
					{
						lockrx = 1;
						ptr = session.player_list.begin() + i;
						hostsocket = ptr->get_client_socket();
						if (hostsocket != INVALID_SOCKET)
						{
							tcpserv.receiveUntilDone(hostsocket);
							retval = framehandler(session, databuff, 4096, tcpserv, hostsocket);
						}
						else
						{
							session.delete_player(i);
							cout << "Test text, socket invalid in process data thread" << endl;
						}
						lockrx = 0;
					}
				}
				else
				{
					if (current_num_clients > session.check_number_of_players())	//if server thinks it has more players than it actually does at the top level, fix it. this can happen because the number of players in a game isnt sync'd to current_num_clients. players is updated when a client quits (i think)
					{
						//				while (current_num_clients > session.check_number_of_players())
						//				{
						//					current_num_clients -= 1;
						//					socketarray[current_num_clients] = INVALID_SOCKET;
						//				}
						lockrx = 0;
					}
				}
		*/
	}
	return(retval);
}

int processData(game &session, tcpserver &tcpserv)
{
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	int retval = 0;
	while (game_status == 0)
	{
		while (updating_session)
			Sleep(1);
		if (!updating_session)
		{
			updating_session = 1;
			update_session(session, 0);	//update this functions game data to the latest version in this function
			updating_session = 0;	//unblock the other function and search for incoming data
		}
		if ((session.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 1; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = session.player_list.begin() + i;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					retval = tcpserv.receiveUntilDone(hostsocket);
					if (retval > 0)
					{
						while (updating_session)
							Sleep(1);
						if (!updating_session)
						{
							updating_session = 1;
							update_session(session, 0);	//update this functions game data to the latest version in this function
							retval = framehandler(session, databuff, 4096, tcpserv, hostsocket);
							update_session(session, 1);
							updating_session = 0;
						}
					}
				}
				else
				{
					while (updating_session)
						Sleep(1);
					if (!updating_session)
					{
						updating_session = 1;
						update_session(session, 0);	//update this functions game data to the latest version in this function
						session.delete_player(i);
						update_session(session, 1);
						updating_session = 0;
						cout << "Test text, socket invalid in process data thread" << endl;
					}
				}
				lockrx = 0;
			}
		}
		else
		{
			if (current_num_clients > session.check_number_of_players())	//if server thinks it has more players than it actually does at the top level, fix it. this can happen because the number of players in a game isnt sync'd to current_num_clients. players is updated when a client quits (i think)
			{
				//				while (current_num_clients > session.check_number_of_players())
				//				{
				//					current_num_clients -= 1;
				//					socketarray[current_num_clients] = INVALID_SOCKET;
				//				}
				lockrx = 0;
			}
		}
		//			update_session(session, 1);		//update the static copy in this function
		updating_session = 0;
		Sleep(10);
	}
	return(retval);
}
/*
int processData_Game(game &session, tcpserver &tcpserv)
{
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	int retval = 0;
	while (game_status ==1)
	{
		if ((session.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 0; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = session.player_list.begin() + 1;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					tcpserv.receiveUntilDone(hostsocket);
					retval = framehandler(session, databuff, 4096, tcpserv, hostsocket);
				}
				lockrx = 0;
			}
		}
	}
	return(retval);
}
*/
int main()
{
	SOCKET usethisasshole = INVALID_SOCKET;
	int retval = 1;
	int retval2 = 0;
	vector<player>::iterator ptr;
	SOCKET hostsocket = INVALID_SOCKET;
	//	thread uno(srvinit, catan, server);
//	usethisasshole = srvinit(catan, serv);
	thread two(processData, catan, rxserv);
	thread one(connectPlayers, catan, serv);
	int count = 0;
	const char* strtowrite;
	string tempstr;
//	connectPlayers(catan, serv);
	while(game_status == 0)
		Sleep(500);
	one.join();
	two.join();
	initial_placement_phase = 1;
//	catan.build_std_board(19);
	update_session(catan, 0);
	while ((game_status == 1) && (initial_placement_phase == 1))// && (count < catan.check_number_of_players()))
	{
		if ((catan.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 1; i < catan.check_number_of_players() + 1; i++)
			{
				lockrx = 1;
				ptr = catan.player_list.begin() + i;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					u_long iMode = 1;	//if = 1, then socket should be nonblocking. if = 0, it should be blocking.
					ioctlsocket(hostsocket, FIONBIO, &iMode);
					retval = rxserv.receiveUntilDone(hostsocket);
					if (retval == 0)
					{
						catan.delete_player(i);
						cout << "Player removed from game since connection closed" << endl;
					}
					else if (retval > 0)
					{
						framehandler(catan, databuff, 4096, rxserv, hostsocket);
						tempstr = print_board();
						strtowrite = tempstr.c_str();
						cout << strtowrite << endl;
					}
					else
					{
						//if not >= 0, nothing was received!
						Sleep(100);	//give it a break so it has time to receive something!
					}
				}
				lockrx = 0;
			}
		}

	}
	while (game_status == 1)
	{
		if ((catan.check_number_of_players() > 0))// && (!lockrx))	//if anyone has joined game, then allow them to send some stuff.
		{
			for (int i = 1; i < current_num_clients + 1; i++)
			{
				lockrx = 1;
				ptr = catan.player_list.begin() + i;
				hostsocket = ptr->get_client_socket();
				if (hostsocket != INVALID_SOCKET)
				{
					retval2 = rxserv.receiveUntilDone(hostsocket);
					if (retval2 == 0)	//connection closed...
					{
						cerr << "connection closed! make this handle the situation!" << endl;
						catan.delete_player(i);
					}
					else if (retval > 0)
					{
						retval = framehandler(catan, databuff, 4096, rxserv, hostsocket);
						tempstr = print_board();
						strtowrite = tempstr.c_str();
						cout << strtowrite << endl;
					}
					else
					{
						Sleep(100);
					}
				}
				lockrx = 0;
			}
		}
	}
/*
	while (1)
	{
		//retval = serv.receiveUntilDone(usethisasshole);
		if (retval == 0)		//if 0, packet was recievced. if 1, wtf happened? if -1, nothing was received.
		{
			retval2 = framehandler(catan, databuff, 4096, serv, usethisasshole);
		}
	}
*/
	return(0);
}


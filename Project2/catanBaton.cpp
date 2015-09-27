//This function will end up having all GUI things removed and will serve as the controller.
//Game, tile, and player classes will serve as the models (data objects and what not)
//then some GUI will tie into this function and then allow user to send data to contorller



/*							    /¯¯¯¯¯¯¯\
							   /  (4,4)	 \
                      /¯¯¯¯¯¯¯\\   18	 //¯¯¯¯¯¯¯\						To find touching corners on other hexs, subtract the following to the x,y coordinate of current tile (if you are on corner A, and looking for corner C, add C's values)
		             /  (3,4)  \\_______//	(4,3)  \						A-C-E Navigation matrix:
			/¯¯¯¯¯¯¯\\	  15   //¯¯¯¯¯¯¯\\	  17   //¯¯¯¯¯¯¯\				A = (0,2)
		   /  (2,4)	 \\_______//  (3,3)	 \\_______//  (4,2)	 \				C = (1,2)
		   \    11	 //¯¯¯¯¯¯¯\\	14	 //¯¯¯¯¯¯¯\\   16	 /				E = (0,1)
		    \_______//	(2,3)  \\_______//	(3,2)  \\_______/
			/¯¯¯¯¯¯¯\\	 10	   //¯¯¯¯¯¯¯\\	 13    //¯¯¯¯¯¯¯\
		   /  (1,3)	 \\_______//  (2,2)	 \\_______//  (3,1)	 \				B-D-F Navigation matrix:
		   \	6	 //¯¯¯¯¯¯¯\\	9	 //¯¯¯¯¯¯¯\\	12	 /				B = (2,2)
		    \_______//	(1,2)  \\_______//	(2,1)  \\_______/				D = (2,1)
			/¯¯¯¯¯¯¯\\	  5	   //¯¯¯¯¯¯¯\\	  8	   //¯¯¯¯¯¯¯\				F = (1,1)
		   /  (0,2)	 \\_______//  (1,1)	 \\_______//  (2,0)	 \
		   \	2	 //¯¯¯¯¯¯¯\\	4	 //¯¯¯¯¯¯¯\\	7	 /
		    \_______//	(0,1)  \\_______//	(1,0)  \\_______/
					 \	  1    //¯¯¯¯¯¯¯\\	  3    /
					  \_______//  (0,0)	 \\_______/
							   \	0	 /
							    \_______/
*/

#include <iostream>
#include <iomanip>
#include "game.h"
#include <fstream>
#include <sstream>
#include "player.h"
#include "tile.h"
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include "clientBaton.h"
#include <stdio.h>
#include <thread>
#include "tcpclient.h"
#include "tcpserver.h"
#include "clientTransmitHandler.h"
#include "server_Catan.h"


#define setwval 1
using namespace std;

//game catangame();

	game catan;
int corner_info(int corner, int tilenum)
{
//	cout << catan.check_corner_owner(cornB, 18) << catan.check_corner_building_type(cornB, 18);
	return(10*catan.check_corner_owner(corner, tilenum) + catan.check_corner_building_type(corner, tilenum));
}

int droll(int tilenumb)
{
	return(catan.get_dice_roll(tilenumb));
}

string print_board()
{
	std::ifstream filein;
	std::stringstream strStream;

	strStream = std::stringstream();	//flush string stream...
//	strStream.open("tempfile.secret"); 
	strStream << setw(setwval) << "                           /" << setw(2) << corner_info(cornB,18) << "¯¯¯" << setw(2) << corner_info(cornC, 18) << "\\                         " << endl;
	strStream << setw(setwval) << "                          /" << setw(2) << corner_info(cornA, 18) << " |" << setw(2) << droll(18) << "|" << setw(2) << corner_info(cornD, 18) << "\\               " << endl;
    strStream << setw(setwval) << "                 /" << setw(2) << corner_info(cornB,15) << "¯¯¯" << setw(2) << corner_info(cornC, 15) << "\\\\    18   //" << setw(2) << corner_info(cornB, 17) << "¯¯¯" <<  setw(2) << corner_info(cornC, 17) << "\\               " << endl;					
	strStream << setw(setwval) << "                /" << setw(2) << corner_info(cornA, 15) << " |" << setw(2) << droll(15) << "|" << setw(2) << corner_info(cornD, 15) << "\\\\" << setw(2) << corner_info(cornF, 18) << "___" << setw(2) << corner_info(cornE, 18) << "//" << setw(2) << corner_info(cornA, 17) << " |" << setw(2) << droll(17) << "|" << setw(2) << corner_info(cornD, 17) << "\\                  " << endl;				//		A-C-E Navigation matrix:
	strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,11) << "¯¯¯" << setw(2) << corner_info(cornC, 11) << "\\\\   15    //" << setw(2) << corner_info(cornB, 14) << "¯¯¯" <<  setw(2) << corner_info(cornC, 14) << "\\\\   17    //" <<  setw(2) << corner_info(cornB,16) << "¯¯¯" << setw(2) << corner_info(cornC, 16) << "\\          " << endl;
	//				/¯¯¯¯¯¯¯\\	  15   //¯¯¯¯¯¯¯\\	  17   //¯¯¯¯¯¯¯\" << endl;		//		A = (0,2)
	strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,11) << " |" << setw(2) << droll(11) << "|" << setw(2) << corner_info(cornD, 11) << "\\\\" << setw(2) << corner_info(cornF, 15) << "___" << setw(2) << corner_info(cornE, 15) << "//" << setw(2) << corner_info(cornA, 14) << " |" << setw(2) << droll(14) << "|" <<  setw(2) << corner_info(cornD, 14) << "\\\\" << setw(2) << corner_info(cornF, 17) << "___" << setw(2) << corner_info(cornE, 17) << "//" <<  setw(2) << corner_info(cornA,16) << " |" << setw(2) << droll(16) << "|" << setw(2) << corner_info(cornD, 16) << "\\        " << endl;
	//			   /  (2,4)	 \\_______//  (3,3)	 \\_______//  (4,2)	 \				C = (1,2)
	strStream << setw(setwval) << "      \\    11   //" << setw(2) << corner_info(cornB, 10) << "¯¯¯" << setw(2) << corner_info(cornC, 10) << "\\\\    14   //" <<setw(2) << corner_info(cornB, 13) << "¯¯¯" << setw(2) << corner_info(cornC, 13) << "\\\\   16    /          " << endl;
		//		   \   11	 //¯¯¯¯¯¯¯\\	14	 //¯¯¯¯¯¯¯\\   16	 /" << endl;	//		E = (0,1)
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 11) << "___" << setw(2) << corner_info(cornE, 11) << "//" << setw(2) << corner_info(cornA, 10) << " |" << setw(2) << droll(10) << "|" << setw(2) << corner_info(cornD, 10) << "\\\\" << setw(2) << corner_info(cornF, 14) << "___" << setw(2) << corner_info(cornE, 14) << "//" << setw(2) << corner_info(cornA, 13) << " |" << setw(2) << droll(13) << "|" << setw(2) << corner_info(cornD, 13) << "\\\\" << setw(2) << corner_info(cornF, 16) << "___" << setw(2) << corner_info(cornE, 16) << "/         " << endl; 
	//				\_______//	(2,3)  \\_______//	(3,2)  \\_______/" << endl;
	strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,6) << "¯¯¯" << setw(2) << corner_info(cornC, 6) << "\\\\    10   //" << setw(2) << corner_info(cornB, 9) << "¯¯¯" <<  setw(2) << corner_info(cornC, 9) << "\\\\    13   //" <<  setw(2) << corner_info(cornB,12) << "¯¯¯" << setw(2) << corner_info(cornC, 12) << "\\     " << endl;
	//				/¯¯¯¯¯¯¯\\	 10	   //¯¯¯¯¯¯¯\\	 13    //¯¯¯¯¯¯¯\'" << endl;
	strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,6) << " |" << setw(2) << droll(6) << "|" << setw(2) << corner_info(cornD, 6) << "\\\\" << setw(2) << corner_info(cornF, 5) << "___" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 9) << " |" << setw(2) << droll(9) << "|" <<  setw(2) << corner_info(cornD, 9) << "\\\\" << setw(2) << corner_info(cornF, 13) << "___" << setw(2) << corner_info(cornE, 13) << "//" <<  setw(2) << corner_info(cornA,12) << " |" << setw(2) << droll(12) << "|" << setw(2) << corner_info(cornD, 12) << "\\     " << endl;
	//			   /  (1,3)	 \\_______//  (2,2)	 \\_______//  (3,1)	 \'" << endl;	//		B-D-F Navigation matrix:
	strStream << setw(setwval) << "      \\    6    //" << setw(2) << corner_info(cornB, 5) << "¯¯¯" << setw(2) << corner_info(cornC, 5) << "\\\\    9    //" <<setw(2) << corner_info(cornB, 8) << "¯¯¯" << setw(2) << corner_info(cornC, 8) << "\\\\   12    /     " << endl;
	//			   \	6	 //¯¯¯¯¯¯¯\\	9	 //¯¯¯¯¯¯¯\\	12	 /" << endl;	//		B = (2,2)
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 6) << "___" << setw(2) << corner_info(cornE, 6) << "//" << setw(2) << corner_info(cornA, 5) << " |" << setw(2) << droll(5) << "|" << setw(2) << corner_info(cornD, 5) << "\\\\" << setw(2) << corner_info(cornF, 9) << "___" << setw(2) << corner_info(cornE, 9) << "//" << setw(2) << corner_info(cornA, 8) << " |" << setw(2) << droll(8) << "|" << setw(2) << corner_info(cornD, 8) << "\\\\" << setw(2) << corner_info(cornF, 12) << "___" << setw(2) << corner_info(cornE, 12) << "/     " << endl; 
	//			    \_______//	(1,2)  \\_______//	(2,1)  \\_______/" << endl;		//		D = (2,1)
	strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,2) << "¯¯¯" << setw(2) << corner_info(cornC, 2) << "\\\\    5    //" << setw(2) << corner_info(cornB, 4) << "¯¯¯" <<  setw(2) << corner_info(cornC, 4) << "\\\\    8    //" <<  setw(2) << corner_info(cornB,7) << "¯¯¯" << setw(2) << corner_info(cornC, 7) << "\\     " << endl;
	//				/¯¯¯¯¯¯¯\\	  5	   //¯¯¯¯¯¯¯\\	  8	   //¯¯¯¯¯¯¯\'" << endl;		//		F = (1,1)
	strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,2) << " |" << setw(2) << droll(2) << "|" << setw(2) << corner_info(cornD, 2) << "\\\\" << setw(2) << corner_info(cornF, 5) << "___" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 4) << " |" << setw(2) << droll(4) << "|" <<  setw(2) << corner_info(cornD, 4) << "\\\\" << setw(2) << corner_info(cornF, 8) << "___" << setw(2) << corner_info(cornE, 8) << "//" <<  setw(2) << corner_info(cornA,7) << " |" << setw(2) << droll(7) << "|" << setw(2) << corner_info(cornD, 7) << "\\     " << endl;
	//			   /  (0,2)	 \\_______//  (1,1)	 \\_______//  (2,0)	 \'" << endl;
	strStream << setw(setwval) << "      \\    2    //" << setw(2) << corner_info(cornB, 1) << "¯¯¯" << setw(2) << corner_info(cornC, 1) << "\\\\    4    //" <<setw(2) << corner_info(cornB, 3) << "¯¯¯" << setw(2) << corner_info(cornC, 3) << "\\\\    7    /     " << endl;
	//			   \	2	 //¯¯¯¯¯¯¯\\    4    //¯¯¯¯¯¯¯\\	7	 /" << endl;
	strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 2) << "___" << setw(2) << corner_info(cornE, 2) << "//" << setw(2) << corner_info(cornA, 1) << " |" << setw(2) << droll(1) << "|" << setw(2) << corner_info(cornD, 1) << "\\\\" << setw(2) << corner_info(cornF, 4) << "___" << setw(2) << corner_info(cornE, 4) << "//" << setw(2) << corner_info(cornA, 3) << " |" << setw(2) << droll(3) << "|" << setw(2) << corner_info(cornD, 3) << "\\\\" << setw(2) << corner_info(cornF, 7) << "___" << setw(2) << corner_info(cornE, 7) << "/     " << endl; 
	//			    \_______//	(0,1)  \\_______//	(1,0)  \\_______/" << endl;
	strStream << setw(setwval) << "                \\    1    //" <<  setw(2) << corner_info(cornB, 0) << "¯¯¯" <<  setw(2) << corner_info(cornC, 0) << "\\\\    3    /" << endl;
	//						 \    1    //¯¯¯¯¯¯¯\\	  3    /" << endl;
	strStream << setw(setwval) << "                 \\" << setw(2) << corner_info(cornF, 1) << "___" << setw(2) << corner_info(cornE, 1) << "//" << setw(2) << corner_info(cornA, 0) << " |" << setw(2) << droll(0) << "|" << setw(2) << corner_info(cornD, 0) << "\\\\" << setw(2) << corner_info(cornF, 3) << "___" << setw(2) << corner_info(cornE, 3) << "/     " << endl;
	//						  \_______//  (0,0)	 \\_______/" << endl;
	strStream << setw(setwval) << "                          \\    0    /     " << endl;

	strStream << setw(setwval) << "                           \\" << setw(2) << corner_info(cornF, 0) << "___" << setw(2) << corner_info(cornE, 0) << "/" << endl;
	//									\_______/" << endl;
//	filein.open("tempfile.secret");
//	filein.rdbuf() >> s;
//	strStream << filein.rdbuf();
	return(strStream.str());
}

BOOL WINAPI ConsoleHandler(DWORD ctrl_type) 
{
    if ( ctrl_type == CTRL_C_EVENT || ctrl_type == CTRL_BREAK_EVENT ) 
	{
        ExitProcess(0);
    }
    return FALSE;
}


char* tempaddr = "127.0.0.1";


void srvinit(game session, tcpserver tcpserv)
{
	//will need to spawn a new thread for each players connection. the connection shouldnt be closed until either the end of the game or the player leaves the game
	cout << "Starting server server" << endl;
	SOCKET tempsocket = INVALID_SOCKET;
	tempsocket = tcpserv.initializeServer(tempsocket);
	tcpserv.receiveUntilDoneWithEcho(tempsocket);
	tcpserv.shutDownClientSocket(tempsocket);
	tcpserv.cleanup(tempsocket);
}

void clinit(tcpclient tcpcli)
{
	cout << "starting client server" << endl;
	tcpcli.initWinsock(tempaddr);
}

extern char txdatabuff[4096] = { 0, };
extern char rxdatabuff[4096] = { 0, };
extern int sizerxbuff = 0;
extern int sizetxbuff = 0;

int main()
{
	int x = 0;
	int temp = 0;
	catan;
	int user_input = -1;
	int user_input2 = -1;
	int user_input3 = -1;
	int current_player = 0;
	int	rollnumvalamt = 0;
	int number_of_players = 0;
	char tempp = 0;
	HANDLE console_handle;
	COORD dwSize;
	COORD dwPosition;
	COORD dwPosition1;
	COORD dwPosition2;
//	DWORD written;
	DWORD flag = 0;
	const char* strtowrite;
	stringstream tempstrtowrite;
	const char* tempstrtowrite2;
	string tempstr;
	CONSOLE_CURSOR_INFO cursor_info;
	tcpclient client(tempaddr);
	tcpserver server(tempaddr);
//	thread uno(srvinit, catan, server);
//	thread dos(clinit, client);
	cout << endl << "threads are running! this is from the main thread" << endl;
	Sleep(3000);
//	uno.join();
//	dos.join();
	cout << "type a number" << endl;
	cin >> user_input;
	cout << "you entered " << user_input << endl;
	cout << "Test";
	console_handle = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	SetConsoleActiveScreenBuffer(console_handle);
//	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	dwSize.X = 80;
	dwSize.Y = 50;
//	SetConsoleScreenBufferSize(console_handle, dwSize);
	cursor_info.dwSize = 1;
	cursor_info.bVisible = TRUE;
//	SetConsoleCursorInfo(console_handle, &cursor_info);
//	SetConsoleTitle("Catan Baton!");
//	SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	dwPosition.X = 0;     
	dwPosition.Y = 2;
	dwPosition1.X = 0;
	dwPosition1.Y = 30;
	dwPosition2.X = 0;
	dwPosition2.Y = 0;
//	if(flag ^= 1)
//		cout <<

	//the init and add player functions should all be sucked into the core program, then build GUI in parallel
	catan.build_std_board(19);		
	catan.add_player(0, "void");		//make empty character so indexes dont get messed up
	number_of_players = catan.add_player(1, "titsmcgee");
	number_of_players = catan.add_player(2, "nippleblader") - 1;
	for(int i = 1; i < 6; i++)
	{
		temp = catan.check_resources(2, i);
		cout << "Resource type: " << i << endl;
		cout << "Resource amount: " << temp << endl;
	}
	catan.temp_build_settlement(18, 2, cornF);
	catan.temp_build_settlement(12, 1, cornB);
	catan.temp_build_settlement(6,  2, cornC);
	catan.temp_build_settlement(13, 1, cornA);
//	SetConsoleCursorPosition(console_handle, dwPosition);
//	SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	catan.temp_build_settlement(1,  2, cornD);
	catan.temp_build_settlement(0,  1, cornF);
	//catan.temp_build_settlement(9,  2, cornE);
//	SetConsoleCursorPosition(console_handle, dwPosition);
//	cout << "\n\n\n", 3, &written, NULL);
	cout << endl << endl << endl;
//	cin.clear();
//	cin.ignore(INT_MAX, 0);
	while(x != 69)		//loop and wait for user input then do what they want.
	{
	//	Sleep(5000);
	//	SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//	SetConsoleCursorPosition(console_handle, dwPosition);	
		if(current_player == 1)
			cout << "Player 1's turn!                                        " << endl;
		else if(current_player == 2)
			cout << "Player 2's turn!                                        " << endl;
		else if(current_player == 0)
			current_player += 1;
		else
			cout << "Player n's turn!                                        " << endl;
		
		tempstr = print_board();
		strtowrite = tempstr.c_str();
		cout << strtowrite << endl;

//		SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "1) Build Settlement                    " << endl;
		cout << "2) Build a roadroad                    " << endl;
		cout << "3) Trade cards naow                    " << endl;
		cout << "4) Check resources!                    " << endl;
		cout << "5) Roll dice dice!!                    " << endl;
		cout << "6) Goto next player                    " << endl;
		cout << "7) Quit, llama duck                    " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
		cout << "                                                                     " << endl;
	//	cin.clear();
	//	SetConsoleCursorPosition(console_handle, dwPosition2);	
		cin.clear();
		cin.ignore(0);
		user_input = -99;
		cin >> user_input;

		switch(user_input)
		{
		case 1: 			
//			SetConsoleCursorPosition(console_handle, dwPosition1);
			cout << "                                                                     " << endl;
			cout << "Which hex?                                    " << endl;
			cin >> user_input2;
			user_input2 = user_input2 % 19;		//force a valid number.
			cout << "Which corner?                                 " << endl;
			cout << "        1_______2                       " << endl;
			cout << "        /       \\                       " << endl;
			cout << "     0 /         \\ 3                    " << endl;
			cout << "       \\         /                      " << endl;
			cout << "      5 \\_______/ 4                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cin >> user_input3;
			user_input3 = user_input3 % 6;		//force a valid number
			temp = client_build_settlement(user_input2, user_input3);
			temp = framehandler(catan, rxdatabuff, sizerxbuff);
			temp = clientFrameHandler(txdatabuff);
			//			temp = catan.build_settlement(user_input2, current_player, user_input3);
			if(temp < 0)	//if error, print message!
				cout << "ERROR: unable to build settlement!" << endl;
			break;
		case 2:			
//			SetConsoleCursorPosition(console_handle, dwPosition1);
			cout << "                                                                     " << endl;
			cout << "Which hex?                                    " << endl;
			cin >> user_input2;
			user_input2 = user_input2 % 19;		//force a valid number.
			cout << "Which road?                                   " << endl;
			cout << "         ___1____   " << endl;
			cout << "        /       \\   " << endl;
			cout << "     0 /         \\ 2" << endl;
			cout << "      /           \\ " << endl;
			cout << "      \\           / " << endl;
			cout << "     5 \\         / 3" << endl;
			cout << "        \\___4___/   \n\n\n\n\n\n" << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cout << "                                                                     " << endl;
			cin >> user_input3;
			user_input3 = user_input3 % 6;		//force a valid number
			temp = client_build_road(user_input2, user_input3);
			temp = framehandler(catan, rxdatabuff, sizerxbuff);
			temp = clientFrameHandler(txdatabuff);
			//temp = catan.build_roads(user_input2, current_player, user_input3);
			if(temp < 0)	//if error, print message!
				cout << "ERROR: unable to build road!" << endl;
			else if (user_input3 == (temp - 1))
			{
				tempstrtowrite = std::stringstream();
				tempstrtowrite << "Road " << user_input3 << "successfully built on tile " << setw(2) << user_input2 << endl;
				cout << tempstrtowrite.str() << endl;
			}
			break;

		case 3:
			cout << "Trading has not been implemented yet!" << endl;
			break;
		case 4:
//			SetConsoleCursorPosition(console_handle, dwPosition1);	
//			SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			for(int i = 1; i < 6; i++)
			{
				if(i == 1)
					cout << "Resource type: Wheat" << endl;
				else if(i == 2)
					cout << "Resource type: Ore" << endl;
				else if(i == 3)
					cout << "Resource type: Wood" << endl;
				else if(i == 4)
					cout << "Resource type: Sheep" << endl;
				else
					cout << "Resource type: Brick" << endl;
//				temp = catan.check_resources(current_player, i);
//				tempstrtowrite << "Resource Amount: " << temp << endl;
				tempstrtowrite << "Resource Amount: " << setw(2) << catan.check_resources(current_player, i) << endl;
				tempstrtowrite2 = tempstrtowrite.str().c_str();
				cout << tempstrtowrite.str() << endl;
				tempstrtowrite = std::stringstream();
			}			
			cout << "Press enter when finished" << endl;
			cin.ignore();
			cin >> user_input3;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			break;
		case 5:
//			SetConsoleCursorPosition(console_handle, dwPosition2);
			rollnumvalamt = (rand()*rand()) % 11 + 2;
			temp = catan.start_turn(rollnumvalamt);
			tempstrtowrite << "Dice roll: " << temp << "   " << endl;
			tempstrtowrite << "Return value: " << temp << "   " << endl;
			cout << tempstrtowrite.str() << endl;
			tempstrtowrite = std::stringstream();
			if (temp == 7)
			{
				tempstrtowrite << "A 7 has been rolled, what tile do you want to place the robber on?" << endl;
				cout << tempstrtowrite.str() << endl;
				cin >> user_input3;
				temp = catan.update_robber_position(user_input3 % active_num_tiles);
				tempstrtowrite = std::stringstream();
				if (temp > 0)
					tempstrtowrite << "Robber placed on tile " << temp << endl;
				else
					tempstrtowrite << "Robber was not placed!!" << endl;
				cout << tempstrtowrite.str() << endl;
				tempstrtowrite = std::stringstream();
			}
			cin.clear();
			cin.ignore(INT_MAX, '\n');
//			tempstrtowrite.clear();
			break;
		case 6:
			current_player = (current_player + 1)%(number_of_players + 1);
			start_turn();
			break;
		case 7:
			exit(1);
			break;
		default:
			cin.clear();
			break;
		}
		user_input = NULL;
//		SetConsoleCursorPosition(console_handle, dwPosition);	
		tempstrtowrite = std::stringstream();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}
		
	//	Sleep(30000);
	return(12);
}

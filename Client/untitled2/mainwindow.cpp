#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QPixmap>
#include <QIcon>
#include <QWidget>
#include <QtGui>
#include <string>
#include <vector>
#include <stdio.h>
#include "clientTransmitHandler.h"
#include "gameClient.h"
#include <QPushButton>
#include <string>
#include <QList>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>

#define BRICK_ICON      "C:/Users/gtb/Documents/GitHub/Brick.png"
#define SHEEP_ICON      "C:/Users/gtb/Documents/GitHub/Sheep.png"
#define FOREST_ICON     "C:/Users/gtb/Documents/GitHub/forest.png"
#define MOUNTAIN_ICON   "C:/Users/gtb/Documents/GitHub/Mountain.png"
#define WHEAT_ICON      "C:/Users/gtb/Documents/GitHub/Wheat.png"
#define DESERT_ICON     "C:/Users/gtb/Documents/GitHub/Desert.png"

#define NO_PLAYER_COLOR "background-color: rgb(175,175,175)"
#define PLAYER_1_COLOR  "background-color: rgb(0,255,0)"
#define PLAYER_2_COLOR  "background-color: rgb(0,0,255)"
#define PLAYER_3_COLOR  "background-color: rgb(255,0,0)"
#define PLAYER_4_COLOR  "background-color: rgb(255,255,255)"

#define Xicon_size  150
#define Yicon_size  150
#define DICE_RADIUS 45

static QPixmap* brickpic;
static QPixmap* wheatpic;
static QPixmap* mountainpic;
static QPixmap* sheeppic;
static QPixmap* forestpic;
static QPixmap* desertpic;

int devcardflag = 0;
int dv_build_roads_flag = 0;
int dv_play_knight_flag = 0;
int dv_yop_flag = 0;
int dv_monopoly_flag = 0;
int dv_victory_point_flag = 0;
int game_started = 0;
int request_user_place_robber = 0;
int waiting_on_robber = 0;
string playercolors[5] = {NO_PLAYER_COLOR, PLAYER_1_COLOR, PLAYER_2_COLOR, PLAYER_3_COLOR, PLAYER_4_COLOR};
/*
 *TODO:
 * need to make a function to 'paint' the board info... it needs to update color of buttons based on who owns them!
 */
static int update_board_icons = 0;
char rxdatabuff [4096];
const int road_array_tile_num[72] = {0,0,0,0,0,0,3,3,3,3,3,7,7,7,7,7,12,12,12,12,12,16,16,16,16,16,17,17,17,17,17,18,18,18,18,18,15,15,15,15,15,11,11,11,11,11,6,6,6,6,6,2,2,2,2,2,10,10,10,5,5,5,1,1,1,13,13,9,8,8,4,4};
const int road_array_road_num[72] = {0,1,2,3,4,5,0,1,2,3,4,0,1,2,3,4,0,1,2,3,5,5,0,1,2,3,0,1,2,4,5,0,1,2,4,5,0,1,3,4,5,0,1,3,4,5,0,2,3,4,5,0,2,3,4,5,2,3,4,2,3,4,2,4,5,3,4,2,1,0,1,2};
const int settlement_array_tile_num[54] = {0,0,0,0,0,0,3,3,3,3,7,7,7,7,12,12,12,12,16,16,16,16,17,17,17,17,18,18,18,18,15,15,15,15,11,11,11,11,6,6,6,6,2,2,2,2,1,1,4,9,9,9,9,9};
const int settlement_array_corner_num[54] = {0,1,2,3,4,5,1,2,3,4,1,2,3,4,0,1,2,3,0,1,2,3,0,1,2,5,5,0,1,2,1,4,5,0,1,0,5,4,3,0,5,4,0,3,4,5,5,2,1,0,1,2,3,4};
const int tile_mapping_array[19] = {9,5,4,8,3,1,0,2,6,10,13,14,12,7,16,17,11,15,18};
QList<QPushButton*> buttonlist;
/*
int get_tile_resource(int tile_num, QString& file)
{
    int tempnum = tile_num%7;
    switch(tempnum)
    {
    case 0:
        file = BRICK_ICON;
        break;
    case 1:
        file = BRICK_ICON;
        break;
    case 2:
        file = SHEEP_ICON;
        break;
    case 3:
        file = FOREST_ICON;
        break;
    case 4:
        file = MOUNTAIN_ICON;
        break;
    case 5:
        file = WHEAT_ICON;
        break;
    case 6:
        file = DESERT_ICON;
        break;
    default:
        break;
    }
    return(1);
}
*/

void get_settlement_corner_and_tile_from_name(std::string  name, int& tile, int& corner)
{
    int switchval = 0;
    name.substr(1, name.length()-1);
    char temp1[1] = {name[4]};
    switchval = atoi(temp1);
    if(name.length() == 6)
    {
        temp1[0] = name[5];
        switchval = switchval*10 + atoi(temp1);
    }
//    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
//    std::cout << "switch val: " << switchval << std::endl;
    tile = settlement_array_tile_num[(switchval-1)%54];
    corner = settlement_array_corner_num[(switchval-1)%54];
}
//tcpclient clienttcp;

void MainWindow::get_icon_file_rsrc_type_and_roll_from_tile_num(QString &filename, int &resourcetype, int &roll, int tile)
{
    roll = Cgame.check_tile_dice_roll(tile);
    resourcetype = Cgame.check_resource(tile);
    switch(resourcetype)
    {
    case 1: //Wheat
        filename = WHEAT_ICON;
        break;
    case 2: //Ore
        filename = MOUNTAIN_ICON;
        break;
    case 3: //Wood
        filename = FOREST_ICON;
        break;
    case 4: //Sheep
        filename = SHEEP_ICON;
        break;
    case 5: //Brick
        filename = BRICK_ICON;
        break;
    case 6: //Desert
        filename = DESERT_ICON;
        break;
    default:
        filename = DESERT_ICON;
        if(debug_text)
            std::cout << "Invalid resource type in get_icon_file_rsrc_type_and_roll_from_tile_num(...)" << std::endl;
    }
}

void MainWindow::set_icons_and_rollvals_on_board()
{
/*    static QPixmap brickpic(BRICK_ICON);
    static QPixmap wheatpic(WHEAT_ICON);
    static QPixmap mountainpic(MOUNTAIN_ICON);
    static QPixmap sheeppic(SHEEP_ICON);
    static QPixmap forestpic(FOREST_ICON);
    static QPixmap desertpic(DESERT_ICON);
*/
    char* charptr;
    int resrc = 0;
    int roll = 0;
    int temp = 0;
    int temp2 = 0;
    char tempchar[1];
    string tempstr;
    QString qstr;
    QString folderpath;// = QDir::currentPath();
    QString iconpath;
    QSize size(Xicon_size, Yicon_size);
    for(int x = 0; x < buttonlist.size(); x++)
    {
        //folderpath = QDir::currentPath();
        tempstr = buttonlist[x]->objectName().toStdString();
        charptr = new char[tempstr.length()+1];
        strcpy(charptr, tempstr.c_str());
        if(charptr[0] == 't')   //if a tile object, then get the number of the object name
        {
            tempchar[0] = charptr[1];
            temp = atoi(tempchar);
            if(buttonlist[x]->objectName().length() == 3)   //if 3 chars long, then it will be
            {
                tempchar[0] = charptr[2];
                temp2 = atoi(tempchar);
                temp = temp*10;
                temp += temp2;
            }
            temp2 = tile_mapping_array[(temp-1)%19];        //real tile number.
            get_icon_file_rsrc_type_and_roll_from_tile_num(qstr, resrc, roll, temp2);
            folderpath.append(qstr);
            switch(resrc)
                {
                case 1: //Wheat
                buttonlist[x]->setIcon(*wheatpic);
                    break;
                case 2: //Ore
                buttonlist[x]->setIcon(*mountainpic);
                    break;
                case 3: //Wood
                buttonlist[x]->setIcon(*forestpic);
                    break;
                case 4: //Sheep
                buttonlist[x]->setIcon(*sheeppic);
                    break;
                case 5: //Brick
                buttonlist[x]->setIcon(*brickpic);
                    break;
                case 6: //Desert
                buttonlist[x]->setIcon(*desertpic);
                    break;
                default:
                buttonlist[x]->setIcon(*desertpic);
                    if(debug_text)
                        std::cout << "Invalid resource type in get_icon_file_rsrc_type_and_roll_from_tile_num(...)" << std::endl;
                }

            std::cout << "File path to set icon to: " << folderpath.toStdString() << std::endl;
//            QPixmap temppix(folderpath);//
//            temppix.load(folderpath);
//            buttonlist[x]->setIcon(temppix);
            buttonlist[x]->setIconSize(size);
            folderpath.clear();
                    //    get_tile_resource(3, tempstr);
                    //    temppix.load(tempstr);
                    //    ui->tile1_4->setIcon(temppix);
                    //    ui->tile1_4->setIconSize(size);
        }
        else if(charptr[0] == 'D')   //if a tile object, then get the number of the object name
        {
            setDiceRoll(buttonlist[x]->objectName().toStdString(), buttonlist[x]);
        }
    }
}

void get_road_road_and_tile_from_name(std::string name, int& tile, int& road)
{
    int switchval = 0;
    name.substr(1, name.length()-1);
    char temp1[1] = {name[5]};
    switchval = atoi(temp1);
    if(name.length() == 7)
    {
        temp1[0] = name[6];
        switchval = switchval*10 + atoi(temp1);
    }
//    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
//    std::cout << "switch val: " << switchval << std::endl;
    tile = road_array_tile_num[(switchval-1)%72];
    road = road_array_road_num[(switchval-1)%72];
}

int get_tile_num_from_tile_name(std::string name)
{
//    int retval = -1;
    int switchval = 0;
    name.substr(1, name.length()-1);
    char temp1[1] = {name[1]};
    switchval = atoi(temp1);
    if(name.length() == 3)
    {
        temp1[0] = name[2];
        switchval = switchval*10 + atoi(temp1);
    }
//    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
//    std::cout << "switch val: " << switchval << std::endl;
    return(tile_mapping_array[(switchval-1) % 19]);
}

static SOCKET ClientSock = INVALID_SOCKET;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString tempstr = "";
    ui->setupUi(this);
    buttonlist = this->findChildren<QPushButton *> ();
    std::cout << "# buttons found: " << buttonlist.size() << std::endl;

    brickpic = new QPixmap(BRICK_ICON);
    wheatpic = new QPixmap(WHEAT_ICON);
    mountainpic = new QPixmap(MOUNTAIN_ICON);
    sheeppic = new QPixmap(SHEEP_ICON);
    forestpic = new QPixmap(FOREST_ICON);
    desertpic = new QPixmap(DESERT_ICON);

/*    brickpic = QPixmap(BRICK_ICON);
    wheatpic = QPixmap(WHEAT_ICON);
    mountainpic = QPixmap(MOUNTAIN_ICON);
    sheeppic = QPixmap(SHEEP_ICON);
    forestpic = QPixmap(FOREST_ICON);
    desertpic = QPixmap(DESERT_ICON);
*/
    for(int x = 0; x < buttonlist.size(); x++)
        connect(buttonlist[x],SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    ClientSock = Cgame.initsocketthing();
    ui->tile1_1->setObjectName("t1");
    ui->tile1_2->setObjectName("t2");
    ui->tile1_3->setObjectName("t3");
    ui->tile1_4->setObjectName("t4");
    ui->tile1_5->setObjectName("t5");
    ui->tile1_6->setObjectName("t6");
    ui->tile1_7->setObjectName("t7");
    ui->tile1_8->setObjectName("t8");
    ui->tile1_9->setObjectName("t9");
    ui->tile1_10->setObjectName("t10");
    ui->tile1_11->setObjectName("t11");
    ui->tile1_12->setObjectName("t12");
    ui->tile1_13->setObjectName("t13");
    ui->tile1_14->setObjectName("t14");
    ui->tile1_15->setObjectName("t15");
    ui->tile1_16->setObjectName("t16");
    ui->tile1_17->setObjectName("t17");
    ui->tile1_18->setObjectName("t18");
    ui->tile1_19->setObjectName("t19");
//    button->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    connect(ui->tile1_1, SIGNAL(released()), this, SLOT(on_pushButton_released()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_rx_packet()));
    timer->start(3000);
    Cgame.joinGame();
    clientFrameHandler(Cgame, rxdatabuff);
}

void MainWindow::check_rx_packet()
{
    static int busyflag = 0;
    timeval timeout;
    int tempnum = 0;
    fd_set readSet;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    FD_ZERO(&readSet);
    FD_SET(ClientSock, &readSet);
    int retval = -1;
    if(!busyflag)
    {
        busyflag = 1;
        if (select(ClientSock, &readSet, NULL, NULL, &timeout) == 1)
        {
            retval = recv(ClientSock, rxdatabuff, 4096, 0);
            if (retval > 0)
            {
                check_rx_data_buff = 1;
                for(int x = 0; x < clienttcp.get_rxbuffsize(); x++)
                    rxdatabuff[x] = clienttcp.read_receive_buff(x);
                clientFrameHandler(Cgame, rxdatabuff);
                if(game_started == 0)   //if game hasnt been started, but we get a start game packet, then request info!
                {
                   tempnum = last_packet_sent(0);
                   if(tempnum == START_GAME)
                   {
                       if(debug_text)
                           std::cout << "Game has been started by host!" << std::endl;
                       Cgame.get_board_info();
                       retval = clientFrameHandler(Cgame, rxdatabuff);
                       updateboardcolors = 1;
                       set_icons_and_rollvals_on_board();
                       updatecolortile = 0;
//                       check_packet_and_update(0);
                   }
                }
            }
        }
        //also check for flags in this?
        Check_and_process_flags();
        update_resources_display();
        update_board_colors();
        busyflag = 0;
    }
/*    if(!busyflag)
    {
        busyflag = 1;
        retval = clienttcp.recieveSingle();
        if(retval > 0)       //if we received data, then process data!
        {
            check_rx_data_buff = 1;
            for(int x = 0; x < clienttcp.get_rxbuffsize(); x++)
                rxdatabuff[x] = clienttcp.read_receive_buff(x);
            clientFrameHandler(Cgame, rxdatabuff);
        }
        busyflag = 0;
    }
*/
}

int MainWindow::Check_and_process_flags()
{
    /*
int devcardflag = 0;
int dv_build_roads_flag = 0;
int dv_play_knight_flag = 0;
int dv_yop_flag = 0;
int dv_monopoly_flag = 0;
int dv_victory_point_flag = 0;
int game_started = 0;
int request_user_place_robber = 0;
     */
    if(request_user_place_robber)
    {
        request_user_place_robber = 0;
        QMessageBox msgBox;
        msgBox.setText("Select a tile to place the robber on!");
        msgBox.setInformativeText("To use dev card, press 'ok'. Otherwise press 'cancel'");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        ui->NotifyText->setText("Select tile to place robber on");
        waiting_on_robber = 1;
        int ret = msgBox.exec();
    }
    return(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    exit(2);
}

void MainWindow::update_resources_display()
{
    ui->card_W->display(Cgame.check_player_resource_amt(1));
    ui->card_O->display(Cgame.check_player_resource_amt(2));
    ui->card_F->display(Cgame.check_player_resource_amt(3));
    ui->card_S->display(Cgame.check_player_resource_amt(4));
    ui->card_B->display(Cgame.check_player_resource_amt(5));
    ui->CURR_DICE_ROLL->display(Cgame.Get_dice_roll());
            //1 wheat
            //2 ore
            //3 wood
            //4 sheep
            //5 brick
            //6 desert

}
void MainWindow::setdicerolls()
{
    for(int i = 0; i < buttonlist.size(); i++)
    {
        if(buttonlist[i]->objectName().toStdString()[0] == 'D')
            setDiceRoll(buttonlist[i]->objectName().toStdString(), buttonlist[i]);
    }
}

void MainWindow::setDiceRoll(std::string nname, QPushButton *ptr)
{
    std::stringstream convert;
    int retval = 0;
    std::string tempstr = "";
    //DICEROLL_ = 9 lenght, if 11, then its 2 digits.
    int length = nname.length();
    char tempchar[1] = {nname[9]};
    int tilenum = atoi(tempchar);
    if(length == 11)
    {
        tempchar[0] = nname[10];
        tilenum = tilenum*10 + atoi(tempchar);
    }
    tilenum -= 1;
    retval = droll(tilenum);
    convert.flush();
    convert << retval;
    tempstr = convert.str();
    if(nname[0] == 'D')
    {
        switch(retval)
        {
        case 1:
            tempstr = "DESERT";
            break;
        case 2:
            tempstr += "\n*";
            break;
        case 3:
            tempstr += "\n**";
            break;
        case 4:
            tempstr += "\n***";
            break;
        case 5:
            tempstr += "\n****";
            break;
        case 6:
            tempstr += "\n*****";
            break;
        case 8:
            tempstr += "\n*****";
            break;
        case 9:
            tempstr += "\n****";
            break;
        case 10:
            tempstr += "\n***";
            break;
        case 11:
            tempstr += "\n**";
            break;
        case 12:
            tempstr += "\n*";
            break;
        default:
            break;
        }
        if(tempstr == "DESERT")     //if its the desert tile, then
        {
            ptr->hide();
        }
        else if(ptr->text().toStdString() != tempstr)
        {
            if(ptr->isHidden())
                ptr->show();
            QString qstr = QString::fromStdString(tempstr);
            ptr->setText(qstr);
            ptr->setFixedHeight(DICE_RADIUS);
            ptr->setFixedWidth(DICE_RADIUS);
            QRect *rect = new QRect(0,0,DICE_RADIUS,DICE_RADIUS);
     //       qDebug() << rect->size();
     //       qDebug() << ptr->size();
            QRegion* region = new QRegion(*rect, QRegion::Ellipse);
     //       qDebug() << region->boundingRect().size();
            ptr->setMask(*region);
            ptr->setStyleSheet("color: white;background-color: rgba(255,255,255,125);");
        }
    }
}

void MainWindow::set_button_color(std::string nname, QPushButton *ptr)
{
    int tile = 0;
    QString tempstr = "";
    int road = 0;
    int retval = 0;
    if(nname[0] == 'r')  //if road, then find the road info!
    {
        get_road_road_and_tile_from_name(nname, tile, road);
        retval = Cgame.get_road_owner(road, tile);
        if((retval > 0) || (ptr->styleSheet().toStdString() != playercolors[retval])) // && (retval < playercolors.size()))
        {
            tempstr = QString::fromUtf8(playercolors[retval].c_str());
            ptr->setStyleSheet(tempstr);
        }
    }
    else if(nname[0] == 's')
    {
        get_settlement_corner_and_tile_from_name(nname, tile, road);
        retval = Cgame.get_corner_owner(tile, road);
        if((retval > 0) || (ptr->styleSheet().toStdString() != playercolors[retval]))// && (retval < playercolors.size()))
        {
            tempstr = QString::fromUtf8(playercolors[retval].c_str());
            ptr->setStyleSheet(tempstr);
        }
    }
//    else if(nname[0] == 'D')
//        setDiceRoll(nname, ptr);
//    else
//        std::cout << "ERRRRROORRRR Invalid button!" << std::endl;

}

int MainWindow::update_board_colors()
{
    for(int i = 0; i < buttonlist.size(); i++)
        set_button_color(buttonlist[i]->objectName().toStdString(), buttonlist[i]);
    return(1);
}


int MainWindow::corner_info(int corner, int tilenum)
{
    int retval = 0;
    retval = (10 * Cgame.check_corner_owner(corner, tilenum) + Cgame.check_corner_building_type(corner, tilenum));
    return(retval);
}

int MainWindow::droll(int tilenumb)
{
    return(Cgame.get_dice_roll(tilenumb));
}

string MainWindow::print_board()
{
    cout << "Update board from server or client data?" << endl << "1) Server data" << endl << "2) Client data" << endl;

    std::stringstream strStream;
    strStream.flush();

//    strStream = std::stringstream();	//flush string stream...
//	strStream.open("tempfile.secret");
    strStream << setw(setwval) << "                           /" << setw(2) << corner_info(cornB,18) << "'''''" << setw(2) << corner_info(cornC, 18) << "\\                         " << endl;
    strStream << setw(setwval) << "                          /" << setw(2) << corner_info(cornA, 18) << " |" << setw(2) << droll(18) << "|" << setw(2) << corner_info(cornD, 18) << "\\               " << endl;
    strStream << setw(setwval) << "                 /" << setw(2) << corner_info(cornB,15) << "'''''" << setw(2) << corner_info(cornC, 15) << "\\\\    18   //" << setw(2) << corner_info(cornB, 17) << "'''''" <<  setw(2) << corner_info(cornC, 17) << "\\               " << endl;
    strStream << setw(setwval) << "                /" << setw(2) << corner_info(cornA, 15) << " |" << setw(2) << droll(15) << "|" << setw(2) << corner_info(cornD, 15) << "\\\\" << setw(2) << corner_info(cornF, 18) << "____" << setw(2) << corner_info(cornE, 18) << "//" << setw(2) << corner_info(cornA, 17) << " |" << setw(2) << droll(17) << "|" << setw(2) << corner_info(cornD, 17) << "\\                  " << endl;				//		A-C-E Navigation matrix:
    strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,11) << "'''''" << setw(2) << corner_info(cornC, 11) << "\\\\   15    //" << setw(2) << corner_info(cornB, 14) << "'''''" <<  setw(2) << corner_info(cornC, 14) << "\\\\   17    //" <<  setw(2) << corner_info(cornB,16) << "'''''" << setw(2) << corner_info(cornC, 16) << "\\          " << endl;
    //				/''''''''''¯\\	  15   //''''''''''¯\\	  17   //''''''''''¯\" << endl;		//		A = (0,2)
    strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,11) << " |" << setw(2) << droll(11) << "|" << setw(2) << corner_info(cornD, 11) << "\\\\" << setw(2) << corner_info(cornF, 15) << "____" << setw(2) << corner_info(cornE, 15) << "//" << setw(2) << corner_info(cornA, 14) << " |" << setw(2) << droll(14) << "|" <<  setw(2) << corner_info(cornD, 14) << "\\\\" << setw(2) << corner_info(cornF, 17) << "____" << setw(2) << corner_info(cornE, 17) << "//" <<  setw(2) << corner_info(cornA,16) << " |" << setw(2) << droll(16) << "|" << setw(2) << corner_info(cornD, 16) << "\\        " << endl;
    //			   /  (2,4)	 \\_________//  (3,3)	 \\_________//  (4,2)	 \				C = (1,2)
    strStream << setw(setwval) << "      \\    11   //" << setw(2) << corner_info(cornB, 10) << "'''''" << setw(2) << corner_info(cornC, 10) << "\\\\    14   //" <<setw(2) << corner_info(cornB, 13) << "'''''" << setw(2) << corner_info(cornC, 13) << "\\\\   16    /          " << endl;
        //		   \   11	 //''''''''''¯\\	14	 //''''''''''¯\\   16	 /" << endl;	//		E = (0,1)
    strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 11) << "____" << setw(2) << corner_info(cornE, 11) << "//" << setw(2) << corner_info(cornA, 10) << " |" << setw(2) << droll(10) << "|" << setw(2) << corner_info(cornD, 10) << "\\\\" << setw(2) << corner_info(cornF, 14) << "____" << setw(2) << corner_info(cornE, 14) << "//" << setw(2) << corner_info(cornA, 13) << " |" << setw(2) << droll(13) << "|" << setw(2) << corner_info(cornD, 13) << "\\\\" << setw(2) << corner_info(cornF, 16) << "____" << setw(2) << corner_info(cornE, 16) << "/         " << endl;
    //				\_________//	(2,3)  \\_________//	(3,2)  \\_________/" << endl;
    strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,6) << "'''''" << setw(2) << corner_info(cornC, 6) << "\\\\    10   //" << setw(2) << corner_info(cornB, 9) << "'''''" <<  setw(2) << corner_info(cornC, 9) << "\\\\    13   //" <<  setw(2) << corner_info(cornB,12) << "'''''" << setw(2) << corner_info(cornC, 12) << "\\     " << endl;
    //				/''''''''''¯\\	 10	   //''''''''''¯\\	 13    //''''''''''¯\'" << endl;
    strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,6) << " |" << setw(2) << droll(6) << "|" << setw(2) << corner_info(cornD, 6) << "\\\\" << setw(2) << corner_info(cornF, 5) << "____" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 9) << " |" << setw(2) << droll(9) << "|" <<  setw(2) << corner_info(cornD, 9) << "\\\\" << setw(2) << corner_info(cornF, 13) << "____" << setw(2) << corner_info(cornE, 13) << "//" <<  setw(2) << corner_info(cornA,12) << " |" << setw(2) << droll(12) << "|" << setw(2) << corner_info(cornD, 12) << "\\     " << endl;
    //			   /  (1,3)	 \\_________//  (2,2)	 \\_________//  (3,1)	 \'" << endl;	//		B-D-F Navigation matrix:
    strStream << setw(setwval) << "      \\    6    //" << setw(2) << corner_info(cornB, 5) << "'''''" << setw(2) << corner_info(cornC, 5) << "\\\\    9    //" <<setw(2) << corner_info(cornB, 8) << "'''''" << setw(2) << corner_info(cornC, 8) << "\\\\   12    /     " << endl;
    //			   \	6	 //''''''''''¯\\	9	 //''''''''''¯\\	12	 /" << endl;	//		B = (2,2)
    strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 6) << "____" << setw(2) << corner_info(cornE, 6) << "//" << setw(2) << corner_info(cornA, 5) << " |" << setw(2) << droll(5) << "|" << setw(2) << corner_info(cornD, 5) << "\\\\" << setw(2) << corner_info(cornF, 9) << "____" << setw(2) << corner_info(cornE, 9) << "//" << setw(2) << corner_info(cornA, 8) << " |" << setw(2) << droll(8) << "|" << setw(2) << corner_info(cornD, 8) << "\\\\" << setw(2) << corner_info(cornF, 12) << "____" << setw(2) << corner_info(cornE, 12) << "/     " << endl;
    //			    \_________//	(1,2)  \\_________//	(2,1)  \\_________/" << endl;		//		D = (2,1)
    strStream << setw(setwval) << "       /" <<  setw(2) << corner_info(cornB,2) << "'''''" << setw(2) << corner_info(cornC, 2) << "\\\\    5    //" << setw(2) << corner_info(cornB, 4) << "'''''" <<  setw(2) << corner_info(cornC, 4) << "\\\\    8    //" <<  setw(2) << corner_info(cornB,7) << "'''''" << setw(2) << corner_info(cornC, 7) << "\\     " << endl;
    //				/''''''''''¯\\	  5	   //''''''''''¯\\	  8	   //''''''''''¯\'" << endl;		//		F = (1,1)
    strStream << setw(setwval) << "      /" <<  setw(2) << corner_info(cornA,2) << " |" << setw(2) << droll(2) << "|" << setw(2) << corner_info(cornD, 2) << "\\\\" << setw(2) << corner_info(cornF, 5) << "____" << setw(2) << corner_info(cornE, 5) << "//" << setw(2) << corner_info(cornA, 4) << " |" << setw(2) << droll(4) << "|" <<  setw(2) << corner_info(cornD, 4) << "\\\\" << setw(2) << corner_info(cornF, 8) << "____" << setw(2) << corner_info(cornE, 8) << "//" <<  setw(2) << corner_info(cornA,7) << " |" << setw(2) << droll(7) << "|" << setw(2) << corner_info(cornD, 7) << "\\     " << endl;
    //			   /  (0,2)	 \\_________//  (1,1)	 \\_________//  (2,0)	 \'" << endl;
    strStream << setw(setwval) << "      \\    2    //" << setw(2) << corner_info(cornB, 1) << "'''''" << setw(2) << corner_info(cornC, 1) << "\\\\    4    //" <<setw(2) << corner_info(cornB, 3) << "'''''" << setw(2) << corner_info(cornC, 3) << "\\\\    7    /     " << endl;
    //			   \	2	 //''''''''''¯\\    4    //''''''''''¯\\	7	 /" << endl;
    strStream << setw(setwval) << "       \\" << setw(2) << corner_info(cornF, 2) << "____" << setw(2) << corner_info(cornE, 2) << "//" << setw(2) << corner_info(cornA, 1) << " |" << setw(2) << droll(1) << "|" << setw(2) << corner_info(cornD, 1) << "\\\\" << setw(2) << corner_info(cornF, 4) << "____" << setw(2) << corner_info(cornE, 4) << "//" << setw(2) << corner_info(cornA, 3) << " |" << setw(2) << droll(3) << "|" << setw(2) << corner_info(cornD, 3) << "\\\\" << setw(2) << corner_info(cornF, 7) << "____" << setw(2) << corner_info(cornE, 7) << "/     " << endl;
    //			    \_________//	(0,1)  \\_________//	(1,0)  \\_________/" << endl;
    strStream << setw(setwval) << "                \\    1    //" <<  setw(2) << corner_info(cornB, 0) << "'''''" <<  setw(2) << corner_info(cornC, 0) << "\\\\    3    /" << endl;
    //						 \    1    //''''''''''¯\\	  3    /" << endl;
    strStream << setw(setwval) << "                 \\" << setw(2) << corner_info(cornF, 1) << "____" << setw(2) << corner_info(cornE, 1) << "//" << setw(2) << corner_info(cornA, 0) << " |" << setw(2) << droll(0) << "|" << setw(2) << corner_info(cornD, 0) << "\\\\" << setw(2) << corner_info(cornF, 3) << "____" << setw(2) << corner_info(cornE, 3) << "/     " << endl;
    //						  \_________//  (0,0)	 \\_________/" << endl;
    strStream << setw(setwval) << "                          \\    0    /     " << endl;

    strStream << setw(setwval) << "                           \\" << setw(2) << corner_info(cornF, 0) << "____" << setw(2) << corner_info(cornE, 0) << "/" << endl;
    //									\_________/" << endl;
//	filein.open("tempfile.secret");
//	filein.rdbuf() >> s;
//	strStream << filein.rdbuf();
    return(strStream.str());
}
void MainWindow::update_dev_cards_on_gui()
{
    ui->card_DK->display(Cgame.get_qty_dv_cardd(1));
    ui->card_DV->display(Cgame.get_qty_dv_cardd(2));
    ui->card_DY->display(Cgame.get_qty_dv_cardd(3));
    ui->card_DM->display(Cgame.get_qty_dv_cardd(4));
    ui->card_DR->display(Cgame.get_qty_dv_cardd(5));
}

void MainWindow::on_pushButton_clicked()
{
    int retval = -1;
    static int i = 0;
    int tile = 0;
    int corner = 0;
    string tempstr;
    const char* strtowrite;
    vector<int> tempvec;
    int road = 0;
    i = 0;
    QPushButton *ptrobj = qobject_cast<QPushButton *>(sender());
    QObject *senderObj = sender(); // This will give Sender object
    // This will give obejct name for above it will give "A", "B", "C"
    QString senderObjName = senderObj->objectName();
//    QPixmap tempp("C:/Users/gtb/Documents/GitHub/Brick_pressed.png");
//    senderObjName.toStdString().c_str()[0];
    if((senderObjName.toStdString().length() == 2) || (senderObjName.toStdString().length() == 3))
    {
        retval = get_tile_num_from_tile_name(senderObjName.toStdString());
        std::cout << "Tile pressed: " << retval << std::endl;
//        ptrobj->setIcon(tempp);
        if(waiting_on_robber)
        {
            ui->NotifyText->setText(" ");
            ui->NotifyText->displayText();
            tempvec = Cgame.check_players_on_tile(retval);
            if(tempvec.size() == 0) //if empty, either no one was on tile or only this player was.
            {
                if(debug_text)
                    std::cout << "No one available to steal from!" << std::endl;
            }
            else
            {
                tile = retval;
                std::cout << "Make onbutton press handle selecting who to steal from when placing robber!" << std::endl;
                retval = tempvec[0];
                Cgame.place_robber(tile, retval);
                clientFrameHandler(Cgame, rxdatabuff);
                update_resources_display();
            }
            waiting_on_robber = 0;
        }
    }
    else if(waiting_on_robber == 0)
    {
        if(senderObjName.toStdString()[0] == 'B')//if one of the control buttons was pressed (all start with B), process it here
        {
            if(senderObjName.toStdString() == "B_START_GAME")
            {
                std::cout << "Start game!" << std::endl;
                game_started = 1;
                Cgame.startGame();
                retval = clientFrameHandler(Cgame, rxdatabuff);
                Cgame.get_board_info();
                retval = clientFrameHandler(Cgame, rxdatabuff);
                updateboardcolors = 1;
                set_icons_and_rollvals_on_board();
            }
            else if(!game_started)
            {
                std::cout << " " << std::endl;
            }
            else if (senderObjName.toStdString() == "BUILD_CITY")
            {
                std::cout << "Make me build a city!" << std::endl;
                ui->BUILD_A_ROAD->setChecked(FALSE);
                ui->BUILD_SET->setChecked(FALSE);
            }
            else if(senderObjName.toStdString() == "BUILD_A_ROAD")
            {
                std::cout << "Make me build a road!" << std::endl;
                ui->BUILD_CITY->setChecked(FALSE);
                ui->BUILD_SET->setChecked(FALSE);
            }
            else if(senderObjName.toStdString() == "BUILD_SET")
            {
                std::cout << "Make me build a settlement!" << std::endl;
                ui->BUILD_CITY->setChecked(FALSE);
                ui->BUILD_A_ROAD->setChecked(FALSE);
            }
            else if(senderObjName.toStdString() == "B_START_TURN")
            {
                std::cout << "Start my turn!" << std::endl;
                Cgame.start_turn();
            }
            else if(senderObjName.toStdString() == "B_CARDS_REF")
            {
                std::cout << "Refresh my cards!" << std::endl;
                Cgame.refresh_cards();
                update_resources_display();
            }
            else if(senderObjName.toStdString() == "B_DICE_ROLL")
            {
                std::cout << "Roll my dice!" << std::endl;
                retval = Cgame.Get_dice_roll();
            }
            else if(senderObjName.toStdString() == "B_END_TURN")
            {
                updateboardcolors = 1;
                std::cout << "End my turn!" << std::endl;
                Cgame.end_turn();
    //            set_icons_and_rollvals_on_board();
            }
            else if(senderObjName.toStdString() == "B_UPDATE_BOARD")
            {
                std::cout << "Update board!" << std::endl;
                Cgame.get_board_info();
                updateboardcolors = 1;
                check_rx_data_buff = 1;
                if(update_board_icons == 0)
                    update_board_icons = 1;
    //            set_icons_and_rollvals_on_board();
            }
            else if(senderObjName.toStdString() == "B_PRINT_BOARD")
            {
                tempstr = print_board();
                strtowrite = tempstr.c_str();
                cout << strtowrite << endl;
            }
            else if(senderObjName.toStdString() == "B_INIT_DICE_ROLLS")
            {
                setdicerolls();
            }
            else if(senderObjName.toStdString() == "B_card_DKB")
            {
                if(Cgame.get_qty_dv_cardd(1) > 0)
                {
                    const QString msg = "Do you want to use a Knight Development Card? You have " + Cgame.get_qty_dv_cardd(1);
                    devcardtouse = 1;
                    use_dev_card_generic(msg);
                }
            }
            else if(senderObjName.toStdString() == "B_card_DVB")
            {
                if(Cgame.get_qty_dv_cardd(2) > 0)
                {
                    const QString msg = "Do you want to use a Victory Point Development Card? You have " + Cgame.get_qty_dv_cardd(2);
                    devcardtouse = 1;
                    use_dev_card_generic(msg);
                }
            }
            else if(senderObjName.toStdString() == "B_card_DYB")
            {
                if(Cgame.get_qty_dv_cardd(3) > 0)
                {
                    const QString msg = "Do you want to use a Year of plenty Development Card? You have " + Cgame.get_qty_dv_cardd(3);
                    devcardtouse = 1;
                    use_dev_card_generic(msg);
                }
            }
            else if(senderObjName.toStdString() == "B_card_DMB")
            {
                if(Cgame.get_qty_dv_cardd(4) > 0)
                    {
                    const QString msg = "Do you want to use a Monopoly Development Card? You have " + Cgame.get_qty_dv_cardd(4);
                    devcardtouse = 1;
                    use_dev_card_generic(msg);
                }
            }
            else if(senderObjName.toStdString() == "B_card_DRB")
            {
                if(Cgame.get_qty_dv_cardd(5) > 0)
                {
                    const QString msg = "Do you want to use a Build Roads Development Card? You have " + Cgame.get_qty_dv_cardd(5);
                    devcardtouse = 1;
                    use_dev_card_generic(msg);
                }
            }
            else if(senderObjName.toStdString() == "B_BUY_DEV_CARDS")
            {
                bool okayflag = FALSE;
                int numcards = QInputDialog::getInt(this, tr("Purchase Development Card"),
                                                    tr("Qty. development cards to buy:"),
                                                    1,1,10,1,&okayflag);
                if(okayflag)
                {
                    Cgame.buy_dv_cardd(numcards);
                    //need to process what the server responds with, then request for updated resource cards
                    if(check_rx_data_buff == 1)
                    {
                        retval = clientFrameHandler(Cgame, rxdatabuff);
                        update_dev_cards_on_gui();
                        Cgame.refresh_cards();
                        if(check_rx_data_buff == 1)
                        {
                            retval = clientFrameHandler(Cgame, rxdatabuff);
                            update_resources_display();
                        }
                    }
                }
            }
            else
            {
                std::cout << "invalid action!" << std::endl;
                retval = -1;
            }
        }
        else
        {
            for(i = 0; i < buttonlist.size(); i++)
            {
                if(sender() == buttonlist[i])
                {
                    if(buttonlist[i]->objectName().toStdString()[0] == 's')
                    {
                        //if settlement button, do settlement stuff.
                        get_settlement_corner_and_tile_from_name(buttonlist[i]->objectName().toStdString(), tile, corner);

                        if(ui->BUILD_CITY->isChecked())
                        {
                            Cgame.build_city(tile, corner);
                            updatecolortile = 1;
                        }
                        else if(ui->BUILD_SET->isChecked())
                        {
                            Cgame.build_settlement(tile, corner);
                            updatecolortile = 1;
                        }
                        else
                            std::cout << "Neither build city or build settlement options are selected!" << std::endl;
                    }
                    else if(buttonlist[i]->objectName().toStdString()[0] == 'r')
                    {
                        get_road_road_and_tile_from_name(buttonlist[i]->objectName().toStdString(), tile, road);
                        if(ui->BUILD_A_ROAD->isChecked())
                        {
                            Cgame.build_road(tile, road);
                            updatecolortile = 1;
                        }
                        else
                            std::cout << "Build road not selected!" << std::endl;
                    }
                    else
                        retval = -1;
    //                buttonlist[i]->setStyleSheet(PLAYER_1_COLOR);
                    break;
                }
            }
        }
    }
    else
    {
        QMessageBox msgB;
        msgB.setText("You must select a tile to place the robber on!");
        msgB.setInformativeText("To use dev card, press 'ok'. Otherwise press 'cancel'");
        msgB.setStandardButtons(QMessageBox::Ok);
        msgB.setDefaultButton(QMessageBox::Ok);
        ui->NotifyText->setText("You must select a tile to place the robber on!");
        ui->NotifyText->displayText();
        waiting_on_robber = 1;
        msgB.exec();
    }
//    ptrobj->setIcon(tempp);
    std::cout << "clicked! Sender ID: " << this->senderSignalIndex() << std::endl << "        Sender name: " << senderObjName.toStdString() << std::endl;
//        ui->tile1_1->setIcon(tempp);
        //>setIcon("C:/Users/gtb/Documents/GitHub/Brick.png");
    check_packet_and_update(i);
}

void MainWindow::check_packet_and_update(int i)
{
    int retval = 0;
    if(check_rx_data_buff == 1)
    {
        retval = clientFrameHandler(Cgame, rxdatabuff);
        if(updatecolortile > 0)
        {
            updatecolortile = 0;
            set_button_color(buttonlist[i]->objectName().toStdString(), buttonlist[i]);
        }
        else if((updateboardcolors > 0))// || (rx_packet_needs_processing > 0))
        {
            updateboardcolors = 0;
            update_board_colors();
        }
        if(update_board_icons == 1)
        {
            set_icons_and_rollvals_on_board();
            update_board_icons = 0;
        }
    }
}

void MainWindow::on_pushButton_released()
{
    QObject *senderObj = sender(); // This will give Sender object
    // This will give obejct name for above it will give "A", "B", "C"
    QString senderObjName = senderObj->objectName();
 //   if(senderObjName[0] == "t")

//    QPixmap tempr("C:/Users/gtb/Documents/GitHub/Brick.png");
//    if(senderObjName == "tile_1")
        std::cout << "unclicked! Button pressed: " << senderObjName.toStdString() << std::endl;
//    ui->tile1_1->setIcon(tempr);
}

void MainWindow::on_hover()
{
    std::cout <<" HOVVVEERRR" << std::endl;
}

void MainWindow::prompt_dev_card_to_use()
{
    int qtyknight   = Cgame.get_qty_dv_cardd(1);
    int qtyvp       = Cgame.get_qty_dv_cardd(2);
    int qtyyear     = Cgame.get_qty_dv_cardd(3);
    int qtymonopoly = Cgame.get_qty_dv_cardd(4);
    int qtyroads    = Cgame.get_qty_dv_cardd(5);

}

void MainWindow::use_dev_card_generic(const QString msg)
{
    QMessageBox msgBox;
    string tempstr = "";
    if(devcardtouse == 1)
        tempstr = "Knight";
    else if(devcardtouse == 2)
        tempstr = "Victory Point";
    else if(devcardtouse == 3)
        tempstr = "Year of Plenty";
    else if(devcardtouse == 4)
        tempstr = "Monopoly";
    else if(devcardtouse == 5)
        tempstr = "Build Roads";
    else
        tempstr = "INVALID DEV CARD SELECTION. ERROR. ERRRRORRR!";
    msgBox.setText(msg);
    msgBox.setInformativeText("To use dev card, press 'ok'. Otherwise press 'cancel'");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch(ret)
    {
    case QMessageBox::Ok:
        if((devcardtouse > 0) && (devcardtouse < 6))
        {
            devcardflag = 1;
            if(debug_text)
                std::cout << "about to use a dev card! Trying to use: " << tempstr << std::endl;
            Cgame.use_dev_cardd(devcardtouse);
        }
        //call use dev card function for knight
        break;
    case QMessageBox::Cancel:
        if(debug_text)
            std::cout << "Cancelled! Not using a " << tempstr << " development card" << std::endl;
        break;
    default:
        break;
    }

}

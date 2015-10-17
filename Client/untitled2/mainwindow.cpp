#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QWidget>
#include <QtGui>
#include <string>
#include <vector>
#include "clientTransmitHandler.h"
#include <QPushButton>
#include <string>
#include <QList>

#define BRICK_ICON      "C:/Users/gtb/Documents/GitHub/Brick.png"
#define SHEEP_ICON      "C:/Users/gtb/Documents/GitHub/Sheep.png"
#define FOREST_ICON     "C:/Users/gtb/Documents/GitHub/forest.png"
#define MOUNTAIN_ICON   "C:/Users/gtb/Documents/GitHub/Mountain.png"
#define WHEAT_ICON      "C:/Users/gtb/Documents/GitHub/Wheat.png"
#define DESERT_ICON     "C:/Users/gtb/Documents/GitHub/Desert.png"

#define NO_PLAYER_COLOR "background-color: rgb(150,150,150)"
#define PLAYER_1_COLOR  "background-color: rgb(0,255,0)"
#define PLAYER_2_COLOR  "background-color: rgb(0,0,255)"
#define PLAYER_3_COLOR  "background-color: rgb(255,0,0)"
#define PLAYER_4_COLOR  "background-color: rgb(255,255,255)"

#define Xicon_size  150
#define Yicon_size  150

string playercolors[5] = {NO_PLAYER_COLOR, PLAYER_1_COLOR, PLAYER_2_COLOR, PLAYER_3_COLOR, PLAYER_4_COLOR};
/*
 *TODO:
 * need to make a function to 'paint' the board info... it needs to update color of buttons based on who owns them!
 */
char rxdatabuff [4096];
const int road_array_tile_num[72] = {0,0,0,0,0,0,3,3,3,3,3,7,7,7,7,7,12,12,12,12,12,16,16,16,16,16,17,17,17,17,17,18,18,18,18,18,15,15,15,15,15,11,11,11,11,11,6,6,6,6,6,2,2,2,2,2,10,10,10,5,5,5,1,1,1,13,13,9,8,8,4,4};
const int road_array_road_num[72] = {0,1,2,3,4,5,0,1,2,3,4,0,1,2,3,4,0,1,2,3,5,5,0,1,2,3,0,1,2,4,5,0,1,2,4,5,0,1,3,4,5,0,1,3,4,5,0,2,3,4,5,0,2,3,4,5,2,3,4,2,3,4,2,4,5,3,4,2,1,0,1,2};
const int settlement_array_tile_num[54] = {0,0,0,0,0,0,3,3,3,3,7,7,7,7,12,12,12,12,16,16,16,16,17,17,17,17,18,18,18,18,15,15,15,15,11,11,11,11,6,6,6,6,2,2,2,2,1,1,4,9,9,9,9,9};
const int settlement_array_corner_num[54] = {0,1,2,3,4,5,1,2,3,4,1,2,3,4,0,1,2,3,0,1,2,3,0,1,2,5,5,0,1,2,1,4,5,0,1,0,5,4,3,0,5,4,0,3,4,5,5,2,1,0,1,2,3,4};
const int tile_mapping_array[19] = {9,5,4,8,3,1,0,2,6,10,13,14,12,7,16,17,11,15,18};
QList<QPushButton*> buttonlist;

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
    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
    std::cout << "switch val: " << switchval << std::endl;
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
            temp2 = tile_mapping_array[temp%19];        //real tile number.
            get_icon_file_rsrc_type_and_roll_from_tile_num(qstr, resrc, roll, temp2);
            folderpath.append(qstr);
            std::cout << "File path to set icon to: " << folderpath.toStdString() << std::endl;
            QPixmap temppix(folderpath);//
//            temppix.load(folderpath);
            buttonlist[x]->setIcon(temppix);
            buttonlist[x]->setIconSize(size);
            folderpath.clear();
                    //    get_tile_resource(3, tempstr);
                    //    temppix.load(tempstr);
                    //    ui->tile1_4->setIcon(temppix);
                    //    ui->tile1_4->setIconSize(size);
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
    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
    std::cout << "switch val: " << switchval << std::endl;
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
    std::cout << temp1 << std::endl;
//    name.erase(name.begin(), name.begin());
    std::cout << "switch val: " << switchval << std::endl;
    return(tile_mapping_array[(switchval-1) % 19]);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString tempstr = "";
    ui->setupUi(this);
    buttonlist = this->findChildren<QPushButton *> ();
    std::cout << "# buttons found: " << buttonlist.size() << std::endl;
    for(int x = 0; x < buttonlist.size(); x++)
        connect(buttonlist[x],SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    QPixmap temppix("C:/Users/gtb/Documents/GitHub/Brick.png");//
    QSize size(Xicon_size, Yicon_size);
    Cgame.initsocketthing();
//    get_tile_resource(0, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_1->setIcon(temppix);
    ui->tile1_1->setObjectName("t1");
//    ui->tile1_1->setIconSize(size);
    get_tile_resource(1, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_2->setIcon(temppix);
    ui->tile1_2->setObjectName("t2");
//    ui->tile1_2->setIconSize(size);
//    get_tile_resource(2, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_3->setIcon(temppix);
//    ui->tile1_3->setIconSize(size);
    ui->tile1_3->setObjectName("t3");
//    get_tile_resource(3, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_4->setIcon(temppix);
//    ui->tile1_4->setIconSize(size);
    ui->tile1_4->setObjectName("t4");
//    get_tile_resource(4, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_5->setIcon(temppix);
//    ui->tile1_5->setIconSize(size);
    ui->tile1_5->setObjectName("t5");
//    get_tile_resource(5, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_6->setIcon(temppix);
//    ui->tile1_6->setIconSize(size);
    ui->tile1_6->setObjectName("t6");
//    get_tile_resource(6, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_7->setIcon(temppix);
//    ui->tile1_7->setIconSize(size);
    ui->tile1_7->setObjectName("t7");
//    get_tile_resource(7, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_8->setIcon(temppix);
//    ui->tile1_8->setIconSize(size);
    ui->tile1_8->setObjectName("t8");
//    get_tile_resource(8, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_9->setIcon(temppix);
//    ui->tile1_9->setIconSize(size);
    ui->tile1_9->setObjectName("t9");
//    get_tile_resource(9, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_10->setIcon(temppix);
//    ui->tile1_10->setIconSize(size);
    ui->tile1_10->setObjectName("t10");
//    get_tile_resource(10, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_11->setIcon(temppix);
//    ui->tile1_11->setIconSize(size);
    ui->tile1_11->setObjectName("t11");
//    get_tile_resource(11, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_12->setIcon(temppix);
//    ui->tile1_12->setIconSize(size);
    ui->tile1_12->setObjectName("t12");
//    get_tile_resource(12, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_13->setIcon(temppix);
//    ui->tile1_13->setIconSize(size);
    ui->tile1_13->setObjectName("t13");
//    get_tile_resource(13, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_14->setIcon(temppix);
//    ui->tile1_14->setIconSize(size);
    ui->tile1_14->setObjectName("t14");
//    get_tile_resource(14, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_15->setIcon(temppix);
//    ui->tile1_15->setIconSize(size);
    ui->tile1_15->setObjectName("t15");
//    get_tile_resource(15, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_16->setIcon(temppix);
//    ui->tile1_16->setIconSize(size);
    ui->tile1_16->setObjectName("t16");
//    get_tile_resource(16, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_17->setIcon(temppix);
//    ui->tile1_17->setIconSize(size);
    ui->tile1_17->setObjectName("t17");
//    get_tile_resource(17, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_18->setIcon(temppix);
//    ui->tile1_18->setIconSize(size);
    ui->tile1_18->setObjectName("t18");
//    get_tile_resource(18, tempstr);
//    temppix.load(tempstr);
//    ui->tile1_19->setIcon(temppix);
//    ui->tile1_19->setIconSize(size);
    ui->tile1_19->setObjectName("t19");
    //setMask((tempmask.mask()));
//    ui->graphicsView->mask();
//    QPixmap pixmap("C:/Users/gtb/Documents/GitHub/Mountain.png");
//    QIcon icon(pixmap);
//    button->setIcon(icon);
//    button->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    connect(ui->tile1_1, SIGNAL(released()), this, SLOT(on_pushButton_released()));
    Cgame.joinGame();
    clientFrameHandler(Cgame, rxdatabuff);
}

MainWindow::~MainWindow()
{
    delete ui;
    exit(2);
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
        if(retval >= 0) // && (retval < playercolors.size()))
        {
            tempstr = QString::fromUtf8(playercolors[retval].c_str());
            ptr->setStyleSheet(tempstr);
        }
    }
    else if(nname[0] == 's')
    {
        get_settlement_corner_and_tile_from_name(nname, tile, road);
        retval = Cgame.get_corner_owner(tile, road);
        if(retval >= 0) // && (retval < playercolors.size()))
        {
            tempstr = QString::fromUtf8(playercolors[retval].c_str());
            ptr->setStyleSheet(tempstr);
        }
    }
    else
        std::cout << "ERRRRROORRRR Invalid button!" << std::endl;

}

int MainWindow::update_board_colors()
{
    for(int i = 0; i < buttonlist.size(); i++)
        set_button_color(buttonlist[i]->objectName().toStdString(), buttonlist[i]);
    return(1);
}

void MainWindow::on_pushButton_clicked()
{
    int retval = -1;
    int i = 0;
    int updatecolortile = 0;
    int updateboardcolors = 0;
    int tile = 0;
    int corner = 0;
    int road = 0;
    QPushButton *ptrobj = qobject_cast<QPushButton *>(sender());
    QObject *senderObj = sender(); // This will give Sender object
    // This will give obejct name for above it will give "A", "B", "C"
    QString senderObjName = senderObj->objectName();
    QPixmap tempp("C:/Users/gtb/Documents/GitHub/Brick_pressed.png");
//    senderObjName.toStdString().c_str()[0];
    if((senderObjName.toStdString().length() == 2) || (senderObjName.toStdString().length() == 3))
    {
        retval = get_tile_num_from_tile_name(senderObjName.toStdString());
        std::cout << "Tile pressed: " << retval << std::endl;
        ptrobj->setIcon(tempp);
    }
    else if(senderObjName.toStdString()[0] == 'B')//if one of the control buttons was pressed (all start with B), process it here
    {
		if (senderObjName.toStdString() == "BUILD_CITY")
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
            set_icons_and_rollvals_on_board();
        }
        else if(senderObjName.toStdString() == "B_UPDATE_BOARD")
        {
            std::cout << "Update board!" << std::endl;
            Cgame.get_board_info();
            updateboardcolors = 1;
            set_icons_and_rollvals_on_board();
        }
        else if(senderObjName.toStdString() == "B_START_GAME")
        {
            std::cout << "Start game!" << std::endl;
            Cgame.startGame();
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
                else if(buttonlist[i]->objectName().toStdString().at(0) == 'r')
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
//    ptrobj->setIcon(tempp);
    std::cout << "clicked! Sender ID: " << this->senderSignalIndex() << std::endl << "        Sender name: " << senderObjName.toStdString() << std::endl;
//        ui->tile1_1->setIcon(tempp);
        //>setIcon("C:/Users/gtb/Documents/GitHub/Brick.png");
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
    }
}

void MainWindow::on_pushButton_released()
{   QObject *senderObj = sender(); // This will give Sender object
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QWidget>
#include <QtGui>
#include <string>
#include <vector>
#include <QPushButton>
#include <string>
#include <QList>

#define BRICK_ICON      "C:/Users/gtb/Documents/GitHub/Brick.png"
#define SHEEP_ICON      "C:/Users/gtb/Documents/GitHub/Sheep.png"
#define FOREST_ICON     "C:/Users/gtb/Documents/GitHub/forest.png"
#define MOUNTAIN_ICON   "C:/Users/gtb/Documents/GitHub/Mountain.png"
#define WHEAT_ICON      "C:/Users/gtb/Documents/GitHub/Wheat.png"
#define DESERT_ICON     "C:/Users/gtb/Documents/GitHub/Desert.png"

#define PLAYER_1_COLOR  "background-color: rgb(0,255,0)"
#define PLAYER_2_COLOR  "background-color: rgb(0,0,255)"
#define PLAYER_3_COLOR  "background-color: rgb(255,0,0)"
#define PLAYER_4_COLOR  "background-color: rgb(255,255,255)"

#define Xicon_size  150
#define Yicon_size  150

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

const int road_array_tile_num[72] = {0,0,0,0,0,0,3,3,3,3,3,7,7,7,7,7,12,12,12,12,12,16,16,16,16,16,17,17,17,17,17,18,18,18,18,18,15,15,15,15,15,11,11,11,11,11,6,6,6,6,6,2,2,2,2,2,10,10,10,5,5,5,1,1,1,13,13,9,8,8,4,4};

const int road_array_road_num[72] = {0,1,2,3,4,5,0,1,2,3,4,0,1,2,3,4,0,1,2,3,5,5,0,1,2,3,0,1,2,4,5,0,1,2,4,5,0,1,3,4,5,0,1,3,4,5,0,2,3,4,5,0,2,3,4,5,2,3,4,2,3,4,2,4,5,3,4,2,1,0,1,2};

const int settlement_array_tile_num[54] = {0,0,0,0,0,0,3,3,3,3,7,7,7,7,12,12,12,12,16,16,16,16,17,17,17,17,18,18,18,18,15,15,15,15,11,11,11,11,6,6,6,6,2,2,2,2,1,1,4,9,9,9,9,9};

const int settlement_array_corner_num[54] = {0,1,2,3,4,5,1,2,3,4,1,2,3,4,0,1,2,3,0,1,2,3,0,1,2,5,5,0,1,2,1,4,5,0,1,0,5,4,3,0,5,4,0,3,4,5,5,2,1,0,1,2,3,4};

const int tile_mapping_array[19] = {9,5,4,8,3,1,0,2,6,10,13,14,12,7,16,17,11,15,18};

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
/*    switch(switchval)
    {
    case 1:
        retval = 18;
        break;
    case 2:
        retval = 17;
        break;
    case 3:
        retval = 12;
        break;
    case 4:
        retval = 13;
        break;
    case 5:
        retval = 1;
        break;
    case 6:
        retval = 11;
        break;
    case 7:
        retval = 0;
        break;
    case 8:
        retval = 10;
        break;
    case 9:
        retval = 9;
        break;
    case 10:
        retval = 16;
        break;
    case 11:
        retval = 14;
        break;
    case 12:
        retval = 15;
        break;
    case 13:
        retval = 3;
        break;
    case 14:
        retval = 2;
        break;
    case 15:
        retval = 4;
        break;
    case 16:
        retval = 5;
        break;
    case 17:
        retval = 8;
        break;
    case 18:
        retval = 7;
        break;
    case 19:
        retval = 6;
        break;
    default:
        retval = -1;
        break;
    }

    return(retval);
}
*/

QList<QPushButton*> buttonlist;

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
    get_tile_resource(0, tempstr);
    temppix.load(tempstr);
    ui->tile1_1->setIcon(temppix);
    ui->tile1_1->setObjectName("t1");
    ui->tile1_1->setIconSize(size);
    get_tile_resource(1, tempstr);
    temppix.load(tempstr);
    ui->tile1_2->setIcon(temppix);
    ui->tile1_2->setObjectName("t2");
    ui->tile1_2->setIconSize(size);
    get_tile_resource(2, tempstr);
    temppix.load(tempstr);
    ui->tile1_3->setIcon(temppix);
    ui->tile1_3->setIconSize(size);
    ui->tile1_3->setObjectName("t3");
    get_tile_resource(3, tempstr);
    temppix.load(tempstr);
    ui->tile1_4->setIcon(temppix);
    ui->tile1_4->setIconSize(size);
    ui->tile1_4->setObjectName("t4");
    get_tile_resource(4, tempstr);
    temppix.load(tempstr);
    ui->tile1_5->setIcon(temppix);
    ui->tile1_5->setIconSize(size);
    ui->tile1_5->setObjectName("t5");
    get_tile_resource(5, tempstr);
    temppix.load(tempstr);
    ui->tile1_6->setIcon(temppix);
    ui->tile1_6->setIconSize(size);
    ui->tile1_6->setObjectName("t6");
    get_tile_resource(6, tempstr);
    temppix.load(tempstr);
    ui->tile1_7->setIcon(temppix);
    ui->tile1_7->setIconSize(size);
    ui->tile1_7->setObjectName("t7");
    get_tile_resource(7, tempstr);
    temppix.load(tempstr);
    ui->tile1_8->setIcon(temppix);
    ui->tile1_8->setIconSize(size);
    ui->tile1_8->setObjectName("t8");
    get_tile_resource(8, tempstr);
    temppix.load(tempstr);
    ui->tile1_9->setIcon(temppix);
    ui->tile1_9->setIconSize(size);
    ui->tile1_9->setObjectName("t9");
    get_tile_resource(9, tempstr);
    temppix.load(tempstr);
    ui->tile1_10->setIcon(temppix);
    ui->tile1_10->setIconSize(size);
    ui->tile1_10->setObjectName("t10");
    get_tile_resource(10, tempstr);
    temppix.load(tempstr);
    ui->tile1_11->setIcon(temppix);
    ui->tile1_11->setIconSize(size);
    ui->tile1_11->setObjectName("t11");
    get_tile_resource(11, tempstr);
    temppix.load(tempstr);
    ui->tile1_12->setIcon(temppix);
    ui->tile1_12->setIconSize(size);
    ui->tile1_12->setObjectName("t12");
    get_tile_resource(12, tempstr);
    temppix.load(tempstr);
    ui->tile1_13->setIcon(temppix);
    ui->tile1_13->setIconSize(size);
    ui->tile1_13->setObjectName("t13");
    get_tile_resource(13, tempstr);
    temppix.load(tempstr);
    ui->tile1_14->setIcon(temppix);
    ui->tile1_14->setIconSize(size);
    ui->tile1_14->setObjectName("t14");
    get_tile_resource(14, tempstr);
    temppix.load(tempstr);
    ui->tile1_15->setIcon(temppix);
    ui->tile1_15->setIconSize(size);
    ui->tile1_15->setObjectName("t15");
    get_tile_resource(15, tempstr);
    temppix.load(tempstr);
    ui->tile1_16->setIcon(temppix);
    ui->tile1_16->setIconSize(size);
    ui->tile1_16->setObjectName("t16");
    get_tile_resource(16, tempstr);
    temppix.load(tempstr);
    ui->tile1_17->setIcon(temppix);
    ui->tile1_17->setIconSize(size);
    ui->tile1_17->setObjectName("t17");
    get_tile_resource(17, tempstr);
    temppix.load(tempstr);
    ui->tile1_18->setIcon(temppix);
    ui->tile1_18->setIconSize(size);
    ui->tile1_18->setObjectName("t18");
    get_tile_resource(18, tempstr);
    temppix.load(tempstr);
    ui->tile1_19->setIcon(temppix);
    ui->tile1_19->setIconSize(size);
    ui->tile1_19->setObjectName("t19");
    //setMask((tempmask.mask()));
//    ui->graphicsView->mask();
//    QPixmap pixmap("C:/Users/gtb/Documents/GitHub/Mountain.png");
//    QIcon icon(pixmap);
//    button->setIcon(icon);
//    button->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    connect(ui->tile1_1, SIGNAL(released()), this, SLOT(on_pushButton_released()));
/*
    connect(ui->tile1_1, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_1, SIGNAL(released()), this, SLOT(on_pushButton_released()));
    connect(ui->tile1_2, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_3, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_4, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_5, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_6, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_7, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_8, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_9, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_10, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_11, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_12, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_13, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_14, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_15, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_16, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_17, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_18, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tile1_19, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_1, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_2, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_3, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_4, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_5, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_6, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_7, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_8, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_9, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_10, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_11, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_12, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_13, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_14, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_15, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_16, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_17, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_18, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_19, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_20, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_21, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_22, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_23, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_24, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_25, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_26, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_27, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_28, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_29, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_30, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_31, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_32, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_33, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_34, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_35, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_36, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_37, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_38, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_39, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_40, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_41, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_42, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_43, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_44, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_45, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_46, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_47, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_48, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_49, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_50, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_51, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_52, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_53, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_54, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_55, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_56, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_57, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_58, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_59, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_60, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_61, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_62, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_63, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_64, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_65, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_66, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_67, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_68, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_69, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_70, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_71, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->road_72, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_1,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_2,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_3,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_4,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_5,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_6,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_7,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_8,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
//    connect(ui->set_9,   SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_10,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_11,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_12,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_13,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_14,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_15,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_16,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_17,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_18,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_19,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_20,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_21,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_22,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_23,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_24,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_25,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_26,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_27,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_28,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_29,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_30,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_31,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_32,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_33,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_34,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_35,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_36,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_37,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_38,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_39,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_40,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_41,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_42,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_43,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_44,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_45,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_46,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_47,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_48,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_49,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_50,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_51,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_52,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_53,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->set_54,  SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    //    connect(ui, SIGNAL(hovered()), this, SLOT(on_hover()));
*/
}

MainWindow::~MainWindow()
{
    delete ui;exitskwkxxwxaz
}

void MainWindow::on_pushButton_clicked()
{
    int retval = -1;
    int tile = 0;
    int corner = 0;
    int road = 0;
    QPushButton *ptrobj === qobject_cast<QPushButton *>(sender());
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
		}
        else if(senderObjName.toStdString() == "BUILD_ROAD")
            std::cout << "Make me build a road!" << std::endl;
        else if(senderObjName.toStdString() == "BUILD_SET")
            std::cout << "Make me build a settlement!" << std::endl;
        else if(senderObjName.toStdString() == "B_START_TURN")
            std::cout << "Start my turn!" << std::endl;
        else if(senderObjName.toStdString() == "B_CARDS_REF")
            std::cout << "Refresh my cards!" << std::endl;
        else if(senderObjName.toStdString() == "B_DICE_ROLL")
            std::cout << "Roll my dice!" << std::endl;
        else if(senderObjName.toStdString() == "B_END_TURN")
            std::cout << "End my turn!" << std::endl;
        else if(senderObjName.toStdString() == "B_UPDATE_BOARD")
            std::cout << "Update board!" << std::endl;
        else
            std::cout << "invalid action!" << std::endl;
    }
    else
    {
        for(int i = 0; i < buttonlist.size(); i++)
        {
            if(sender() == buttonlist[i])
            {
                if(buttonlist[i]->objectName().toStdString()[0] == 's')
                {
                    //if settlement button, do settlement stuff.
                    get_settlement_corner_and_tile_from_name(buttonlist[i]->objectName().toStdString(), tile, corner);
                }
                else if(buttonlist[i]->objectName().toStdString().at(0) == 'r')
                    get_road_road_and_tile_from_name(buttonlist[i]->objectName().toStdString(), tile, road);
                else
                    retval = 0;
                buttonlist[i]->setStyleSheet(PLAYER_1_COLOR);
                break;
            }
        }
    }
//    ptrobj->setIcon(tempp);
    std::cout << "clicked! Sender ID: " << this->senderSignalIndex() << std::endl << "        Sender name: " << senderObjName.toStdString() << std::endl;
//        ui->tile1_1->setIcon(tempp);
        //>setIcon("C:/Users/gtb/Documents/GitHub/Brick.png");
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

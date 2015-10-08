#include "mainwindow.h"
#include <QApplication>
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QWidget>
#include <QtGui>

//t0    ->  x = 0   y = 0
//t1    ->  x = 1   y = 1
//t2    ->  x = 2   y = 2
//t3    ->  x = 2   y = 4
//t4    ->  x = 2   y = 6
//t5    ->  x = 1   y = 7
//t6    ->  x = 0   y = 8
//t7    ->  x = -1  y = 7
//t8    ->  x = -2  y = 6
//t9    ->  x = -2  y = 4
//t10   ->  x = -2  y = 2
//t11   ->  x = -1  y = 1
//t12   ->  x = 0   y = 2
//t13   ->  x = 1   y = 3
//t14   ->  x = 1   y = 5
//t15   ->  x = 0   y = 6
//t16   ->  x = -1  y = 5
//t17   ->  x = -1  y = 3
//t18   ->  x = 0   y = 4

int main(int argc, char *argv[])
{
    //yoffset = 0;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
/*    QWidget *window = new QWidget;
    PushShape *button1 = new PushShape("C:/Users/gtb/Documents/GitHub/Brick.png");
    PushShape *button2 = new PushShape("C:/Users/gtb/Documents/GitHub/forest.png");
    PushShape *button3 = new PushShape("C:/Users/gtb/Documents/GitHub/Sheep.png");
    PushShape *button4 = new PushShape("C:/Users/gtb/Documents/GitHub/Brick.png");
    PushShape *button5 = new PushShape("C:/Users/gtb/Documents/GitHub/forest.png");
    PushShape *button6 = new PushShape("C:/Users/gtb/Documents/GitHub/Sheep.png");
    window->resize(window_width, window_height);
//    QPushButton *wid = new PushShape("C:/Users/gtb/Documents/GitHub/Brick.png");
//    wid->show();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);
    layout->addWidget(button6);
    window->setLayout(layout);
    window->show();*/
    return a.exec();
}


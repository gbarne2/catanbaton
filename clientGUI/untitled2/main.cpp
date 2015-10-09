#include "mainwindow.h"
#include <QApplication>
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QWidget>
#include <QtGui>


int main(int argc, char *argv[])
{
    //yoffset = 0;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();

    Sleep(3000);
    while(1)
    {
        std::cout << "looping in main!" << std::endl;
        Sleep(1000);
    }
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
    return(1);
}


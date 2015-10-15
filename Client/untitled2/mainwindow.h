#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameClient.h"
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void set_button_color(std::string nname, QPushButton *ptr);
    explicit MainWindow(QWidget *parent = 0);
    int update_board_colors();
    ~MainWindow();
public slots:
    void on_pushButton_clicked();
    void on_pushButton_released();
    void on_hover();

public: signals:
    void hovered();

private:
    Ui::MainWindow *ui;
    gameClient Cgame;
};

extern char rxdatabuff [4096];

#endif // MAINWINDOW_H

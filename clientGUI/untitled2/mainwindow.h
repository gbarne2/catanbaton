#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameClient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

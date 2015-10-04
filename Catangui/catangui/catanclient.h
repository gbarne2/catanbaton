#ifndef CATANCLIENT_H
#define CATANCLIENT_H

#include <QMainWindow>

namespace Ui {
class Catanclient;
}

class Catanclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit Catanclient(QWidget *parent = 0);
    ~Catanclient();

private:
    Ui::Catanclient *ui;
};

#endif // CATANCLIENT_H

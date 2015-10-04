#include "catanclient.h"
#include "ui_catanclient.h"

Catanclient::Catanclient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Catanclient)
{
    ui->setupUi(this);
}

Catanclient::~Catanclient()
{
    delete ui;
}

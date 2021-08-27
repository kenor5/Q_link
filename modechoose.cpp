#include "modechoose.h"
#include "ui_modechoose.h"

Modechoose::Modechoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modechoose)
{
    ui->setupUi(this);
}

Modechoose::~Modechoose()
{
    delete ui;
}

void Modechoose::on_pushButton_3_clicked()
{
    this->hide();
    emit dlgshow();
}

void Modechoose::on_pushButton_clicked()
{
    this->hide();
    emit mainshow();
}


void Modechoose::on_pushButton_2_clicked()
{
    this->hide();
    emit multiPlayer();
}

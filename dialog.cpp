#include "dialog.h"
#include "ui_dialog.h"
#include "modechoose.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->hide();
    emit modeChooseShow();
}

void Dialog::on_pushButton_3_clicked()
{
    emit quit();
}

void Dialog::on_pushButton_2_clicked()
{
    this->hide();
    emit loadGame();
}

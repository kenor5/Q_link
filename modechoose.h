#ifndef MODECHOOSE_H
#define MODECHOOSE_H

#include <QDialog>

namespace Ui {
class Modechoose;
}

class Modechoose : public QDialog
{
    Q_OBJECT

public:
    explicit Modechoose(QWidget *parent = nullptr);
    ~Modechoose();
private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void dlgshow();
    void mainshow();
    void multiPlayer();

private:
    Ui::Modechoose *ui;
};

#endif // MODECHOOSE_H

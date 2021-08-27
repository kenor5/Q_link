#include "mainwindow.h"
#include "dialog.h"
#include "modechoose.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
    Dialog dlg;
    Modechoose mdc;

    dlg.setObjectName("dlg");
    mdc.setObjectName("mdc");
    w->setObjectName("w");

    dlg.show();

    QObject::connect(&dlg, SIGNAL(modeChooseShow()), &mdc, SLOT(show()));
    QObject::connect(&dlg, SIGNAL(quit()), &a, SLOT(quit()));
    QObject::connect(&mdc, SIGNAL(dlgshow()), &dlg, SLOT(show()));
    QObject::connect(&mdc, SIGNAL(mainshow()), w, SLOT(showandstart()));
    QObject::connect(&mdc, SIGNAL(multiPlayer()), w, SLOT(multiPlayer()));
    QObject::connect(w, SIGNAL(gameover()), &mdc, SLOT(show()));
    QObject::connect(&dlg, SIGNAL(loadGame()), w, SLOT(loadFile()));
    return a.exec();
}

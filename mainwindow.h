#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QGridLayout>
#include <QPushButton>
#include <cstring>
#include <algorithm>
#include <ctime>
#include "imagebutton.h"
#include "music.h"
#include <iostream>
#include <QLabel>
#include <QPainter>
#include <QKeyEvent>
#include <QQueue>
#include <QTimer>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QMessageBox>
#include <string>

const int X = 8;    //横向10个图片
const int Y = 10;
const int totpic = X * Y /4;

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void change(int, int);
    void change2(int, int);
    void showandstart();
    void showTimeLimit();
    void pauseClicked();
    void playerFlash(int);
    void restart();
    void loadFile();
    void saveFile();
    void multiPlayer();

signals:
    void activateImg(int, int);
    void activateImg2(int, int);
    void gameover();

protected:

    void keyPressEvent(QKeyEvent *event);

private:
    struct loc{
      int x, y;
    public:
      loc(){}
      loc(int x, int y){this->x = x; this->y = y;}

      bool operator == (const loc &x1) {
              return x == x1.x && x1.y == y;

        }
      void operator = (const loc &x1) {
          x = x1.x;
          y = x1.y;
      }
    };

    ImageButton *image[X+2][Y+2];    // 存储二态图片的按钮
    int mode;
    int map[X+2][Y+2];    // 存储地图，0代表空地，数字代表图片类型
    int randomMap[X*Y];    // 用于随机建图的辅助数组
    int imgNum = X * Y;    // 图片数量
    bool solvability;    // 两个图片是否可以消除
    bool enableFlash = 0;    // 闪现权限，初始值为0
    bool enableHint = 0;
    int visited[X+2][Y+2];    // 用于辅助bfs，记录是否访问过以及在第几次折线上
    int timeLimit;    // 剩余时间
    int curScreen;    //主界面当前页面
    int flashStart;
    int hintStart;
    int score[2];
    QLabel *timeScreen;     // 显示时间
    QLabel *curSol;    // 显示当前两图标是否有解
    QLabel *remainSol;    // 显示剩下的图标是否有解
    QLabel *scores;
    QQueue<loc> queue;    //用于辅助bfs的队列
    QQueue<loc> check;    //用于记录和空地有接触的方块位置
    QGridLayout *gridLayout;    // 主布局
    QHBoxLayout *hLayout;    // 右侧功能栏
    QVBoxLayout *vLayout;    // 功能栏加主布局
    QHBoxLayout *pauseHLayout;
    QWidget *windows;    // 主widget
    QWidget *pauseWindows;    //暂停界面
    QWidget *toolWindows;
    QStackedWidget *mainWindows;
    QTimer *timer;
    QPixmap player1, player2, player3;    // 两个玩家的图标
    QPixmap pathPic[24];    // 路径图标
    QPixmap prop[6];    // 道具图标
    QPushButton *pause;    // 暂停按钮
    QHBoxLayout *pauseLayout;
    QPushButton *pauseButton[2];
    QSignalMapper *signalMapper[X+2][Y+2];

    QSound *sound[3];


    loc path[X+2][Y+2];    // 存储连线路径
    loc loc_player1, loc_player2;    // 玩家位置
    loc loc_first[3], loc_second[3];      // 激活的两个按钮位置
    loc loc_prop[6];    // 道具位置
    loc loc_hint[2];    // 提示位置

    enum{    // 路径方向
        u2d,l2r,u2r,u2l,d2l,d2r,
        p_u2d,p_l2r,p_u2r,p_u2l,p_d2l,p_d2r,
        p1_u2d,p1_l2r,p1_u2r,p1_u2l,p1_d2l,p1_d2r,
        p2_u2d,p2_l2r,p2_u2r,p2_u2l,p2_d2l,p2_d2r
    };
    enum{    // 道具列表
        _1s = 1, _dizzy, _flash, _shuffle, _hint, _freeze
    };
    // 初始随机排列图片，传入两个图片路径的字符串列表
    void arrangeImage(const QStringList &texts1, const QStringList &texts2);
    // 创建自定义按钮，传入两个图片的路径
    ImageButton *createImageButton(const QString &str1, const QString &str2);
    //bfs实现函数
    void bfs(int, int, int, int);
    // 刷新玩家位置，用于键盘事件后的刷新
    void flushPlayerLoc();
    // 检查两个图片是否可消除
    bool checkFeasibility(int);
    // 画出连线
    void drawLine();
    // 根据位置设置连线图片
    void flushLine(int);
    void debug();
    // 如果连线和player重合的处理
    void lineOnPlayer(loc, int ,int ,int);
    // 检查剩下的是否可消除
    bool checkRemain();
    // 检查是否在边缘
    bool connectedWithBlank(int, int);
    // 返回一个随机空白地
    loc randomPlace();
    // 返回随机一个图片的地址
    loc randonPic();
    // 加载图片
    void loadImg();
    // 设置暂停页面
    void pauseWidgetSetup();
    // 打乱重排
    void shuffle();
    // 放置道具
    void placeProp(int);
    // 道具效果处理
    void propHandler(int);
    // 检查图片周围有无空地
    void checkEmptyLoc(const loc&);
    // 主框架
    void arrangeLayout();
    // 主widget
    void arrangeWindow();
    // 初始化数组
    void initArray();
    void arrangeToolWidget();
    void flushImg();

};
#endif // MAINWINDOW_H

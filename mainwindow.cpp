#include "mainwindow.h"
#include <fstream>
//
// MainWindow 里面的各个私有变量的实例化和初始化
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("连连看");
    this->setFixedSize(900, 630);
    this->setStyleSheet("#w {border-image:url(:/new/prefix1/image/bkg_3.jpg)}");


    timeLimit = 200;
    initArray();
    std::srand((unsigned)time(NULL));
    loc_player1 = randomPlace();
    loc_player2 = randomPlace();

    pauseWindows = new QWidget();
    windows = new QWidget();
    mainWindows = new QStackedWidget();
    toolWindows = new QWidget();
    timer = new QTimer();
    timeScreen = new QLabel;
    curSol = new QLabel;
    scores = new QLabel;
    pause = new QPushButton;
    remainSol = new QLabel;
    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    gridLayout = new QGridLayout;
    sound[0] = new QSound(":/new/prefix1/bgm.wav");
    sound[1] = new QSound(":/new/prefix1/ok.wav");
    sound[2] = new QSound(":/new/prefix1/choose.wav");

    loadImg();
    arrangeLayout();
    this->setCentralWidget(mainWindows);

    connect(pause, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeLimit()));
    connect(this, SIGNAL(gameover()), this, SLOT(restart()));
}

void MainWindow::arrangeWindow()
{
    mainWindows->addWidget(pauseWindows);
    mainWindows->addWidget(windows);
    mainWindows->setCurrentWidget(windows);
    curScreen = 0;
    vLayout->addWidget(timeScreen,Qt::AlignCenter);
    vLayout->addWidget(curSol,Qt::AlignCenter);
    vLayout->addWidget(remainSol,Qt::AlignCenter);
    vLayout->addWidget(scores, Qt::AlignCenter);
    vLayout->addSpacerItem(new QSpacerItem(40, 40));
    vLayout->addWidget(pause, Qt::AlignCenter);

    hLayout->addLayout(gridLayout);
    hLayout->addSpacing(20);

    toolWindows->setLayout(vLayout);
    toolWindows->setObjectName("toolWindows");
    toolWindows->setStyleSheet("#toolWindows{background-color:rgba(244,244,244,140)}");
    toolWindows->setFixedSize(QSize(130, 580));
    hLayout->addWidget(toolWindows);
    flushPlayerLoc();
    windows->setLayout(hLayout);
    windows->setFixedSize(885, 600);

}

void MainWindow::arrangeToolWidget()
{
    timeScreen->setFixedSize(QSize(100, 100));
    timeScreen->setStyleSheet("background-color: rgba(255, 254, 248, 200);"
                              "border:2px rgb(237, 241, 255);"
                              "border-radius:7px;"
                              "padding:2px 3px;"
                              "font:bold 40px;"
                              "color:rgba(0,0,0,150);"
                              "border-style: outset;");
    curSol->setFixedSize(QSize(100, 50));
    curSol->setStyleSheet("background-color: rgba(255, 254, 248, 100);"
                          "border:2px rgb(237, 241, 255);"
                          "border-radius:7px;"
                          "padding:2px 3px;"
                          "font:bold 15px;"
                          "color:rgba(0,0,0,100);"
                          "border-style: outset;");
    remainSol->setFixedSize(QSize(100, 50));
    remainSol->setStyleSheet("background-color: rgba(255, 254, 248, 100);"
                             "border:2px rgb(237, 241, 255);"
                             "border-radius:7px;"
                             "padding:2px 3px;"
                             "font:bold 15px;"
                             "color:rgba(0,0,0,100);"
                             "border-style: outset;");
    scores->setFixedSize(QSize(100, 100));
    scores->setStyleSheet("background-color: rgba(255, 254, 248, 100);"
                             "border:2px rgb(237, 241, 255);"
                             "border-radius:7px;"
                             "padding:2px 3px;"
                             "font:bold 20px;"
                             "color:rgba(0,0,0,100);"
                             "border-style: outset;");
    pause->setFixedSize(QSize(100, 100));
    pause->setStyleSheet("QPushButton{"
                         "border-image:url(:/new/prefix1/image/pause.png);"
                         "border:2px rgb(237, 241, 255);"
                         "border-radius:50px;"
                         "padding:2px 3px;"
                         "border-style: outset;"
                          "}"

                         "QPushButton:hover{"
                          "background-color:rgb(229, 241, 251); "
                         "color: black;"
                         "}"
                         "QPushButton:pressed{"
                         "background-color:rgb(204, 228, 247);"
                         "border-style: inset;"
                          "}");
}
//
// 主页面的暂停界面布置
//
void MainWindow::pauseWidgetSetup()
{
    pauseLayout = new QHBoxLayout();
    pauseButton[0] = new QPushButton;
    pauseButton[1] = new QPushButton;

    pauseButton[0]->setText("存档");
    pauseButton[1]->setText("继续游戏");
    pauseButton[0]->setStyleSheet("QPushButton{"
                                  "background-color: rgb(255, 254, 248);"
                                  "border:2px rgb(237, 241, 255);"
                                  "border-radius:7px;"
                                  "padding:2px 3px;"
                                  "font:bold 25px;"
                                  "color:rgba(0,0,0,100);"
                                  "border-style: outset;}"

                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    pauseButton[1]->setStyleSheet("QPushButton{"
                                  "background-color: rgb(255, 254, 248);"
                                  "border:2px rgb(237, 241, 255);"
                                  "border-radius:7px;"
                                  "padding:2px 3px;"
                                  "font:bold 25px;"
                                  "color:rgba(0,0,0,100);"
                                  "border-style: outset;}"

                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

    pauseLayout->addWidget(pauseButton[0]);
    pauseLayout->addWidget(pauseButton[1]);

    pauseWindows->setLayout(pauseLayout);
    connect(pauseButton[0], SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(pauseButton[1], SIGNAL(clicked()), this, SLOT(pauseClicked()));

}
//
// 加载玩家icon、路径icon和道具icon
//
void MainWindow::loadImg()
{
    player1.load(":/new/prefix1/image/player1.png");
    player2.load(":/new/prefix1/image/player2.png");
    player3.load(":/new/prefix1/image/player3.png");

    // 两个存储图标路径的字符串列表
    QStringList texts1 = {
        ":/new/prefix1/image/group1/1-1 (0).png",
        ":/new/prefix1/image/group1/1-1 (1).png",
        ":/new/prefix1/image/group1/1-1 (2).png",
        ":/new/prefix1/image/group1/1-1 (3).png",
        ":/new/prefix1/image/group1/1-1 (4).png",
        ":/new/prefix1/image/group1/1-1 (5).png",
        ":/new/prefix1/image/group1/1-1 (6).png",
        ":/new/prefix1/image/group1/1-1 (7).png",
        ":/new/prefix1/image/group1/1-1 (8).png",
        ":/new/prefix1/image/group1/1-1 (9).png",
        ":/new/prefix1/image/group1/1-1 (10).png",
        ":/new/prefix1/image/group1/1-1 (11).png",
        ":/new/prefix1/image/group1/1-1 (12).png",
        ":/new/prefix1/image/group1/1-1 (13).png",
        ":/new/prefix1/image/group1/1-1 (14).png",
        ":/new/prefix1/image/group1/1-1 (15).png",
        ":/new/prefix1/image/group1/1-1 (16).png",
        ":/new/prefix1/image/group1/1-1 (17).png",
        ":/new/prefix1/image/group1/1-1 (18).png",
        ":/new/prefix1/image/group1/1-1 (19).png",
        ":/new/prefix1/image/group1/1-1 (20).png",
    };
    QStringList texts2 = {
        ":/new/prefix1/image/group1-1/1-2 (0).png",
        ":/new/prefix1/image/group1-1/1-2 (1).jpg",
        ":/new/prefix1/image/group1-1/1-2 (2).jpg",
        ":/new/prefix1/image/group1-1/1-2 (3).jpg",
        ":/new/prefix1/image/group1-1/1-2 (4).jpg",
        ":/new/prefix1/image/group1-1/1-2 (5).jpg",
        ":/new/prefix1/image/group1-1/1-2 (6).jpg",
        ":/new/prefix1/image/group1-1/1-2 (7).jpg",
        ":/new/prefix1/image/group1-1/1-2 (8).jpg",
        ":/new/prefix1/image/group1-1/1-2 (9).jpg",
        ":/new/prefix1/image/group1-1/1-2 (10).jpg",
        ":/new/prefix1/image/group1-1/1-2 (11).jpg",
        ":/new/prefix1/image/group1-1/1-2 (12).jpg",
        ":/new/prefix1/image/group1-1/1-2 (13).jpg",
        ":/new/prefix1/image/group1-1/1-2 (14).jpg",
        ":/new/prefix1/image/group1-1/1-2 (15).jpg",
        ":/new/prefix1/image/group1-1/1-2 (16).jpg",
        ":/new/prefix1/image/group1-1/1-2 (17).jpg",
        ":/new/prefix1/image/group1-1/1-2 (18).jpg",
        ":/new/prefix1/image/group1-1/1-2 (19).jpg",
        ":/new/prefix1/image/group1-1/1-2 (20).jpg",
    };
    // 排列图片并显示
    arrangeImage(texts1, texts2);

    QStringList tmp = {":/new/prefix1/image/u2d.png",
                       ":/new/prefix1/image/l2r.png",
                       ":/new/prefix1/image/u2r.png",
                       ":/new/prefix1/image/u2l.png",
                       ":/new/prefix1/image/d2l.png",
                       ":/new/prefix1/image/d2r.png",

                       ":/new/prefix1/image/2p_u2d.png",
                       ":/new/prefix1/image/2p_l2r.png",
                       ":/new/prefix1/image/2p_u2r.png",
                       ":/new/prefix1/image/2p_u2l.png",
                       ":/new/prefix1/image/2p_d2l.png",
                       ":/new/prefix1/image/2p_d2r.png",

                       ":/new/prefix1/image/p1_u2d.png",
                       ":/new/prefix1/image/p1_l2r.png",
                       ":/new/prefix1/image/p1_u2r.png",
                       ":/new/prefix1/image/p1_u2l.png",
                       ":/new/prefix1/image/p2_d2l.png",
                       ":/new/prefix1/image/p1_d2r.png",

                       ":/new/prefix1/image/p2_u2d.png",
                       ":/new/prefix1/image/p2_l2r.png",
                       ":/new/prefix1/image/p2_u2r.png",
                       ":/new/prefix1/image/p2_u2l.png",
                       ":/new/prefix1/image/p1_d2l.png",
                       ":/new/prefix1/image/p2_d2r.png",
                      };
    for (int i = 0; i < tmp.size(); ++i) {
        pathPic[i].load(tmp[i]);
    }

    QStringList tmp1 = {":/new/prefix1/image/_1s.png",
                        ":/new/prefix1/image/_dizzy.png",
                        ":/new/prefix1/image/_flash.png",
                        ":/new/prefix1/image/_shuffle.png",
                        ":/new/prefix1/image/_hint.png",
                        ":/new/prefix1/image/_freeze.png"

    };
    for (int i = 0; i < tmp1.size(); ++i) {
        prop[i].load(tmp1[i]);
    }
}
//
// 随机返回一个空地位置
//
MainWindow::loc MainWindow::randomPlace()
{

    int tmp = rand()%((X+2)*(Y+2) - imgNum) + 1;    // 剩下的空地数量
    for (int i = 0; i < X+2; ++i)
        for(int j = 0; j < Y+2; ++j) {
            if(!map[i][j]) tmp --;
            if(!tmp) return loc(i, j);
        }
    return loc(0, 0);
}
//
// 随机返回一个图片位置
//
MainWindow::loc MainWindow::randonPic()
{

    int tmp = rand()%imgNum + 1;    // 剩下的图片数量
    for (int i = 0; i < X+2; ++i)
        for(int j = 0; j < Y+2; ++j) {
            if(map[i][j]) tmp --;
            if(!tmp) return loc(i, j);
        }
    return loc(0, 0);
}
//
// 重新排列
//
void MainWindow::shuffle()
{
    for(int i = 1; i < X+1; ++i)
        for(int j = 1; j < Y+1; ++j) {
            if(map[i][j]) {
                std::srand((unsigned)time(NULL));
                loc tmp = randonPic();
                image[i][j]->exchangeIcon(image[tmp.x][tmp.y]);
                image[i][j]->flushIcon();
                image[tmp.x][tmp.y]->flushIcon();

                int tmp1 = map[tmp.x][tmp.y];
                map[tmp.x][tmp.y] = map[i][j];
                map[i][j] = tmp1;
            }
        }

}

MainWindow::~MainWindow()
{
}
//
// 当暂停按键和返回按键的槽函数，切换顶部窗口
//
void MainWindow::pauseClicked()
{   if(!curScreen) {
        mainWindows->setCurrentWidget(pauseWindows);
        curScreen = 1;
    }else {
        mainWindows->setCurrentWidget(windows);
        curScreen = 0;
    }
}
//
// 键盘事件处理，控制人物移动，发送激活信号
//
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!curScreen){
        switch (event->key()) {
        case Qt::Key_W: {
            // 到达边界就break
            if(loc_player1.x == 0) break;
            // 碰到图片就发送激活信号
            if(map[loc_player1.x - 1][loc_player1.y] > 0)
                emit activateImg(loc_player1.x - 1, loc_player1.y);
            else if(loc_player1.x) {
                loc_player1.x --;
                flushPlayerLoc();
            }
            if(map[loc_player1.x][loc_player1.y] < 0)
                propHandler(map[loc_player1.x][loc_player1.y]);

            break;
        }
        case Qt::Key_A: {
            if(loc_player1.y == 0) break;
            if(map[loc_player1.x][loc_player1.y - 1] > 0)
                emit activateImg(loc_player1.x,loc_player1.y - 1);
            else if(loc_player1.y) {
                loc_player1.y --;
                flushPlayerLoc();
            }
            if(map[loc_player1.x][loc_player1.y] < 0)
                propHandler(map[loc_player1.x][loc_player1.y]);
            break;
        }
        case Qt::Key_S: {
            if(loc_player1.x == X+1) break;
            if(map[loc_player1.x + 1][loc_player1.y] > 0)
                emit activateImg(loc_player1.x + 1, loc_player1.y);
            else if(loc_player1.x < X+1){
                loc_player1.x ++;
                flushPlayerLoc();
            }
            if(map[loc_player1.x][loc_player1.y] < 0)
                propHandler(map[loc_player1.x][loc_player1.y]);

            break;
        }
        case Qt::Key_D: {
            if(loc_player1.y == Y+1) break;
            if(map[loc_player1.x][loc_player1.y + 1] > 0)
                emit activateImg(loc_player1.x, loc_player1.y + 1);
            else if(loc_player1.y < Y+1) {
                loc_player1.y ++;
                flushPlayerLoc();
            }
            if(map[loc_player1.x][loc_player1.y] < 0)
                propHandler(map[loc_player1.x][loc_player1.y]);

            break;
        }
        case Qt::Key_I: {

            if (!mode) break;
            // 到达边界就break
            if(loc_player2.x == 0) break;
            // 碰到图片就发送激活信号
            if(map[loc_player2.x - 1][loc_player2.y] > 0)
                emit activateImg2(loc_player2.x - 1, loc_player2.y);
            else if(loc_player2.x) {
                loc_player2.x --;
                flushPlayerLoc();
            }
            if(map[loc_player2.x][loc_player2.y] < 0)
                propHandler(map[loc_player2.x][loc_player2.y]);

            break;
        }
        case Qt::Key_J: {
            if (!mode) break;
            if(loc_player2.y == 0) break;
            if(map[loc_player2.x][loc_player2.y - 1] > 0)
                emit activateImg2(loc_player2.x,loc_player2.y - 1);
            else if(loc_player2.y) {
                loc_player2.y --;
                flushPlayerLoc();
            }
            if(map[loc_player2.x][loc_player2.y] < 0)
                propHandler(map[loc_player2.x][loc_player2.y]);
            break;
        }
        case Qt::Key_K: {
            if (!mode) break;
            if(loc_player2.x == X+1) break;
            if(map[loc_player2.x + 1][loc_player2.y] > 0)
                emit activateImg2(loc_player2.x + 1, loc_player2.y);
            else if(loc_player2.x < X+1){
                loc_player2.x ++;
                flushPlayerLoc();
            }
            if(map[loc_player2.x][loc_player2.y] < 0)
                propHandler(map[loc_player2.x][loc_player2.y]);

            break;
        }
        case Qt::Key_L: {
            if (!mode) break;
            if(loc_player2.y == Y+1) break;
            if(map[loc_player2.x][loc_player2.y + 1] > 0)
                emit activateImg2(loc_player2.x, loc_player2.y + 1);
            else if(loc_player2.y < Y+1) {
                loc_player2.y ++;
                flushPlayerLoc();
            }
            if(map[loc_player2.x][loc_player2.y] < 0)
                propHandler(map[loc_player2.x][loc_player2.y]);

            break;
        }
        }
    }
}
//
// 刷新player位置，用于键盘事件后的刷新
//
void MainWindow::flushPlayerLoc()
{
    // 先把所有非图片区域清空
    for(int i = 0; i < X+2; ++i)
        for(int j = 0; j < Y+2; ++j) {
            if(map[i][j] == 0)
                image[i][j]->clearIcon();
        }
    // 在新位置放置图片
    image[loc_player1.x][loc_player1.y]->setPlayer(QIcon(player1));

    if(mode) {
        image[loc_player2.x][loc_player2.y]->setPlayer(QIcon(player2));
        // 如果重合，放置另一张图片
        if(loc_player1 == loc_player2)
            image[loc_player1.x][loc_player1.y]->setPlayer(QIcon(player3));
    }
}

void MainWindow::arrangeLayout()
{
    pauseWidgetSetup();
    arrangeWindow();
    arrangeToolWidget();
}

void MainWindow::initArray()
{
    memset(score, 0, sizeof(score));
    memset(map, 0, sizeof(map));
    // 为随机化数组赋初值，为4组1-20
    for(int i = 0; i < totpic*4; ++i)
        randomMap[i] = i % totpic +1;
    // 随机化数组
    std::srand(time(0));
    std::random_shuffle(randomMap, randomMap+totpic*4);

    //将随机后的数组赋给地图，当作图片的位置
    int k = 0;
    for(int i = 1; i <= X; ++i)
        for(int j = 1; j <= Y; ++j) {
            map[i][j] = randomMap[k++];
        }
}
//
// 起始函数
//
void MainWindow::showandstart()
{
    timeLimit = 200;
    timer->start(1000);
    this->show();
    sound[0]->play();
    sound[0]->setLoops(-1);
    mode = 0;
}

void MainWindow::multiPlayer()
{
    connect(this, SIGNAL(activateImg2(int, int)), this, SLOT(change2(int, int)));
    timeLimit = 200;
    timer->start(1000);
    this->show();
    sound[0]->play();
    sound[0]->setLoops(-1);
    mode = 1;
    flushPlayerLoc();
}
//
// 时间显示的槽函数
//
void MainWindow::showTimeLimit()
{
    if (!curScreen){
        if (timeLimit) {
            timeLimit --;
            timeScreen->setNum(timeLimit);
            if(!mode) {
                switch (timeLimit) {
                case 95: placeProp(_flash);break;
                case 30: placeProp(_shuffle);break;
//                case 85: placeProp(_hint);break;
                case 90: placeProp(_1s); break;
                default: break;
                }
            }
        }else {
            QMessageBox tmp;
            tmp.setText("很遗憾，时间到了！");
            tmp.exec();
            this->hide();
            timer->stop();
            emit gameover();
            // TODO
        }
    }
}
//
// 创建Imagebutton， 用于辅助arrangeImage函数
//
ImageButton *MainWindow::createImageButton(const QString &str1,
                                           const QString &str2)
{
    // 加载两张图片
    QPixmap img1, img2;
    img1.load(str1);
    img2.load(str2);
    ImageButton *button = new ImageButton;
    // 设置大小和图片
//    button->setGeometry(0, 0, 0, 0);
    button->setFixedSize(int(img1.width()/2.15), int(img1.height()/2.15));
    button->setButtonIcon(QIcon(img1), QIcon(img2));
    button->setIconSize(QSize(img1.width()/2, img1.height()/2));
    button->setStyleSheet("border:1px rgba(237, 241, 255, 100);"
                          "border-radius:8px;"
                          "padding:0px;"
                          "border-style: outset;");
    return button;
}

//
// 根据map的数字来设置当前button的图像，0代表空地，1-20代表方块，
//
void MainWindow::arrangeImage(const QStringList &texts1,
                              const QStringList &texts2)
{
    // 对每一个图标，创建并加入gridlayout
    for(int i = 0; i < X+2; ++i)
        for(int j = 0; j < Y+2; ++j) {
            signalMapper[i][j] = new QSignalMapper;
            int buttonNum = map[i][j];
            image[i][j] = createImageButton(
                          texts1[buttonNum],
                          texts2[buttonNum]);
            gridLayout->addWidget(image[i][j], i, j, Qt::AlignCenter);
            connect(image[i][j], SIGNAL(clicked()), signalMapper[i][j], SLOT(map()));
            signalMapper[i][j]->setMapping(image[i][j],100*i+j);
            connect(signalMapper[i][j],SIGNAL(mapped(int)),this, SLOT(playerFlash(int)));
        }
    // 链接激活函数和槽函数
    connect(this, SIGNAL(activateImg(int, int)),this, SLOT(change(int, int)));
}

//
// 处理激活的槽函数
//
void MainWindow::change(int x, int y)
{
    enum{first, second};        // 第几次激活
    static int visitTime = first;
    if (visitTime == first) {        // 第一次激活直接激活
        loc_first[0] = loc(x, y);
        visitTime = second;
        image[x][y]->changeIcon();
        flushPlayerLoc();
        sound[1]->play();
    }else {    // 第二次激活
        loc_second[0] = loc(x, y);
        visitTime = first;

        if (x == loc_first[0].x && y == loc_first[0].y) {   // 如果是同一个按钮，变回去
            image[x][y]->changeIcon();
            sound[1]->play();
        }
        else{
            if(checkFeasibility(0)) {    // 如果两个按钮可消除
                map[x][y] = 0;    //消除图片，并把map更新为空地
                map[loc_first[0].x][loc_first[0].y] = 0;
                image[x][y]->changeIcon();
                flushLine(0);
                sound[2]->play();
                imgNum -= 2;
                score[0] +=2;
                QString tmp = "score:\n" + QString::number(score[0]);
                if(mode) tmp+=":"+QString::number(score[1]);
                scores->setText(tmp);
                curSol->setText("可消除");
                if (imgNum == 0) {
                    QMessageBox tmp;
                    tmp.setText("恭喜你，呱唧呱唧！");
                    tmp.exec();
                    emit gameover();
                }
            }else {     // 不可消除就去除激活态
                image[loc_first[0].x][loc_first[0].y]->changeIcon();
                curSol->setText("不可消除");
                sound[1]->play();
            }
        }
        if(checkRemain())
            remainSol->setText("有解");
        else {
            remainSol->setText("无解");
            QMessageBox tmp;
            tmp.setText("很遗憾，无解");
            tmp.exec();
            emit gameover();
        }
    }
}

void MainWindow::change2(int x, int y)
{
    // 第几次激活
    static int visitTime1 = 1;
    // 第一次激活直接激活
    if (visitTime1 == 1) {
        loc_first[1] = loc(x, y);
        visitTime1 = 2;
        image[x][y]->changeIcon();
        flushPlayerLoc();
        sound[1]->play();
//        sound[1]->stop();
    }else {    // 第二次激活
        loc_second[1] = loc(x, y);
        visitTime1 = 1;

        if (x == loc_first[1].x && y == loc_first[1].y) {   // 如果是同一个按钮，变回去
            image[x][y]->changeIcon();
            sound[1]->play();
//            sound[1]->stop();
        }

        else{
            if(checkFeasibility(1)) {    // 如果两个按钮可消除
                map[x][y] = 0;    //消除图片，并把map更新为空地
                map[loc_first[1].x][loc_first[1].y] = 0;
                image[x][y]->changeIcon();
                flushLine(1);
                sound[2]->play();
//                sound[2]->stop();
                imgNum -= 2;
                score[1] +=2;
                QString tmp = "score:\n" + QString::number(score[0]);
                if(mode) tmp+=":"+QString::number(score[1]);
                scores->setText(tmp);
                curSol->setText("可消除");
                if (imgNum == 0) {
                    QMessageBox tmp;
                    tmp.setText("恭喜你，呱唧呱唧！");
                    tmp.exec();
                    emit gameover();
//                    TODO
                }
            }else {     // 不可消除就去除激活态
                image[loc_first[1].x][loc_first[1].y]->changeIcon();
                curSol->setText("不可消除");
                sound[1]->play();
//                sound[1]->stop();
            }
        }
        if(checkRemain())
            remainSol->setText("有解");
        else {
            remainSol->setText("无解");
            QMessageBox tmp;
            tmp.setText("很遗憾，无解");
            tmp.exec();

            emit gameover();
        }

    }


}

//
// 闪现槽函数
//
void MainWindow::playerFlash(int interLoc)
{
    int x = interLoc / 100;
    int y = interLoc % 100;
    if(!flashStart)
        flashStart = timeLimit;
    if(flashStart - timeLimit > 5)
        enableFlash = 0;

    if (enableFlash) {
        if(map[x][y] == 0)
            loc_player1 = loc(x, y);
        else if(map[x][y] > 0) {
            checkEmptyLoc(loc(x, y));
        }
    }
    flushPlayerLoc();
}


void MainWindow::checkEmptyLoc(const loc& tmp)
{
    if (map[tmp.x - 1][tmp.y] == 0) {
        loc_player1 = loc(tmp.x - 1, tmp.y);
        emit activateImg(tmp.x, tmp.y);
    }
    if (map[tmp.x + 1][tmp.y] == 0) {
        loc_player1 = loc(tmp.x + 1, tmp.y);
        emit activateImg(tmp.x, tmp.y);
    }
    if (map[tmp.x ][tmp.y- 1] == 0) {
        loc_player1 = loc(tmp.x, tmp.y - 1);
        emit activateImg(tmp.x, tmp.y);
    }
    \
    if (map[tmp.x][tmp.y+1] == 0) {
        loc_player1 = loc(tmp.x, tmp.y + 1);
        emit activateImg(tmp.x, tmp.y);
    }
}
//
// 检查两个图标是否可消除
bool MainWindow::checkFeasibility(int i)
{
    // 如果两张图片不一样，不可消除
    if(map[loc_first[i].x][loc_first[i].y] !=
       map[loc_second[i].x][loc_second[i].y]) {

        return false;
    }

    //先把visited数组清零
    solvability = false;
    memset(visited, 0, sizeof(visited));
    queue.clear();
    for (int s = 0; s < X+2; ++s)
        for(int j = 0; j < Y+2; ++j)
            path[s][j] = loc(-1, -1);
    queue.enqueue(loc(loc_first[i].x, loc_first[i].y));
    while(!queue.empty()) {
        loc tmp = queue.front();
        queue.pop_front();
        bfs(tmp.x, tmp.y, visited[tmp.x][tmp.y]+1, i);
        if (solvability) return true;
    }
    return false;
}
//
// 可消除状态下的路线更新，路径上的方块都转换图像。path里记录的是拐点信息
//
void MainWindow::flushLine(int c)
{
    // 从第二个img开始往回溯
    loc cur = loc_second[c];
    loc next = path[cur.x][cur.y];
    while (!(cur == loc_first[c])) {
        // 两个关键点在同一列
        if (cur.x == next.x) {
            if(cur.y < next.y) {
                // 把关键点之间的空地改为直线
                for (int i = cur.y + 1; i < next.y; ++i) {
                    // l2r代表left_to_right, u2d代表up_to_down
                    image[cur.x][i]->setPlayer(QIcon(pathPic[l2r]));
                    // 如果和人物的位置重合，就换图片
                    lineOnPlayer(loc(cur.x, i), p1_l2r, p2_l2r, p_l2r);
                }
                // 拐点部分单独处理
                //要根据下一个拐点具体判断
                if (!(next == loc_first[c])) {
                    if(path[next.x][next.y].x < next.x) {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[u2l]));
                        lineOnPlayer(next, p1_u2l, p2_u2l, p_u2l);
                    }
                    else {
//                        std::cerr << "d"  ;
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[d2l]));
                        lineOnPlayer(next, p1_d2l, p2_d2l, p_d2l);
                    }
                }
            }else {
                for (int i = next.y + 1; i < cur.y; ++i) {
                    image[cur.x][i]->setPlayer(QIcon(pathPic[l2r]));
                    lineOnPlayer(loc(cur.x, i), p1_l2r, p2_l2r, p_l2r);
                }
                if (!(next == loc_first[c])) {
                    if(path[next.x][next.y].x < next.x) {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[u2r]));
                        lineOnPlayer(next, p1_u2r, p2_u2r, p_u2r);
                    }
                    else {
//                        std::cerr << "d"  ;
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[d2r]));
                        lineOnPlayer(next, p1_d2r, p2_d2r, p_d2r);
                    }

                }

            }
        // 两关键点在同一行
        }else {
            if(cur.x < next.x) {
                for (int i = cur.x + 1; i < next.x; ++i) {
                    image[i][cur.y]->setPlayer(QIcon(pathPic[u2d]));
                    lineOnPlayer(loc(i, cur.y), p1_u2d, p2_u2d, p_u2d);
                }
                if (!(next == loc_first[c])) {
                    if(path[next.x][next.y].y < next.y) {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[u2l]));
                        lineOnPlayer(next, p1_u2l, p2_u2l, p_u2l);
                    }
                    else {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[u2r]));
                        lineOnPlayer(next, p1_u2r, p2_u2r, p_u2r);
                    }
                }
            }else {
                for (int i = next.x + 1; i < cur.x; ++i) {
                    image[i][cur.y]->setPlayer(QIcon(pathPic[u2d]));
                    lineOnPlayer(loc(i, cur.y), p1_u2d, p2_u2d, p_u2d);
                }
                if (!(next == loc_first[c])) {
                    if(path[next.x][next.y].y < next.y) {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[d2l]));
                        lineOnPlayer(next, p1_d2l, p2_d2l, p_d2l);
                    }
                    else {
                        image[next.x][next.y]->setPlayer(QIcon(pathPic[d2r]));
                        lineOnPlayer(next, p1_d2r, p2_d2r, p_d2r);
                    }
                }
            }
        }
        // 更新当前点和下一个点，进入下一次回溯
        cur = next;
        next = path[cur.x][cur.y];
    }
}
//
// 处理路径显示和人物重合
//
void MainWindow::lineOnPlayer(loc next,
                              int index1,
                              int index2,
                              int index3)
{
    if (next == loc_player1)
        image[next.x][next.y]->setPlayer(QIcon(pathPic[index1]));
    if (mode){
        if (next == loc_player2)
            image[next.x][next.y]->setPlayer(QIcon(pathPic[index2]));
        if (next == loc_player1 && next == loc_player2)
            image[next.x][next.y]->setPlayer(QIcon(pathPic[index3]));
    }
}


void MainWindow::debug()
{
    for (int i = 0; i < X+2; ++i) {
        for(int j = 0; j < Y+2; ++j){
            std::cerr << "("
                      << path[i][j].x
                      <<","
                      << path[i][j].y
                      << ") ";
        }
    std::cerr << std::endl;
    }
    std::cerr << std::endl;
}


// 广搜函数，x1 y1 第一个图片坐标; cnt 转折点次数;
void MainWindow::bfs(int x1, int y1,int cnt, int i)
{

    int tmp_x = x1, tmp_y = y1;
    int cnt_limit = 3;
    // 向上搜索
    while (-- tmp_x >=0 && !map[tmp_x][tmp_y]) {
        if (!visited[tmp_x][tmp_y] && cnt <= cnt_limit) {
            queue.enqueue(loc(tmp_x, tmp_y));
            visited[tmp_x][tmp_y] = cnt;
            path[tmp_x][tmp_y] = loc(x1, y1);
        }
    }
    if (tmp_x == loc_second[i].x && tmp_y == loc_second[i].y) {
        solvability = true;
        path[tmp_x][tmp_y] = loc(x1, y1);
        return;
    }

    tmp_x = x1;
    // 向下搜索
    while (++ tmp_x < X+2 && !map[tmp_x][tmp_y]) {
        if (!visited[tmp_x][tmp_y] && cnt <= cnt_limit) {
            queue.enqueue(loc(tmp_x, tmp_y));
            visited[tmp_x][tmp_y] = cnt;
            path[tmp_x][tmp_y] = loc(x1, y1);
        }
    }
    if (tmp_x == loc_second[i].x && tmp_y == loc_second[i].y) {
        solvability = true;
        path[tmp_x][tmp_y] = loc(x1,y1);
        return;
    }

    tmp_x = x1;
    while (-- tmp_y >= 0 && !map[tmp_x][tmp_y]) {
        if (!visited[tmp_x][tmp_y] && cnt <= cnt_limit) {
            queue.enqueue(loc(tmp_x, tmp_y));
            visited[tmp_x][tmp_y] = cnt;
            path[tmp_x][tmp_y] = loc(x1, y1);
        }
    }
    if (tmp_x == loc_second[i].x && tmp_y == loc_second[i].y) {
        solvability = true;
        path[tmp_x][tmp_y] = loc(x1, y1);
        return;
    }

    tmp_y = y1;
    while (++ tmp_y < Y+2 && !map[tmp_x][tmp_y]) {
        if (!visited[tmp_x][tmp_y] && cnt <= cnt_limit) {
            queue.enqueue(loc(tmp_x, tmp_y));
            visited[tmp_x][tmp_y] = cnt;
            path[tmp_x][tmp_y] = loc(x1, y1);
        }
    }
    if (tmp_x == loc_second[i].x && tmp_y == loc_second[i].y) {
        solvability = true;
        path[tmp_x][tmp_y] = loc(x1, y1);
        return;
    }

}
//
// 检查剩余方块是否有解，返回有或无
//
bool MainWindow::checkRemain()
{
    check.clear();
    for(int i = 1; i < X+1; ++i)
        for(int j = 1; j < Y+1; ++j) {
            // 如果处在边缘，加入队列
            if (connectedWithBlank(i, j))
                check.push_back(loc(i, j));
        }
    // 两两互相检查
    while (!check.empty()) {
        loc_first[2] = check.front();
        check.pop_front();

        for(int i = 0; i <check.size(); ++i) {
            loc_second[2] = check[i];
            if (checkFeasibility(2))
            {
                loc_hint[0] = loc_first[2];
                loc_hint[1] = loc_second[2];
                return true;
            }
        }
    }
    return false;
}
//
// 检查方块是否处在边缘，用于辅助checkremain。
// 由于游戏玩法原因，只要四个方向有空地就一定位于边缘
//
bool MainWindow::connectedWithBlank(int i, int j)
{
    if(map[i][j]) {
        if(!map[i+1][j]) return true;
        if(!map[i-1][j]) return true;
        if(!map[i][j+1]) return true;
        if(!map[i][j-1]) return true;
        return false;
    }else
        return false;
}

//
//  放置道具，传入道具编号，实现设置位置刷新显示等功能
//
void MainWindow::placeProp(int num)
{
    std::srand(unsigned(time(NULL)));
    loc tmp = randomPlace();
    // 避免和玩家或其它道具重合
    while (tmp == loc_player1 || tmp == loc_player2 || map[tmp.x][tmp.y] < 0) {
        tmp = randomPlace();
    }
    // 让道具在地图上的表示分布在-1到-6
    loc_prop[num - 1] = tmp;
    map[tmp.x][tmp.y] = -1 * num;
    image[tmp.x][tmp.y]->setIcon(QIcon(prop[num - 1]));

}

//
// 道具处理函数，传入道具编号的负数
//
void MainWindow::propHandler(int num)
{
    num *= -1;
    map[loc_prop[num - 1].x][loc_prop[num - 1].y] = 0;
    switch (num) {
        case _1s: {
            timeLimit += 30;
            break;
        }
        case _shuffle: {
            shuffle();
            break;
        }
        case _flash: {
            flashStart = 0;
            enableFlash = 1;
            break;
        }
        case _dizzy: {

            break;
        }
        case _freeze: {

            break;
        }
        case _hint: {
            hintStart = 0;
            enableHint = 1;
            break;
        }
    }
}
//
// 重新开始游戏
//
void MainWindow::restart()
{
    this->hide();

    initArray();
    std::srand((unsigned)time(NULL));
    loc_player1 = randomPlace();
    loc_player2 = randomPlace();
    sound[0]->stop();

    shuffle();
    flushPlayerLoc();
}

void MainWindow::loadFile()
{
    std::ifstream in("C:\\Users\\86189\\Documents\\project\\lastGame.txt", std::ios::in);
    in >> mode;
    in >> timeLimit
        >> loc_player1.x
        >> loc_player1.y
        >> score[0];
    if (mode){
        in >> loc_player2.x
           >> loc_player2.y
           >> score[1];
        connect(this, SIGNAL(activateImg2(int, int)), this, SLOT(change2(int, int)));
    }
    for(int i = 0; i < X+2;++i)
        for(int j = 0; j < Y+2; ++j)
            in >> map[i][j];

    in.close();

    timer->start(1000);
    this->show();
    sound[0]->play();
    sound[0]->setLoops(-1);
    flushImg();
    flushPlayerLoc();
}

void MainWindow::saveFile()
{
    QMessageBox tmp;
    tmp.setText("已保存！");
    tmp.exec();

    mainWindows->setCurrentWidget(windows);
    curScreen = 0;
    std::ofstream out("C:\\Users\\86189\\Documents\\project\\lastGame.txt", std::ios::out);
    out << mode <<" ";
    out << timeLimit << " "
        << loc_player1.x << " "
        << loc_player1.y << " "
        << score[0]<< std::endl;
    if (mode)
        out << loc_player2.x << " "
            << loc_player2.y << " "
            <<score[1]
            <<std::endl;
    for(int i = 0; i < X+2;++i)
        for(int j = 0; j < Y+2; ++j)
            out << map[i][j] << " ";

    out.close();
    emit gameover();
}

void MainWindow::flushImg()
{
    QStringList texts1 = {
        ":/new/prefix1/image/group1/1-1 (0).png",
        ":/new/prefix1/image/group1/1-1 (1).png",
        ":/new/prefix1/image/group1/1-1 (2).png",
        ":/new/prefix1/image/group1/1-1 (3).png",
        ":/new/prefix1/image/group1/1-1 (4).png",
        ":/new/prefix1/image/group1/1-1 (5).png",
        ":/new/prefix1/image/group1/1-1 (6).png",
        ":/new/prefix1/image/group1/1-1 (7).png",
        ":/new/prefix1/image/group1/1-1 (8).png",
        ":/new/prefix1/image/group1/1-1 (9).png",
        ":/new/prefix1/image/group1/1-1 (10).png",
        ":/new/prefix1/image/group1/1-1 (11).png",
        ":/new/prefix1/image/group1/1-1 (12).png",
        ":/new/prefix1/image/group1/1-1 (13).png",
        ":/new/prefix1/image/group1/1-1 (14).png",
        ":/new/prefix1/image/group1/1-1 (15).png",
        ":/new/prefix1/image/group1/1-1 (16).png",
        ":/new/prefix1/image/group1/1-1 (17).png",
        ":/new/prefix1/image/group1/1-1 (18).png",
        ":/new/prefix1/image/group1/1-1 (19).png",
        ":/new/prefix1/image/group1/1-1 (20).png",
    };
    QStringList texts2 = {
        ":/new/prefix1/image/group1-1/1-2 (0).png",
        ":/new/prefix1/image/group1-1/1-2 (1).jpg",
        ":/new/prefix1/image/group1-1/1-2 (2).jpg",
        ":/new/prefix1/image/group1-1/1-2 (3).jpg",
        ":/new/prefix1/image/group1-1/1-2 (4).jpg",
        ":/new/prefix1/image/group1-1/1-2 (5).jpg",
        ":/new/prefix1/image/group1-1/1-2 (6).jpg",
        ":/new/prefix1/image/group1-1/1-2 (7).jpg",
        ":/new/prefix1/image/group1-1/1-2 (8).jpg",
        ":/new/prefix1/image/group1-1/1-2 (9).jpg",
        ":/new/prefix1/image/group1-1/1-2 (10).jpg",
        ":/new/prefix1/image/group1-1/1-2 (11).jpg",
        ":/new/prefix1/image/group1-1/1-2 (12).jpg",
        ":/new/prefix1/image/group1-1/1-2 (13).jpg",
        ":/new/prefix1/image/group1-1/1-2 (14).jpg",
        ":/new/prefix1/image/group1-1/1-2 (15).jpg",
        ":/new/prefix1/image/group1-1/1-2 (16).jpg",
        ":/new/prefix1/image/group1-1/1-2 (17).jpg",
        ":/new/prefix1/image/group1-1/1-2 (18).jpg",
        ":/new/prefix1/image/group1-1/1-2 (19).jpg",
        ":/new/prefix1/image/group1-1/1-2 (20).jpg",
    };

    for (int i = 0; i < X+2; ++i)
        for(int j = 0; j < Y+2; ++j) {
            ImageButton *tmp;
            tmp = createImageButton(texts1[map[i][j]], texts2[map[i][j]]);
            image[i][j]->exchangeIcon(tmp);
            image[i][j]->flushIcon();
            delete tmp;
        }

}

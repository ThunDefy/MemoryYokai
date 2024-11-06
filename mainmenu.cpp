#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "sologameui.h"
#include "competitivegameui.h"
#include "helpwindow.h"

#include "Card.h"
#include "player.h"

#include <QGraphicsTextItem>
#include <QFont>
#include <QImage>
#include <QBrush>
#include <QFont>
#include <QTime>
#include <QtGlobal>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTextBrowser>

QSqlTableModel* yokaiInf;
QSqlTableModel* recordsInf;

SoloGameUI * soloUi;
CompetitiveGameUi* compUi;

MainMenu::MainMenu(QWidget *parent){
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");

    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":/db/Yokai_data.db");
    db.setDatabaseName("../MemoryYokai/Yokai_data.db");

    if(db.open()){
        qDebug()<<"BD opened successfully";
        yokaiInf = new QSqlTableModel(this,db);
        recordsInf= new QSqlTableModel(this,db);
        yokaiInf->setTable("Yokai");
        yokaiInf->select();
        recordsInf->setTable("Records");
        recordsInf->select();

    }else{
        qDebug()<<"Error BD did not open" ;
    }

    setWindowIcon(QIcon(":/menu/src/Menu/On.png"));
    soloUi = new SoloGameUI;
    compUi = new CompetitiveGameUi;
    HelpWind = new HelpWindow();
    HelpWind->hide();

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1920,1080);
    setBackgroundBrush(QBrush(QImage(":/menu/src/Menu/MainMenuBack.jpg")));

    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1920,1080);

    startGameBt = new QPushButton();
    startGameBt->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    startGameBt->setGeometry(QRect(390,590,300,300));
    scene->addWidget(startGameBt);

    connect(startGameBt,SIGNAL(clicked()),this,SLOT(PushStartGameBt()));

    soloPlayBt = new QPushButton();
    soloPlayBt->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    soloPlayBt->setGeometry(QRect(70,165,330,330));
    scene->addWidget(soloPlayBt);
    SetOrUnsetSoloImg(1);
    connect(soloPlayBt,SIGNAL(clicked()),this,SLOT(PushSoloPlayBt()));

    int shiftLeft = 135;
    for(int i = 0; i<7;i++){
        if(i>1)shiftLeft = 138;
        selectSizeBt[i] = new QPushButton();
        selectSizeBt[i]->setGeometry(QRect(65+(shiftLeft*i),43,80,90));
        scene->addWidget(selectSizeBt[i]);
        connect(selectSizeBt[i], &QPushButton::clicked, this,[=] () { PushSelectSizeBt(i); });
    }
    UnSelectedAllSizeBt();

    for(int i = 0; i<3;i++){
        selectPlayersBt[i] = new QPushButton();
        selectPlayersBt[i]->setGeometry(QRect(440+(180*i),300,140,170));
        scene->addWidget(selectPlayersBt[i]);
        connect(selectPlayersBt[i], &QPushButton::clicked, this,[=] () { PushSelectPlayersBt(i); });
    }
    UnSelectedAllPlayersBt();

    QString tmpSize, tmpTime, tmpTurns;
    for(int i = 0; i<7;i++){
        recordsText[i] = new QGraphicsTextItem();
        tmpSize = (recordsInf->data(recordsInf->index(i,1)).toString());
        tmpTime = (recordsInf->data(recordsInf->index(i,2)).toString());
        tmpTurns = (recordsInf->data(recordsInf->index(i,3)).toString());
        if(tmpTime == '0' || tmpTurns == '0')recordsText[i]->setPlainText(tmpSize+"\t\t\t- \t\t\t-");
        else recordsText[i]->setPlainText(tmpSize+"\t\t\t"+tmpTime+"\t\t\t"+tmpTurns);
        recordsText[i]->setDefaultTextColor(Qt::white);
        recordsText[i]->setFont(QFont("Kashima RUS by Cop",40));
        recordsText[i]->setPos(1240,120+(45*i));
        recordsText[i]->show();
        scene->addItem(recordsText[i]);
    }

    exitBt = new QPushButton();
    exitBt->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    exitBt->setGeometry(QRect(406,1000,300,90));
    scene->addWidget(exitBt);
    connect(exitBt,SIGNAL(clicked()),this,SLOT(PushExitBt()));

    showHelpBt = new QPushButton();
    showHelpBt->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    showHelpBt->setGeometry(QRect(1820,436,80,80));
    scene->addWidget(showHelpBt);
    connect(showHelpBt,SIGNAL(clicked()),this,SLOT(ShowHelp()));

}


void MainMenu::PushStartGameBt()
{
    HelpWind->hide();
    if(gameMod == 0){
        this->close();
        soloUi = new SoloGameUI(size,1);
        soloUi->showFullScreen();
    }else{
        this->close();
        compUi = new CompetitiveGameUi(size,playersCount);
        compUi->showFullScreen();
    }

}

void MainMenu::PushSoloPlayBt()
{
    if(gameMod != 0){
        UnSelectedAllPlayersBt();
        gameMod = 0;
        SetOrUnsetSoloImg(1);
    }
}

void MainMenu::PushExitBt()
{
    HelpWind->close();
    close();
}

void MainMenu::ShowHelp()
{
    HelpWind->show();
}


void MainMenu::PushSelectSizeBt(int btInd)
{
    UnSelectedAllSizeBt(btInd);

    switch (btInd) {
    case 0:
        size = 12;
        break;
    case 1:
        size = 16;
        break;
    case 2:
        size = 20;
        break;
    case 3:
        size = 24;
        break;
    case 4:
        size = 30;
        break;
    case 5:
        size = 40;
        break;
    case 6:
        size = 50;
    default:
        break;
    }
}

void MainMenu::PushSelectPlayersBt(int btInd)
{
    UnSelectedAllPlayersBt(btInd);
    SetOrUnsetSoloImg(0);
    gameMod = 1;
    switch (btInd) {
    case 0:
        playersCount = 2;
        break;
    case 1:
        playersCount = 3;
        break;
    case 2:
        playersCount = 4;
        break;
    default:
        break;
    }

}

void MainMenu::UnSelectedAllSizeBt(int except)
{
    QPixmap setImage (":/menu/src/Menu/Set.png");
    QPixmap noSetImage (":/menu/src/Menu/NoSet.png");

    noSetImage = noSetImage.scaled(80,90);
    setImage = setImage.scaled(80,90);

    for(int i = 0; i<7;i++){
        if(i == except){
            selectSizeBt[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
            selectSizeBt[i]->setIcon(setImage);
            selectSizeBt[i]->setIconSize(noSetImage.rect().size());
        }
        else{
            selectSizeBt[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
            selectSizeBt[i]->setIcon(noSetImage);
            selectSizeBt[i]->setIconSize(noSetImage.rect().size());
        }

    }
}

void MainMenu::UnSelectedAllPlayersBt(int except)
{
    QPixmap setImage (":/menu/src/Menu/Set.png");
    QPixmap noSetImage (":/menu/src/Menu/NoSet.png");

    noSetImage = noSetImage.scaled(140,170);
    setImage = setImage.scaled(140,170);

    for(int i = 0; i<3;i++){
        if(i == except){
            selectPlayersBt[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
            selectPlayersBt[i]->setIcon(setImage);
            selectPlayersBt[i]->setIconSize(noSetImage.rect().size());
        }
        else{
            selectPlayersBt[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
            selectPlayersBt[i]->setIcon(noSetImage);
            selectPlayersBt[i]->setIconSize(noSetImage.rect().size());
        }

    }
}

void MainMenu::SetOrUnsetSoloImg(int on)
{
    QPixmap soloPlayOnBtImg(":/menu/src/Menu/On.png");
    QPixmap soloPlayOffBtImg(":/menu/src/Menu/Off.png");

    soloPlayOnBtImg = soloPlayOnBtImg.scaled(330,330);
    soloPlayOffBtImg = soloPlayOffBtImg.scaled(330,330);

    if(on == 1){
        soloPlayBt->setIcon(soloPlayOnBtImg);
        soloPlayBt->setIconSize(soloPlayOnBtImg.rect().size());
    }else{
        soloPlayBt->setIcon(soloPlayOffBtImg);
        soloPlayBt->setIconSize(soloPlayOffBtImg.rect().size());
    }


}


MainMenu::~MainMenu()
{
    delete ui;
}

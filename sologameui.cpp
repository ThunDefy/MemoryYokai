#include "sologameui.h"
#include "ui_sologameui.h"
#include "Card.h"
#include "player.h"
#include "mainmenu.h"
#include "competitivegameui.h"

#include <QGraphicsTextItem>
#include <QFont>
#include <QImage>
#include <QBrush>
#include <QKeyEvent>
#include <QFont>
#include <QFontDatabase>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

extern QSqlTableModel* recordsInf;


void SoloGameUI::CreateField(){
    switch (size) {
    case 50:
        columns_count = 10;
        indent_column = 150;
        indent_line = 200;
        scale = 0.6;
        margin_left = 250;
        margin_top = 40;
        if(isSolo)sizeInd=6;
        break;

    case 40:
        columns_count = 10;
        indent_column = 150;
        indent_line = 200;
        scale = 0.6;
        margin_left = 250;
        margin_top = 150;
        if(isSolo)sizeInd=5;
        break;

    case 30:
        columns_count = 8;
        indent_column = 180;
        indent_line = 250;
        scale = 0.75;
        margin_left = 250;
        margin_top = 30;
        if(isSolo)sizeInd=4;
        break;

    case 24:
        columns_count = 8;
        indent_column = 185;
        indent_line = 290;
        scale = 0.77;
        margin_left = 230;
        margin_top = 90;
        if(isSolo)sizeInd=3;
        break;

    case 20:
        columns_count = 5;
        indent_column = 250;
        indent_line = 250;
        scale = 0.75;
        margin_left = 320;
        margin_top = 20;
        if(isSolo)sizeInd=2;
        break;

    case 16:
        columns_count = 4;
        indent_column = 250;
        indent_line = 250;
        scale = 0.75;
        margin_left = 400;
        margin_top = 20;
        if(isSolo)sizeInd=1;
        break;

    case 12:
        columns_count = 4;
        indent_column = 250;
        indent_line = 330;
        scale = 1;
        margin_left = 390;
        margin_top = 40;
        if(isSolo)sizeInd=0;
        break;
    default:
        break;
    };
    CleanOpenCards();

    srand(time(0));

    for(int j = 0; j<size ; j++)
        DataField[j] = 0;

    int rnd_img[size/2];
    int rnd_ind;
    for(int i=0;i<size/2;i++) rnd_img[i] = 0;

    int twins;
    for(int i = 0; i<size/2;i++){
        twins = 1;
        while(twins==1){
            twins =0;
            rnd_ind = 1 + rand() % 50;
            for(int j=0;j<size/2;j++)
                if( rnd_img[j] == rnd_ind)
                    twins =1;
            }
        rnd_img[i] = rnd_ind;
    }

    for(int i = 0; i<size;i++){
        while(1){
            twins=0;
            rnd_ind = rand() % (size/2);
            for(int j = 0; j<size ; j++)
                if(this->DataField[j] == rnd_img[rnd_ind])
                    twins++;
            if(twins <2){
                this->DataField[i] = rnd_img[rnd_ind];
                break;
            }
        }
    }

    QPixmap card;

    int column = 0, line = 1;
    for(int i = 0; i<size;i++){
        column += 1;
        if(column == columns_count+1){
            qDebug()<<i;
            if(size==30 && i ==24){
                qDebug()<<"Yes";
                line+=indent_line;
                column = 2;
            }
            else{
                line+=indent_line;
                column = 1;
            }
        }
        card.load(":/cards/src/Cards/0.jpg");
        cards[i] = new Card(i,DataField[i]);
        cards[i]->setPixmap(card) ;
        cards[i]->setToolTip("???");
        cards[i]->setScale(scale);
        cards[i]-> setZValue(-1);
        cards[i]->setPos(margin_left+(column*indent_column),margin_top+line);
        scene->addItem(cards[i]);
    }
    qDebug()<<"_Field created";

}

void SoloGameUI::SetOpenCard(int cardInd, int imgInd)
{

    if(OpenCards[0]==0){
        OpenCards[0] = imgInd;
        OpenCards[2] = cardInd;
    }
    else if(OpenCards[1]==0){
        if(isSolo){
            player->makeTurn();
            UpdateTextTurns();
        }
        OpenCards[1] = imgInd;
        OpenCards[3] = cardInd;
        canOpenNew = false;
        QTimer::singleShot(500, this, SLOT(TwinsCheck()));

    }else{
        CleanOpenCards();
    }

}

void SoloGameUI::TwinsCheck()
{
    if(OpenCards[0] == OpenCards[1] && OpenCards[2] != OpenCards[3]){
        qDebug()<<"Find twins!!";

        cards[OpenCards[2]]->CardRemove();
        cards[OpenCards[3]]->CardRemove();
        CleanOpenCards();
        player->gets1Point();
        pairs_remain -=1;
        UpdateTextPairs();
        qDebug()<<"__Pairs remain = "<<pairs_remain;
        if(!checkRemain()){
            qDebug()<<"Game OVER";
            controlTimer(0);
            EndGameMenuShow();
        }

    }else if(OpenCards[0] != OpenCards[1] && OpenCards[2] != OpenCards[3]) {
        qDebug()<<"fail";

        cards[OpenCards[2]]->CardFlip();
        cards[OpenCards[3]]->CardFlip();
        CleanOpenCards();
    }
    if(!isPause)canOpenNew = true;
}

void SoloGameUI::PushPauseBt()
{
    if(isPause){
        isPause = false;
        if(isSolo)controlTimer(1);
        canOpenNew = true;
        pauseMenu->hide();
        for(int i=0;i<3;i++){
            pauseButtons[i]->hide();
        }

    }
    else{
        if(isSolo)controlTimer(0);
        isPause = true;
        canOpenNew = false;
        pauseMenu->show();
        pauseMenu-> setZValue(2);
        for(int i=0;i<3;i++){
            pauseButtons[i]->setGeometry(QRect(785,310+(150*i),345,120));
            pauseButtons[i]->show();
            pauseButtons[i]->activateWindow();
            pauseButtons[i]->raise();
        }

    }

}

void SoloGameUI::PushContinueBt()
{
    isPause = false;
    if(isSolo)controlTimer(1);
    canOpenNew = true;
    pauseMenu->hide();
    for(int i=0;i<3;i++)
        pauseButtons[i]->hide();
}

void SoloGameUI::PushRestartBt()
{
    if(isPause){
        PushPauseBt();
        RestartGame();
    }
    else{
        endMenu->hide();
        pause->show();
        RestartGame();
        if(isSolo)controlTimer(1);
        canOpenNew = true;
        for(int i=1;i<3;i++){
            pauseButtons[i]->hide();
        }
        if(isSolo){
            timeNow->setFont(QFont("Kashima RUS by Cop",120));
            timeNow->setPos(65,460);
            turns->setFont(QFont("Kashima RUS by Cop",120));
            turns->setPos(66,720);
        }

    }
}

void SoloGameUI::PushBackToMenuBt()
{
        this->close();
        MainMenu * mainMenu = new MainMenu();
        mainMenu->showFullScreen();


}

void SoloGameUI::CleanOpenCards()
{
    for(int i=0;i<4;i++)
        OpenCards[i]=0;
    qDebug()<<"CLEAR DONE";

}

void SoloGameUI::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
       PushPauseBt();
    }

}

void SoloGameUI::UpdateTextPairs()
{
    pairs->setPlainText(QString::number(pairs_remain));
}

void SoloGameUI::UpdateTextTimeNow()
{
    timeNow->setPlainText(QString::number(timer));

}

void SoloGameUI::UpdateTextTurns()
{
    turns->setPlainText(QString::number(player->getTurns()));
}

void SoloGameUI::CheckNewRecord()
{
    QSqlQuery* query;
    query = new QSqlQuery();

    query->prepare("UPDATE Records SET Time=?, Turns=? WHERE Size=?");

    int OldTime = (recordsInf->data(recordsInf->index(sizeInd,2)).toInt());
    int OldTurns = (recordsInf->data(recordsInf->index(sizeInd,3)).toInt());
    qDebug()<<OldTime<<" "<<OldTurns;

    if(OldTime == 0){
        qDebug()<<"NEW RECORD";
        query->bindValue(0,timer);
        query->bindValue(1,player->getTurns());
        query->bindValue(2,size);
        query->exec();
    }else if(timer < OldTime ){
        qDebug()<<"NEW RECORD";
        query->bindValue(0,timer);
        query->bindValue(1,player->getTurns());
        query->bindValue(2,size);
        query->exec();
    }else if(timer == OldTime){
        if(player->getTurns() < OldTurns){
            query->bindValue(1,player->getTurns());
            query->bindValue(2,size);
            query->exec();
        }
    }
}

void SoloGameUI::RestartGame()
{
    CleanOpenCards();

    for(int i = 0; i<size;i++){
         scene->removeItem(cards[i]);
    }
    CreateField();

    timer= 0;
    pairs_remain = size/2;

    player->setScore(0);
    player->setTurns(0);

    UpdateTextTurns();
    controlTimer(1);
    UpdateTextTimeNow();
    UpdateTextPairs();

    canOpenNew = true;

    qDebug()<<"_Restart done";

}

void SoloGameUI::EndGameMenuShow()
{
    CheckNewRecord();
    endMenu->show();
    pause->hide();
    for(int i=1;i<3;i++){
        pauseButtons[i]->show();
        pauseButtons[i]->setStyleSheet("background-color: rgba(255, 180, 255, 0);");
        pauseButtons[i]->setGeometry(QRect(720,510+(125*(i-1)),193,70));
    }
    timeNow->setFont(QFont("Kashima RUS by Cop",60));
    timeNow->setPos(1110,560);
    timeNow-> setZValue(3);
    turns->setFont(QFont("Kashima RUS by Cop",60));
    turns->setPos(1110,680);
    turns->setZValue(3);

}

bool SoloGameUI::checkRemain()
{
    if(pairs_remain != 0) return true;
    else return false;
}

void SoloGameUI::timerEvent(QTimerEvent *evt)
{
    if (evt->timerId() == timerId)
        {
            timer +=1;
            UpdateTextTimeNow();
    }
}

void SoloGameUI::controlTimer(int status)
{
    if(status == 1) timerId = startTimer(1000);
    else if(status == 0) killTimer(timerId);
}

SoloGameUI::SoloGameUI(int setsize, int CreateSoloUi)
{
    setWindowIcon(QIcon(":/menu/src/Menu/On.png"));
    isSolo = CreateSoloUi;

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1920,1080);
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setFixedSize(1920,1080);

    pauseMenu = new QGraphicsPixmapItem();
    QPixmap pauseMenuImage(":/menu/src/Menu/pauseMenu.png");
    pauseMenu->setPixmap(pauseMenuImage) ;
    pauseMenu->setPos(700,250);
    pauseMenu->hide();
    scene->addItem(pauseMenu);

    for(int i=0;i<3;i++){
        pauseButtons[i] = new QPushButton();
        pauseButtons[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        pauseButtons[i]->setGeometry(QRect(785,310+(150*i),345,120)); // 452 restart
        pauseButtons[i]->hide();
        scene->addWidget(pauseButtons[i]);
    }

    connect(pauseButtons[0],SIGNAL(clicked()),this,SLOT(PushContinueBt()));
    connect(pauseButtons[1],SIGNAL(clicked()),this,SLOT(PushRestartBt()));
    connect(pauseButtons[2],SIGNAL(clicked()),this,SLOT(PushBackToMenuBt()));


       // Отрисовка соло интерфейса

    if(CreateSoloUi){

        size = setsize;
        pairs_remain = size/2;

        CreateField();

        setBackgroundBrush(QBrush(QImage(":/menu/src/Menu/SoloBack.jpg")));
        player = new Player();

        pause = new QPushButton();
        QPixmap pauseImage(":/menu/src/Menu/Pause.png");
        pauseImage = pauseImage.scaled(180,180);
        pause->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        pause->setIcon(pauseImage);
        pause->setIconSize(pauseImage.rect().size());
        pause->setGeometry(QRect(80,0,180,180));
        scene->addWidget(pause);
        connect(pause,SIGNAL(clicked()),this,SLOT(PushPauseBt()));

        pairs = new QGraphicsTextItem();
        pairs->setPlainText(QString::number(pairs_remain));
        pairs->setDefaultTextColor(Qt::white);
        pairs->setFont(QFont("Kashima RUS by Cop",120));
        pairs->setPos(65,214);
        scene->addItem(pairs);

        timeNow = new QGraphicsTextItem();
        timeNow->setPlainText(QString::number(timer));
        timeNow->setDefaultTextColor(Qt::white);
        timeNow->setFont(QFont("Kashima RUS by Cop",120));
        timeNow->setPos(65,460);
        scene->addItem(timeNow);

        turns = new QGraphicsTextItem();
        turns->setPlainText(QString::number(player->getTurns()));
        turns->setDefaultTextColor(Qt::white);
        turns->setFont(QFont("Kashima RUS by Cop",120));
        turns->setPos(66,720);
        scene->addItem(turns);
        controlTimer(1);

        endMenu = new QGraphicsPixmapItem();
        QPixmap endMenuImage(":/menu/src/Menu/endMenu.png");
        endMenu->setPixmap(endMenuImage) ;
        endMenu->setPos(700,250);
        endMenu->hide();
        scene->addItem(endMenu);
    }

}



SoloGameUI::~SoloGameUI()
{
    delete ui;
}






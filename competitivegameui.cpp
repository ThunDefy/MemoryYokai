#include "competitivegameui.h"
#include "ui_competitivegameui.h"

#include "sologameui.h"
#include "Card.h"
#include "player.h"
#include "mainmenu.h"

#include <QGraphicsTextItem>
#include <QFont>
#include <QImage>
#include <QBrush>
#include <QFont>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QThread>

CompetitiveGameUi::CompetitiveGameUi(int setsize, int pcount)
{
    size = setsize;
    pairs_remain = size/2;
    playersCount = pcount;

    CreateField();
    setBackgroundBrush(QBrush(QImage(":/menu/src/Menu/CompBack.jpg")));

    for(int i=0;i<playersCount;i++){
        PlayersTextStatistics[i] = new QGraphicsTextItem();
        players[i] = new Player();
    }
    WinnerText = new QGraphicsTextItem();


    ShowPlayersFrames();
    ShowPlayersMasks();
    ShowPlayersScores();
    ResetPlayersScores();

    pause = new QPushButton();
    QPixmap pauseImage(":/menu/src/Menu/Pause.png");
    pauseImage = pauseImage.scaled(150,150);
    pause->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    pause->setIcon(pauseImage);
    pause->setIconSize(pauseImage.rect().size());
    pause->setGeometry(QRect(0,20,150,150));
    scene->addWidget(pause);
    connect(pause,SIGNAL(clicked()),this,SLOT(PushPauseBt()));

    pairs = new QGraphicsTextItem();
    //pairs->setPlainText(QString::number(pairs_remain)+" / "+QString::number(size/2));
    pairs->setDefaultTextColor(Qt::black);
    pairs->setFont(QFont("Kashima RUS by Cop",40));
    pairs->setPos(180,43);
    UpdateTextPairs();
    scene->addItem(pairs);

    endMenu = new QGraphicsPixmapItem();
    QPixmap endMenuImage(":/menu/src/Menu/EndMenuComp.png");
    endMenu->setPixmap(endMenuImage) ;
    endMenu->setPos(700,250);
    endMenu->hide();
    scene->addItem(endMenu);

}

void CompetitiveGameUi::ShowWhoseTurnNow()
{
    QPixmap yourTurn(":/menu/src/Menu/On.png");
    QPixmap notYourTurn(":/menu/src/Menu/Off.png");

    for(int i =0; i< playersCount; i++){
        if(i == nowTurn){
            PlayersMasks[i]->setPixmap(yourTurn);
        }else{
            PlayersMasks[i]->setPixmap(notYourTurn);
        }
    }

    qDebug()<<"Now Turn for player# "<<nowTurn;
}

void CompetitiveGameUi::UpdateTextPlayersScores()
{
    for(int i=0;i<playersCount;i++){
        PlayersScores[i]->setPlainText("ПО="+QString::number(players[i]->getScore()));
         qDebug()<<"Player # "<<i<<" has: "<<players[i]->getScore()<<" points";
    }

}

void CompetitiveGameUi::EndGameMenuShow()
{
    endMenu->show();
    pause->hide();
    for(int i=1;i<3;i++){
        pauseButtons[i]->show();
        pauseButtons[i]->setStyleSheet("background-color: rgba(255, 180, 255, 0);");
        pauseButtons[i]->setGeometry(QRect(720,510+(125*(i-1)),193,70));
    }
    Winner = WhoWin();
    ShowPlayersEndStatistics();

    qDebug()<<"Winner is:"<<Winner;

}

void CompetitiveGameUi::UpdateTextPairs()
{
    pairs->setPlainText(QString::number(pairs_remain)+" / "+QString::number(size/2));
}

void CompetitiveGameUi::ShowPlayersFrames()
{
    QPixmap frame;
    frame.load("../MemoryYokai/src/Menu/Рамка.png");
    int txt_margin_left;
    int txt_margin_top;

    switch (playersCount) {
    case 2:
        scale = 1;
        margin_left =5;
        margin_top = 300;
        indent_line = 290;

        txt_margin_left = 230;
        txt_margin_top = 130;

        break;
    case 3:
        scale = 1;
        margin_left =5;
        margin_top = 180;
        indent_line = 290;

        txt_margin_left = 230;
        txt_margin_top = 130;

        break;
    case 4:
        scale = 0.85;
        margin_left =20;
        margin_top = 175;
        indent_line = 225;

        txt_margin_left = 195;
        txt_margin_top = 105;

        break;
    default:
        break;
    }

    for(int i =0; i< playersCount; i++){
        PlayersFrames[i] = new QGraphicsPixmapItem();
        PlayersFrames[i]->setPixmap(frame);
        PlayersFrames[i]->setZValue(-1);
        PlayersFrames[i]->setScale(scale);
        PlayersFrames[i]->setPos(margin_left,margin_top+(indent_line*i));
        scene->addItem(PlayersFrames[i]);

        PlayersNums[i] = new QGraphicsTextItem();
        PlayersNums[i]->setPlainText(QString::number(i+1));
        PlayersNums[i]->setDefaultTextColor(Qt::black);
        PlayersNums[i]->setFont(QFont("Kashima RUS by Cop",50));
        PlayersNums[i]->setPos(txt_margin_left+margin_left,txt_margin_top+margin_top+(indent_line*i));
        //PlayersNums[i]->setPos(200+margin_left,150+margin_top+(indent_line*i)); Для ПО
        scene->addItem(PlayersNums[i]);


    }

}

void CompetitiveGameUi::ShowPlayersMasks()
{
    QPixmap yourTurn(":/menu/src/Menu/On.png");
    QPixmap notYourTurn(":/menu/src/Menu/Off.png");

    for(int i =0; i< playersCount; i++){
        PlayersMasks[i] = new QGraphicsPixmapItem();

        if(i == 0 )PlayersMasks[i]->setPixmap(yourTurn);
        else PlayersMasks[i]->setPixmap(notYourTurn);


        if(playersCount==4){
            PlayersMasks[i]->setScale(0.44);
            PlayersMasks[i]->setPos(margin_left-5,margin_top+(indent_line*i)+10);
        }
        else {
            PlayersMasks[i]->setScale(0.50);
            PlayersMasks[i]->setPos(margin_left,margin_top+(indent_line*i)+10);
        }
        scene->addItem(PlayersMasks[i]);
    }
}

void CompetitiveGameUi::ShowPlayersScores()
{
    for(int i =0; i< playersCount; i++){
        PlayersScores[i] = new QGraphicsTextItem();
        PlayersScores[i]->setDefaultTextColor(Qt::black);

        if(playersCount == 4){
            PlayersScores[i]->setFont(QFont("Kashima RUS by Cop",45));
            PlayersScores[i]->setPos(170+margin_left,160+margin_top+(indent_line*i));
        }
        else{
            PlayersScores[i]->setFont(QFont("Kashima RUS by Cop",50));
            PlayersScores[i]->setPos(200+margin_left,185+margin_top+(indent_line*i));
        }

        scene->addItem(PlayersScores[i]);
    }

}

void CompetitiveGameUi::ShowPlayersEndStatistics()
{

    WinnerText->setPlainText(QString::number(Winner+1));
    WinnerText->setDefaultTextColor(Qt::black);
    WinnerText->setFont(QFont("Kashima RUS by Cop",70));
    WinnerText->setPos(1415,450);
    WinnerText->show();
    scene->addItem(WinnerText);

    for(int i =0; i< playersCount; i++){
        PlayersTextStatistics[i]->setPlainText("Игрок#"+QString::number(i+1)+"\t\t  "+QString::number(players[i]->getScore())+"\t\t"+QString::number(players[i]->getTurns()));
        PlayersTextStatistics[i]->setDefaultTextColor(Qt::white);
        PlayersTextStatistics[i]->setFont(QFont("Kashima RUS by Cop",35));
        PlayersTextStatistics[i]->setPos(965,580+(50*i));
        PlayersTextStatistics[i]->show();
        scene->addItem(PlayersTextStatistics[i]);

    }

}

void CompetitiveGameUi::PassTurn()
{
    if(nowTurn == playersCount-1) nowTurn=0;
    else nowTurn +=1;
    ShowWhoseTurnNow();

}

int CompetitiveGameUi::WhoWin()
{
    int max=0;
    int winner=0;
    for(int i=0;i<playersCount;i++){
        if(players[i]->getScore() > max){
            winner = i;
            max = players[i]->getScore();
        }else if(players[i]->getScore() == max){
            if(players[i]->getTurns() < players[winner]->getTurns()){
                winner = i;
                max = players[i]->getScore();
            }else if(players[i]->getTurns() == players[winner]->getTurns()){
                if(i < winner){
                    winner = i;
                    max = players[i]->getScore();
                }
            }
        }
    }
    return winner;
}

void CompetitiveGameUi::ResetPlayersScores()
{
    nowTurn = 0;
    for(int i=0;i<playersCount;i++){
        players[i]->setScore(0);
        players[i]->setTurns(0);
    }
    UpdateTextPlayersScores();
}

void CompetitiveGameUi::PushRestartBt()
{
    if(isPause){
        PushPauseBt();
        RestartGame();
    }
    else{
        endMenu->hide();
        pause->show();
        RestartGame();
        canOpenNew = true;
        for(int i=1;i<3;i++){
            pauseButtons[i]->hide();
        }
    }
}

void CompetitiveGameUi::RestartGame()
{
    CleanOpenCards();  
    ResetPlayersScores();
    UpdateTextPairs();
    ShowWhoseTurnNow();

    for(int i = 0; i<size;i++){
         scene->removeItem(cards[i]);
    }

    pairs_remain = size/2;
    UpdateTextPairs();
    canOpenNew = true;

    for(int i =0; i< playersCount; i++){
        PlayersTextStatistics[i]->hide();
    }

    CreateField();

    WinnerText->hide();
    qDebug()<<"_Restart done";

}

void CompetitiveGameUi::TwinsCheck()
{

    if(OpenCards[0] == OpenCards[1] && OpenCards[2] != OpenCards[3]){
        qDebug()<<"Find twins!!";

        players[nowTurn]->makeTurn();

        cards[OpenCards[2]]->CardRemove();
        cards[OpenCards[3]]->CardRemove();
        CleanOpenCards();

        players[nowTurn]->gets1Point();
        UpdateTextPlayersScores();

        pairs_remain -=1;
        UpdateTextPairs();

        qDebug()<<"__Pairs remain = "<<pairs_remain;
        if(!checkRemain()){
            qDebug()<<"Game OVER";
            EndGameMenuShow();
        }

    }else if(OpenCards[0] != OpenCards[1] && OpenCards[2] != OpenCards[3]) {
        qDebug()<<"fail";
        players[nowTurn]->makeTurn();
        cards[OpenCards[2]]->CardFlip();
        cards[OpenCards[3]]->CardFlip();
        CleanOpenCards();
        PassTurn();
    }
    canOpenNew = true;
}



CompetitiveGameUi::~CompetitiveGameUi()
{
    delete ui;
}

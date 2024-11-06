#ifndef COMPETITIVEGAMEUI_H
#define COMPETITIVEGAMEUI_H

#include "card.h"
#include "player.h"
#include "sologameui.h"
#include "mainmenu.h"

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTime>
#include <QFont>
#include <QTimer>
#include <QGraphicsTextItem>

namespace Ui {
class CompetitiveGameUi;
}

class CompetitiveGameUi : public SoloGameUI
{
    Q_OBJECT

public:
     CompetitiveGameUi(int setsize=12, int pcount=2);


    ~CompetitiveGameUi();

private:

    int playersCount;
    Player* players [4];
    QGraphicsTextItem* WinnerText;
    QGraphicsTextItem* PlayersScores [4];
    QGraphicsTextItem* PlayersTextStatistics [4];
    QGraphicsTextItem* PlayersNums [4];
    QGraphicsPixmapItem* PlayersFrames[4];
    QGraphicsPixmapItem* PlayersMasks[4];

    int nowTurn=0;
    int Winner=0;

    void RestartGame() override;
    void EndGameMenuShow() override;
    void UpdateTextPairs() override;

    void ShowPlayersFrames();
    void ShowPlayersMasks();
    void ShowPlayersScores();

    void ShowPlayersEndStatistics();

    void PassTurn();
    void ShowWhoseTurnNow();

    int WhoWin();
    void ResetPlayersScores();

    void UpdateTextPlayersScores();

    Ui::CompetitiveGameUi *ui;

private slots:
    void PushRestartBt() override;
    void TwinsCheck() override;
};

#endif // COMPETITIVEGAMEUI_H

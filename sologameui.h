#ifndef SOLOGAMEUI_H
#define SOLOGAMEUI_H

#include "card.h"
#include "player.h"
//#include "competitivegameui.h"

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTime>
#include <QFont>
#include <QTimer>
#include <QGraphicsTextItem>

#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class SoloGameUI;
}

class SoloGameUI : public  QGraphicsView
{
    Q_OBJECT

public:

    int isSolo;

    int OpenCards[4];
    bool canOpenNew = true;

    void SetOpenCard(int cardInd, int ind);
    void CleanOpenCards();

    void keyPressEvent(QKeyEvent* event);

    explicit SoloGameUI(int setsize=12,int CreateSoloUi = 0);

    ~SoloGameUI();

private:          // Штуки только для соло

    QGraphicsTextItem* timeNow;
    QGraphicsTextItem* turns;
    Player* player;

    int timer= 0;
    int timerId;

    int sizeInd ;

    void timerEvent(QTimerEvent *evt);
    void controlTimer(int status);

    void UpdateTextTimeNow();
    void UpdateTextTurns();

    void CheckNewRecord();

    Ui::SoloGameUI *ui;

protected:       // Штуки для обоих режимов


    Card* cards[50];

    QGraphicsScene * scene;

    QGraphicsTextItem* pairs;
    virtual void UpdateTextPairs();

    QPushButton* pause;
    QGraphicsPixmapItem* pauseMenu;
    QPushButton* pauseButtons[3];
    QGraphicsPixmapItem* endMenu;

    bool isPause = false;

    int size ; // 50 40 30 24 20 16 12
    int pairs_remain;

    int DataField[50];
    void CreateField();

    int columns_count;
    int indent_column, indent_line;
    int margin_left, margin_top;
    double scale;

    bool checkRemain();

    virtual void RestartGame();
    virtual void EndGameMenuShow();

protected slots:
    virtual void TwinsCheck();
    void PushPauseBt();

    void PushContinueBt();
    virtual void PushRestartBt();
    void PushBackToMenuBt();

private slots:




};

#endif // SOLOGAMEUI_H

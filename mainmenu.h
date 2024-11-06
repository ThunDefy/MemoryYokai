#ifndef MAINMENU_H
#define MAINMENU_H

#include "card.h"
#include "player.h"
#include "helpwindow.h"

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
class MainMenu;
}

class MainMenu : public  QGraphicsView
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    QGraphicsScene * scene;

    ~MainMenu();

private:
    int size = 12;
    int playersCount;
    int gameMod = 0;

    QGraphicsTextItem* recordsText [7];

    QPushButton* startGameBt;
    QPushButton* selectSizeBt[7];
    QPushButton* selectPlayersBt[3];
    QPushButton* soloPlayBt;
    QPushButton* exitBt;
    QPushButton* showHelpBt;

    HelpWindow* HelpWind;

    void PushSelectSizeBt(int btInd);
    void PushSelectPlayersBt(int btInd);

    void UnSelectedAllSizeBt(int except =0);
    void UnSelectedAllPlayersBt(int except = -1);

    void SetOrUnsetSoloImg(int on=1);

    QSqlDatabase db;

    Ui::MainMenu *ui;

private slots:
    void PushStartGameBt();
    void PushSoloPlayBt();
    void PushExitBt();
    void ShowHelp();

};

#endif // MAINMENU_H

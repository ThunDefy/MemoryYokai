#ifndef CARD_H
#define CARD_H


#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlTableModel>

enum Visibility_type {OPEN, HIDDEN, EMPTY};

class Card: public QObject,public QGraphicsPixmapItem
{
public:
    Card(int cardInd,int imgInd);

    int card_ind;
    int img_ind;

    void CardRemove();
    void CardFlip();

    void mousePressEvent(QGraphicsSceneMouseEvent* );
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* );

private:
    QVariant name;
    QVariant description;
    void VisibilityChange();
    Visibility_type visibility = HIDDEN;


public slots:


};

#endif // CARD_H

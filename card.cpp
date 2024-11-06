#include "card.h"
#include "sologameui.h"
#include "competitivegameui.h"

#include <QThread>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QEvent>

extern SoloGameUI * soloUi;
extern CompetitiveGameUi* compUi;
extern QSqlTableModel* yokaiInf;

Card::Card(int cardInd,int imgInd)
{
    name = yokaiInf->data(yokaiInf->index(imgInd-1,1));
    description = yokaiInf->data(yokaiInf->index(imgInd-1,2));
    //qDebug()<<name.toString()<<" - "<<description.toString();
    card_ind = cardInd;
    img_ind = imgInd;
}

void Card::CardRemove(){
     visibility = EMPTY;
     VisibilityChange();
}
void Card::CardFlip(){
     visibility = HIDDEN;
     VisibilityChange();
}

void Card::VisibilityChange(){
    qDebug()<<"VisibilityChange";

    if(visibility == OPEN){
        //setToolTip(name.toString()+" - "+description.toString());
        setToolTip(("<style>p { margin: 0 0 0 0; font-size: 17px; color: #FFF5EE; background-color: #8B0000 }</style><p style='white-space:pre'>"+name.toString()+" - "+description.toString()));
        QString str_ind;
        str_ind.setNum(img_ind);
        setPixmap(QPixmap(":/cards/src/Cards/"+str_ind+".jpg"));
    }
    if(visibility == EMPTY)
        setPixmap(QPixmap(""));
    if(visibility == HIDDEN){
        setToolTip("???");
        setPixmap(QPixmap(":/cards/src/Cards/0.jpg"));
    }
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent * )
{
    if(soloUi->isSolo){
        if(soloUi->canOpenNew){
            qDebug()<<"card ind = "<<card_ind;
            qDebug()<<"img ind = "<<img_ind;
            if(visibility == OPEN){
                //visibility = HIDDEN;
                //VisibilityChange();
                //soloUi->CleanOpenCards();
            }
            else if (visibility = HIDDEN){
                visibility = OPEN;
                VisibilityChange();
                soloUi->SetOpenCard(card_ind,img_ind);
            }
        }
    }else{
        if(compUi->canOpenNew){
            qDebug()<<"card ind = "<<card_ind;
            qDebug()<<"img ind = "<<img_ind;
            if(visibility == OPEN){
//                visibility = HIDDEN;
//                VisibilityChange();
//                compUi->CleanOpenCards();
            }
            else if (visibility = HIDDEN){
                visibility = OPEN;
                VisibilityChange();
                compUi->SetOpenCard(card_ind,img_ind);
            }
        }
    }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent * )
{
    //qDebug()<<"You release button";
}

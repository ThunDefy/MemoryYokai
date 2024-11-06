#include "sologameui.h"
#include "mainmenu.h"

#include <QApplication>
#include <QSplashScreen>
#include <QtGlobal>

void LoadModules(QSplashScreen* psplash){
    QTimer time;
    time.start(15);

    for(int i=0; i<100 ; ){
        if(time.remainingTime() == 0){
            time.start(15);
            ++i;
        }
        psplash->showMessage("Loading modules: " + QString::number(i)+"%",
                             Qt::AlignRight | Qt::AlignBottom, Qt::black);
        qApp->processEvents();
    }
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QSplashScreen splash(QPixmap("../MemoryYokai/src/Menu/Start.png"));
    MainMenu* mainMenu= new MainMenu();

//    splash.show();
//    LoadModules(&splash);
//    splash.finish(mainMenu);

    mainMenu->showFullScreen();
   //mainMenu->show();

    return a.exec();
}

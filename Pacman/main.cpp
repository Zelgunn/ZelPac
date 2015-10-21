#include "pacscreen.h"
#include <QApplication>

/** \mainpage Documentation de Pacman
 *
 *
 *
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qsrand(QTime::currentTime().msec());

    Q_ASSERT(argc != 2);
    Pacscreen *pc;
    if(argc == 2)
    {
        pc = new Pacscreen(argv[1]);
    }
    else
    {
        pc = new Pacscreen;
    }
    pc->show();

    return a.exec();
}

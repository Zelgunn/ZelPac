#include "pacscreen.h"
#include <QApplication>

/** \mainpage Documentation de Pacman
 *
 * \section to_do To Do
 * - La classe Game ne gère pas encore le passage d'un niveau à l'autre.
 * - La classe Game ne gère pas encore le son.
 * - La classe Game ne dispose pas encore de toutes les animations.
 * - La classe Game ne dispose pas encore de la table de correspondance des textures.
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

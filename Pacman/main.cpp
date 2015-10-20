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

    Pacscreen pc;
    pc.show();

    return a.exec();
}

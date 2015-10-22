#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

#include "map.h"

/**
 * @brief Classe graphique remplaçant la vue de Qt pour propager les événements.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La classe View propage les événements de molette de souris avec le signal wheelEventOnView.
 */
class View : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base de View
     * @param parent Parent de la fenêtre.
     */
    View(QWidget * parent = 0);

    /**
     * Destructeur de View.
     */
    ~View();

protected:
    /**
     * @brief Surchage de la fonction d'événement de molette de la souris.
     * @param event Paramètre généré par l'événement.
     */
    void wheelEvent(QWheelEvent *event);

signals:
    /**
     * @brief Signal émit lors de l'événement de molette de la souris.
     * @param event Paramètre généré par l'événement.
     */
    void wheelEventOnView(QWheelEvent *event);
};

#endif // VIEW_H

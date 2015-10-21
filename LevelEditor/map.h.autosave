#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include <QDebug>

/**
 * @brief Objet graphique détectant les clics souris et générant des signaux.
 * @author Valentin D.d.G.
 * @version 1.0
 */
class Map : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base de Map.
     */
    Map();

protected:
    /**
     * @brief Surchage de la fonction d'événement d'appui de la souris.
     * @param event Paramètre généré par l'événement.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    /**
     * @brief Signal émit lorsque l'on clic sur l'objet graphique.
     * @param event Paramètre généré par l'événement.
     */
    void mapClicked(QGraphicsSceneMouseEvent *event);
};

#endif // MAP_H

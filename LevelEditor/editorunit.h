#ifndef EDITORUNIT_H
#define EDITORUNIT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

/**
 * @brief Petite classe graphique permettant de bouger un pion.
 * @author Valentin D.d.G.
 * @version 1.0
 */

class EditorUnit : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base d'EditorUnit.
     * @param parent Parent de l'objet graphique.
     */
    EditorUnit(QGraphicsItem *parent = 0);

    /**
     * @brief Constructeur avec image d'EditorUnit.
     * @param pixmap Image à utiliser
     * @param parent Parent de l'objet graphique.
     */
    EditorUnit(const QPixmap &pixmap, QGraphicsItem *parent = 0);

signals:
    /**
     * @brief Signal émit quand le pion est déplacé.
     */
    void unitMoved();

protected:
    /**
     * @brief Surchage de la fonction d'événement de relâchement de la souris.
     * @param event Paramètre généré par l'événement.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // EDITORUNIT_H

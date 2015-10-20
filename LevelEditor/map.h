#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include <QDebug>

class Map : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Map();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void mapClicked(QGraphicsSceneMouseEvent *event);
};

#endif // MAP_H

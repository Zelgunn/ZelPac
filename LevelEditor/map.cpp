#include "map.h"

Map::Map()
{

}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mapClicked(event);
    QGraphicsPixmapItem::mousePressEvent(event);
}

#include "editorunit.h"

EditorUnit::EditorUnit(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    setFlags(QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemSendsGeometryChanges);
}

EditorUnit::EditorUnit(const QPixmap &pixmap, QGraphicsItem *parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

void EditorUnit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit unitMoved();
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

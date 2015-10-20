#ifndef EDITORUNIT_H
#define EDITORUNIT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

class EditorUnit : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    EditorUnit(QGraphicsItem *parent = 0);
    EditorUnit(const QPixmap &pixmap, QGraphicsItem *parent = 0);

signals:
    void unitMoved();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // EDITORUNIT_H

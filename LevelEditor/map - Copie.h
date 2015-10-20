#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QSize>
#include <QFile>

#include <QDebug>

class Map : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Map(QSize tilesCount, QSize tilesSize, QList<QImage> *textures, QObject *parent = 0);
    Map(QStringList rawData, QSize tilesSize(), QList<QImage> *textures, QObject *parent = 0);

    QPoint posToTile(QPoint p);
    void saveGrid(QString filename);

    QImage *setTileAt(QImage *image, QPointF p, int textureIdx);
    QSize tilesCount() const;
    void setTilesCount(const QSize &tilesCount);
    QSize tilesSize() const;
    void setTilesSize(const QSize &tilesSize);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    mousePressOnMap(QPoint pos);

private:
    QSize m_tilesCount;
    QSize m_tilesSize;
    int *m_grid;
    QList<QImage> *m_textures;
    int m_textureIdx;
};

#endif // MAP_H

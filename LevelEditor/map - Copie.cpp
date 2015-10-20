#include "map.h"

Map::Map(QSize tilesCount, QSize tilesSize, QList<QImage> *textures, QObject *parent) : QObject(parent)
{
    m_tilesCount = tilesCount;
    m_tilesSize = tilesSize;
    m_grid = (int*)malloc(sizeof(int) * tilesCount.width() * tilesCount.height());
    for(int i = 0; i<tilesCount.width() * tilesCount.height(); i++)
        m_grid[i] = -1;
    m_textures = textures;
    m_textureIdx = -1;

    int width = 1 + m_tilesCount.width() * (tilesSize.width()+1);
    int height = 1 + m_tilesCount.height() * (tilesSize.height()+1);

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::white);

    for(int i=0; i<width; i++)
    {
        for(int j=0; j<=m_tilesCount.height(); j++)
        {
            image.setPixel(i, j*(tilesSize.height()+1), qRgb(0,0,0));
        }
        image.setPixel(i, height-1, qRgb(0,0,0));
    }

    for(int j=0; j<height; j++)
    {
        for(int i=0; i<=m_tilesCount.width(); i++)
        {
            image.setPixel(i*(tilesSize.width()+1), j, qRgb(0,0,0));
        }
    }

    setPixmap(QPixmap::fromImage(image));
}

Map::Map(QStringList rawData, QSize tilesSize, QList<QImage> *textures, QObject *parent)
{

}

QPoint Map::posToTile(QPoint p)
{
    QPoint res;
    res.setX(p.x()/(m_tilesSize.width()+1));
    res.setY(p.y()/(m_tilesSize.height()+1));
    return res;
}

void Map::saveGrid(QString filename)
{
    QFile file(filename);
    Q_ASSERT(file.open(QIODevice::WriteOnly));
    QTextStream out(&file);
    for(int j=0; j<m_tilesCount.height(); j++)
    {
        for(int i=0; i<m_tilesCount.width(); i++)
        {
            out << (char) ('a' + m_grid[i+j*m_tilesCount.width()]);
        }
        out << "\r\n";
    }

    file.close();
}

QImage *Map::setTileAt(QImage *image, QPointF p, int textureIdx)
{
    if((textureIdx<0)||textureIdx>=m_textures->size())
        return image;
    m_grid[(int)(p.x() + p.y() * m_tilesCount.width())] = textureIdx;
    QImage texture = m_textures->at(textureIdx).scaled(m_tilesSize.width(), m_tilesSize.height());
    for(int j=0; j<m_tilesSize.height(); j++)
        for(int i=0; i<m_tilesSize.width(); i++)
            image->setPixel(p.x()*(m_tilesSize.width()+1) + i +1,p.y()*(m_tilesSize.height()+1) +j +1, texture.pixel(i,j));
    return image;
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint p = posToTile(event->pos().toPoint());

    emit mousePressOnMap(p);

    QGraphicsPixmapItem::mousePressEvent(event);
}
QSize Map::tilesSize() const
{
    return m_tilesSize;
}

void Map::setTilesSize(const QSize &tilesSize)
{
    m_tilesSize = tilesSize;
}

QSize Map::tilesCount() const
{
    return m_tilesCount;
}

void Map::setTilesCount(const QSize &tilesCount)
{
    m_tilesCount = tilesCount;
}



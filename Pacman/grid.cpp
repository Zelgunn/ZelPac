#include "grid.h"

Grid::Grid(QDomElement elem, TextureHandler *textureHandler)
{
    m_textureHandler = textureHandler;
    QDomNode node = elem.firstChild();

    QString s;

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            if(elem.tagName() == "TileCount")
            {
                m_size.setWidth(elem.attribute("width", "10").toInt());
                m_size.setHeight(elem.attribute("height", "10").toInt());
            }

            if(elem.tagName() == "GridValues")
            {
                s = elem.attribute("values", "");
                m_gridValues = (int*)malloc(sizeof(int)*s.size());

                for(int i=0; i<s.size(); i++)
                    m_gridValues[i] = (int)(s.at(i).toLatin1() - 'a');
            }

            if(elem.tagName() == "Texture")
            {
                addTexture(elem.attribute("filename"));
            }

            if(elem.tagName() == "CollisionsGrid")
            {
                s = elem.attribute("values", "");
                m_collisionGrid = (int*)malloc(sizeof(int)*s.size());
                for(int i=0; i<s.size(); i++)
                    m_collisionGrid[i] = (int)(s.at(i).toLatin1() - '0');
            }

            if(elem.tagName() == "GhostHouse")
            {
                m_ghostHouse.setX(elem.attribute("x").toInt());
                m_ghostHouse.setY(elem.attribute("y").toInt());
                m_ghostHouse.setWidth(elem.attribute("width").toInt());
                m_ghostHouse.setHeight(elem.attribute("height").toInt());
            }
        }
        // Itération
        node = node.nextSibling();
    }

    QList<QPointF> basePos;
    basePos.append(QPointF(13.5,11));
    basePos.append(QPointF(12,14));
    basePos.append(QPointF(13.5,14));
    basePos.append(QPointF(15,14));

    int width = m_size.width()*m_textureHandler->tilesSize();
    int height = m_size.height()*m_textureHandler->tilesSize();

    QPixmap image(width, height);
    image.fill(Qt::white);

    processBaseImage();
}

Grid::~Grid()
{
    delete m_gridValues;
    delete m_collisionGrid;
}

QSize Grid::size() const
{
    return m_size;
}

void Grid::addTexture(QString filename)
{
    QPixmap tmp(filename);
    if(tmp.isNull()) return;
    tmp = tmp.scaled(QSize(m_textureHandler->tilesSize(), m_textureHandler->tilesSize()));

    m_textures.append(tmp);
}

QPixmap Grid::textureAt(int index)
{
    return m_textureHandler->textureAt(index);
}

int Grid::tileTexture(QPoint p)
{
    int idx = (int)(p.x() + p.y()*m_size.width());
    return m_gridValues[idx];
}

int Grid::tileTexture(int i)
{
    return m_gridValues[i];
}

void Grid::setTileTexture(QPoint p, int texture)
{
    int index = p.x() + p.y()*m_size.width();
    if(m_gridValues[index]!=texture)
    {
        m_changedPoints.append(p);
        m_gridValues[index] = texture;
    }
}

QPixmap Grid::processBaseImage()
{
    int width = m_size.width()*m_textureHandler->tilesSize();
    int height = m_size.height()*m_textureHandler->tilesSize();

    m_baseImage = QPixmap(width, height);
    m_baseImage.fill(Qt::blue);
    QPixmap texture;
    QPainter painter(&m_baseImage);
    int index;


    for(int y=0; y<m_size.height(); y++)
    {
        for(int x=0; x<m_size.width(); x++)
        {
            index = m_gridValues[x+y*m_size.width()];
            texture = m_textureHandler->textureAt(index);
            painter.drawPixmap(x*m_textureHandler->tilesSize(), y*m_textureHandler->tilesSize(), texture);
        }
    }

    painter.end();

    return m_baseImage;
}

QPixmap Grid::updateBaseImage()
{
    QPoint p;
    int x,y;
    QPixmap texture;
    QPainter painter(&m_baseImage);

    for(int k=0; k<m_changedPoints.size(); k++)
    {
        p = m_changedPoints.at(k);
        x = p.x()*m_textureHandler->tilesSize();
        y = p.y()*m_textureHandler->tilesSize();
        texture = textureAt(tileTexture(p));
        painter.drawPixmap(x, y, texture);
    }
    m_changedPoints.clear();

    painter.end();
    return m_baseImage;
}
QRect Grid::ghostHouse() const
{
    return m_ghostHouse;
}

int Grid::emptyTile() const
{
    return m_emptyTile;
}

void Grid::setEmptyTile(int emptyTile)
{
    m_emptyTile = emptyTile;
}

int Grid::pelletTile() const
{
    return m_pelletTile;
}

void Grid::setPelletTile(int pelletTile)
{
    m_pelletTile = pelletTile;
}

int Grid::energizerTile() const
{
    return m_energizerTile;
}

void Grid::setEnergizerTile(int energizerTile)
{
    m_energizerTile = energizerTile;
}

int Grid::tilesSize() const
{
    return m_textureHandler->tilesSize();
}

int Grid::collisionAt(QPoint pos) const
{
    if(pos.x() < 0) pos.setX(m_size.width() -1);
    if(pos.x() >= m_size.width()) pos.setX(0);
    int idx = pos.x() + pos.y()*m_size.width();
    return m_collisionGrid[idx];
}

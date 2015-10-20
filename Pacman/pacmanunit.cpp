#include "pacmanunit.h"

PacmanUnit *pacman = Q_NULLPTR;

PacmanUnit::PacmanUnit(QObject *parent)
    : QObject(parent)
{

}

PacmanUnit::PacmanUnit(Grid *grid, QDomElement elem, QObject *parent)
    : QObject(parent), Unit(grid, elem)
{
    m_frame = 0;
    m_nextDirection = -1;
    m_direction = Unit::Right;
}

void PacmanUnit::nextFrame()
{
    m_frame = (m_frame+1)%PACMAN_FRAMES_COUNT;
}

void PacmanUnit::setFrame(int frame)
{
    m_frame = frame;
}

int PacmanUnit::frame() const
{
    return m_frame;
}

void PacmanUnit::setNextDirection(int nextDirection)
{
    m_nextDirection = nextDirection;
}

void PacmanUnit::move()
{
    if(!m_position.isFloatPositionNull())
    {
        // Déplacement au sein de la case
        int speed = m_speed;
        if((m_direction == Unit::Up)||(m_direction == Unit::Left))
            speed = -speed;
        if(m_position.floatPosition().x() == 0)
            m_position.increaseY(speed);
        else
            m_position.increaseX(speed);
        return;
    }
    // Prochaine case libre ?
    if(m_nextDirection >= 0)
    {
        if(isMoveLegal(m_nextDirection))
        {
            m_direction = m_nextDirection;
            m_nextDirection = -1;
        }
    }

    int tileType = m_grid->tileTexture(m_position.integerPosition());

    if(tileType == m_grid->pelletTile())
    {
        emit pelletEaten();
        m_grid->setTileTexture(m_position.integerPosition(), m_grid->emptyTile());
    }

    if(tileType == m_grid->energizerTile())
    {
        emit energizerEaten();
        m_grid->setTileTexture(m_position.integerPosition(), m_grid->emptyTile());
    }

    if(isMoveLegal(m_direction))
        moveTo(m_direction);
}

void PacmanUnit::setDirection(int direction)
{
    bool tmp = false;

    if(isMoveLegal(direction))
    {
        if(m_position.isFloatPositionNull())
            tmp = true;
        if((m_position.floatPosition().x() != 0)
                &&((direction == Unit::Left)||(direction == Unit::Right)))
            tmp = true;
        if((m_position.floatPosition().y() != 0)
                &&((direction == Unit::Up)||(direction == Unit::Down)))
            tmp = true;
    }


    if(tmp)
    {
        m_direction = direction;
        m_nextDirection = DirectionNotValid;
    }
    else
        m_nextDirection = direction;
}

void PacmanUnit::resetPosition()
{
    Unit::resetPosition();
    m_direction = Unit::Right;
}

QPixmap PacmanUnit::renderPacmanImage(int size)
{
    QPixmap res(size, size);
    QColor tmpColor(255,255,255,0);
    res.fill(tmpColor);

    int baseAngle;
    int deltaAngle;
    if(m_frame<PACMAN_FRAMES_COUNT/2)
        deltaAngle = 60 - 120*m_frame/PACMAN_FRAMES_COUNT;
    else
        deltaAngle = 120*(m_frame-PACMAN_FRAMES_COUNT/2)/PACMAN_FRAMES_COUNT;

    switch (m_direction) {
    case Up:
        baseAngle = 90;
        break;
    case Down:
        baseAngle = 270;
        break;
    case Left:
        baseAngle = 180;
        break;
    default:
        baseAngle = 0;
        break;
    }

    QPainter painter(&res);
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QBrush(QColor(255,255,0)));
    painter.drawPie(QRectF(0,0,size,size),(baseAngle + deltaAngle)*16,(360 - deltaAngle*2)*16);

    return res;
}

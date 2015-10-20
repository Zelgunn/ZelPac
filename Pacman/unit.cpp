#include "unit.h"

Unit::Unit() :
    m_direction(Right), m_grid(Q_NULLPTR)
{

}

Unit::Unit(Grid *grid) :
    m_direction(Right), m_grid(grid)
{

}

Unit::Unit(Grid *grid, QDomElement elem)
{
    m_grid = grid;
    m_position = DoublePosition(elem);
    m_origine = m_position;
    m_speed = elem.attribute("speed").toInt();
    m_baseSpeed = elem.attribute("speed").toInt();
    m_scaredSpeed = elem.attribute("sspeed").toInt();
}

QPoint Unit::position() const
{
    return m_position.integerPosition();
}

QPoint Unit::positionInTile() const
{
    return m_position.floatPosition();
}

DoublePosition Unit::fullPosition() const
{
    return m_position;
}

void Unit::setDirection(int direction)
{
    m_direction = direction;
}

int Unit::direction() const
{
    return m_direction;
}

char Unit::speed() const
{
    return m_speed;
}

void Unit::setSpeed(char speed)
{
    m_speed = speed;
}

char Unit::scaredSpeed() const
{
    return m_scaredSpeed;
}

void Unit::setScaredSpeed(char scaredSpeed)
{
    m_scaredSpeed = scaredSpeed;
}

char Unit::baseSpeed() const
{
    return m_baseSpeed;
}

void Unit::setBaseSpeed(char baseSpeed)
{
    m_baseSpeed = baseSpeed;
}

void Unit::reverseSpeeds(bool reverse)
{
    if(reverse)
        m_speed = m_scaredSpeed;
    else
        m_speed = m_baseSpeed;
}

bool Unit::isMoveLegal(int direction)
{
    QPoint nextTile = m_position.integerPosition() + Unit::directionToMove(direction);
    if(nextTile.x() < 0) nextTile.setX(m_grid->size().width() -1);
    if(nextTile.x() >= m_grid->size().width()) nextTile.setX(0);
    return (m_grid->collisionAt(nextTile) != Grid::CollisionOn);
}

void Unit::moveToTile(QPoint tile)
{
    m_position.setIntegerPosition(tile);
    m_position.setFloatPosition(QPoint(0,0));
}

void Unit::moveToTile(int direction)
{
    QPoint nextTile = m_position.integerPosition() + directionToMove(direction);
    moveToTile(nextTile);
}

void Unit::moveTo(int direction)
{
    switch (direction) {
    case Unit::Up:
        m_position.increaseY(-m_speed);
        break;
    case Unit::Down:
        m_position.increaseY(m_speed);
        break;
    case Unit::Left:
        m_position.increaseX(-m_speed);
        break;
    case Unit::Right:
        m_position.increaseX(m_speed);
        break;
    }
    if(m_position.integerPosition().x() < 0) m_position.setIntegerPositionX(m_grid->size().width() -1);
    else if(m_position.integerPosition().x() >= m_grid->size().width()) m_position.setIntegerPositionX(0);
}

void Unit::resetPosition()
{
    m_position = m_origine;
    m_direction = Unit::Left;
}

QPoint Unit::directionToMove(int direction)
{
    QPoint move(0,0);
    switch (direction) {
    case Unit::Up:
        move.setY(-1);
        break;
    case Unit::Down:
        move.setY(1);
        break;
    case Unit::Left:
        move.setX(-1);
        break;
    case Unit::Right:
        move.setX(1);
        break;
    }

    return move;
}

qreal Unit::euclidianDistance(QPoint p1, QPoint p2)
{
    return qSqrt(qPow((p1.x() - p2.x()),2) + qPow((p1.y() - p2.y()),2));
}



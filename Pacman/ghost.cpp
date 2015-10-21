#include "ghost.h"

bool _ghostsScared = false;

Ghost *blinky = Q_NULLPTR;
Ghost *pinky = Q_NULLPTR;
Ghost *inky = Q_NULLPTR;
Ghost *clyde = Q_NULLPTR;

Ghost::Ghost()
{
    m_id = Blinky;
    m_free = false;
    m_previousDirection = Unit::Right;
    m_scatterCorner = QPoint(0,0);
}

Ghost::Ghost(Grid *grid, int id)
    : Unit(grid)
{
    m_state = Scatter;
    m_direction = Unit::Left;
    m_id = id;
    m_free = false;
    m_previousDirection = Unit::Right;
    m_scatterCorner = QPoint(0,0);
}

Ghost::Ghost(Grid *grid, QDomElement elem, int id)
    : Unit(grid, elem)
{
    m_state = Scatter;
    m_direction = Unit::Left;
    m_id = id;
    m_free = false;
    m_previousDirection = Unit::Right;
    switch(id)
    {
    case Blinky:
        m_scatterCorner.setX(26);
        m_scatterCorner.setY(-2);
        break;
    case Pinky:
        m_scatterCorner.setX(2);
        m_scatterCorner.setY(-2);
        break;
    case Inky:
        m_scatterCorner.setX(27);
        m_scatterCorner.setY(31);
        break;
    case Clyde:
        m_scatterCorner.setX(0);
        m_scatterCorner.setY(31);
        break;
    }
}

bool Ghost::isInsideGhostHouse() const
{
    return m_grid->ghostHouse().contains(m_position.integerPosition());
}

void Ghost::move()
{
    if(isInsideGhostHouse())
    {
        if(isFree())
        {
            if(!isCaught())
            {
                release();
            }
        }
    }
    else if(isOnTheMove()) localMove();
    else if(isCaught()) goHome();
    else if(areGhostsScared()) escape();
    else if(isScattering()) scatter();
    else chase();
}

void Ghost::reverseDirection()
{
    if(!isInsideGhostHouse())
    {
        m_previousDirection = m_direction;
        m_direction = (m_direction/2)*2 + (m_direction+1)%2;
    }
}

int Ghost::findNextdirection(QPoint tileTarget, bool ignorePreviousDirection)
{
    QList<int> availableDirections;
    QList<QPoint> availableMoves;
    QPoint tmpPoint;
    DoublePosition tmpPosition;

    int x,y, collision;

    for(int i=0; i<4; i++)
    {
        x = (i/2)*(-1 + 2*(i%2));
        y = (1 - (i/2))*(-1 + 2*(i%2));

        tmpPosition = m_position;
        tmpPosition.increaseX(m_speed * x);
        tmpPosition.increaseY(m_speed * y);
        tmpPoint = m_position.integerPosition() + QPoint(x,y);

        collision = m_grid->collisionAt(tmpPoint);

        if((collision != Grid::CollisionOn)&&((i != m_previousDirection)||(ignorePreviousDirection)))
        {
            availableDirections.append(i);
            availableMoves.append(tmpPoint);
        }
    }
    if(availableDirections.size() == 1)
        return availableDirections.first();

    int minIdx = -1;
    qreal dist, min = 10000.0;
    for(int i=0; i<availableDirections.size(); i++)
    {
        dist = euclidianDistance(tileTarget, availableMoves.at(i));
        if(min>dist)
        {
            min = dist;
            minIdx = i;
        }
    }
    if(minIdx == -1) return -1;
    return availableDirections.at(minIdx);
}

int Ghost::state() const
{
    return m_state;
}

void Ghost::setState(int state)
{
    if(m_state != state)
        reverseDirection();
    m_state = state;
}

bool Ghost::isFree() const
{
    return m_free;
}

void Ghost::setFree(bool free)
{
    m_free = free;
}

bool Ghost::isOnTheMove() const
{
    return !(m_position.isFloatPositionNull());
}

bool Ghost::isChasing() const
{
    return (m_state == Ghost::Chase);
}

bool Ghost::isScattering() const
{
    return (m_state == Ghost::Scatter);
}

void Ghost::setCaught()
{
    int nextDirection = findNextdirection(QPoint(13, 14), true);
    m_direction = nextDirection;
    m_state = Caught;
}

bool Ghost::isCaught() const
{
    return m_state == Caught;
}

QPoint Ghost::scatterCorner() const
{
    return m_scatterCorner;
}

void Ghost::setGhostsScared(bool scared)
{
    _ghostsScared = scared;
}

bool Ghost::areGhostsScared()
{
    return _ghostsScared;
}

QPixmap Ghost::ghostImage(int size)
{
    QString s;
    switch (m_id) {
    case Ghost::Blinky:
        s = ":/textures/images/Ghost_Blinky.png";
        break;
    case Ghost::Pinky:
        s = ":/textures/images/Ghost_Pinky.png";
        break;
    case Ghost::Inky:
        s = ":/textures/images/Ghost_Inky.png";
        break;
    case Ghost::Clyde:
        s = ":/textures/images/Ghost_Clyde.png";
        break;
    }
    QPoint eyesOrigin;
    switch (m_direction) {
    case Unit::Up: eyesOrigin = QPoint(21,27);
        break;
    case Unit::Down: eyesOrigin = QPoint(21,49);
        break;
    case Unit::Left: eyesOrigin = QPoint(14,42);
        break;
    case Unit::Right: eyesOrigin = QPoint(28,42);
        break;
    }
    QImage res(s);

    QColor eyeColor(0,0,128,255);
    for(int i=0; i<12; i++)
    {
        for(int j=0; j<12; j++)
        {
            res.setPixel(eyesOrigin.x()+i, eyesOrigin.y()+j, eyeColor.rgba());
            res.setPixel(eyesOrigin.x()+i+41, eyesOrigin.y()+j, eyeColor.rgba());
        }
    }
    QSize tmp(size, size);
    if(res.size()!=tmp) res = res.scaled(tmp);

    return QPixmap::fromImage(res);
}

QPixmap Ghost::ghostEyesImage(int size)
{
    QPoint eyesOrigin;
    switch (m_direction) {
    case Unit::Up: eyesOrigin = QPoint(21,27);
        break;
    case Unit::Down: eyesOrigin = QPoint(21,49);
        break;
    case Unit::Left: eyesOrigin = QPoint(14,42);
        break;
    case Unit::Right: eyesOrigin = QPoint(28,42);
        break;
    }
    QImage res(":/textures/images/Ghost_Eyes.png");

    QColor eyeColor(0,0,128,255);
    for(int i=0; i<12; i++)
    {
        for(int j=0; j<12; j++)
        {
            res.setPixel(eyesOrigin.x()+i, eyesOrigin.y()+j, eyeColor.rgba());
            res.setPixel(eyesOrigin.x()+i+41, eyesOrigin.y()+j, eyeColor.rgba());
        }
    }
    QSize tmp(size, size);
    if(res.size()!=tmp) res = res.scaled(tmp);

    return QPixmap::fromImage(res);
}

QPixmap Ghost::scaredGhostImage(bool flashOn, int size)
{
    QPixmap res;
    if(flashOn)
        res = QPixmap(":/textures/images/Ghost_Flash.png");
    else
        res = QPixmap(":/textures/images/Ghost_Blue.png");
    QSize tmp(size, size);
    return res.scaled(tmp);
}

QPoint Ghost::blinkyChaseTarget()
{
    return pacman->position();
}

QPoint Ghost::pinkyChaseTarget()
{
    QPoint pacmanTarget = pacman->position();
    pacmanTarget += directionToMove(pacman->direction()) * 4;
    return pacmanTarget;
}

QPoint Ghost::inkyChaseTarget()
{
    QPoint inkyTarget = pacman->position() + directionToMove(pacman->direction()) * 2;
    inkyTarget = inkyTarget * 2 - blinky->position();
    return inkyTarget;
}

QPoint Ghost::clydeChaseTarget()
{
    QPoint clydeTarget = pacman->position();
    if(euclidianDistance(clyde->position(), clydeTarget) < 8)
        clydeTarget = clyde->scatterCorner();
    return clydeTarget;
}

void Ghost::localMove()
{
    int speed = m_speed;
    if((m_direction == Unit::Up)||(m_direction == Unit::Left))
        speed = -speed;
    if(m_position.floatPosition().y() == 0)
        m_position.increaseX(speed);
    else
        m_position.increaseY(speed);
}

void Ghost::chase()
{
    QPoint tileTarget;
    switch (m_id) {
    case Blinky:
        tileTarget = blinkyChaseTarget();
        break;
    case Pinky:
        tileTarget = pinkyChaseTarget();
        break;
    case Inky:
        tileTarget = inkyChaseTarget();
        break;
    case Clyde:
        tileTarget = clydeChaseTarget();
        break;
    }

    int nextDirection = findNextdirection(tileTarget);
    if(nextDirection != m_direction)
    {
        m_direction = nextDirection;
        m_previousDirection = (m_direction/2)*2 + (m_direction+1)%2;
    }
    moveTo(nextDirection);
}

void Ghost::scatter()
{
    int nextDirection = findNextdirection(m_scatterCorner);
    if(nextDirection != m_direction)
    {
        m_direction = nextDirection;
        m_previousDirection = (m_direction/2)*2 + (m_direction+1)%2;
    }
    moveTo(nextDirection);
}

void Ghost::escape()
{
    int nextDirection = m_previousDirection;
    while((nextDirection == m_previousDirection)||(!isMoveLegal(nextDirection)))
    {
        nextDirection = qrand()%4;
    }
    if(nextDirection != m_direction)
    {
        m_direction = nextDirection;
        m_previousDirection = (m_direction/2)*2 + (m_direction+1)%2;
    }
    moveTo(nextDirection);
}

void Ghost::goHome()
{
    if(m_position.integerPosition().y() == 11)
    {
        if((m_position.integerPosition().x() >= 12)
                &&(m_position.integerPosition().x() <= 13))
        {
            m_position.setIntegerPosition(QPoint(14, 14));
            m_position.setFloatPosition(QPoint(MAX_FLOAT_POS/2, 0));
        }
    }
    int nextDirection = findNextdirection(QPoint(13, 11));
    if(nextDirection != m_direction)
    {
        m_direction = nextDirection;
        m_previousDirection = (m_direction/2)*2 + (m_direction+1)%2;
    }
    moveToTile(nextDirection);
}

void Ghost::release()
{
    m_direction = Unit::Left;
    DoublePosition houseCenter;
    houseCenter.setFloatPosition(QPoint(500,0));
    houseCenter.setIntegerPosition(QPoint(13,14));

    if(m_position.integerPosition().x() == 13)
    {
        if(qAbs(m_position.floatPosition().x() - (MAX_FLOAT_POS/2)) < 25)
        {
            m_position.setFloatPositionX(MAX_FLOAT_POS/2);
            moveTo(Unit::Up);
        }
        else if(m_position.floatPosition().x() < (MAX_FLOAT_POS/2))
            moveTo(Unit::Right);
        else
            moveTo(Unit::Left);
    }
    else if(m_position.integerPosition().x() < 13)
        moveTo(Unit::Right);
    else
        moveTo(Unit::Left);
}





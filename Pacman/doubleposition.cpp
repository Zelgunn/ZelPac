#include "doubleposition.h"

DoublePosition::DoublePosition()
{
    m_integerPosition = QPoint(0,0);
    m_floatPosition = QPoint(0,0);
}

DoublePosition::DoublePosition(QPoint intPos, QPoint floatPos)
 : m_integerPosition(intPos), m_floatPosition(floatPos)
{

}

DoublePosition::DoublePosition(QDomElement elem)
{
    QPoint tmpPoint;
    tmpPoint.setX(elem.attribute("ix").toInt());
    tmpPoint.setY(elem.attribute("iy").toInt());
    m_integerPosition = tmpPoint;

    if(elem.attribute("fx") == "half")
        tmpPoint.setX(MAX_FLOAT_POS/2);
    else
        tmpPoint.setX(0);
    if(elem.attribute("fy") == "half")
        tmpPoint.setY(MAX_FLOAT_POS/2);
    else
        tmpPoint.setY(0);
    m_floatPosition = tmpPoint;
}

QPoint DoublePosition::integerPosition() const
{
    return m_integerPosition;
}

QPoint DoublePosition::floatPosition() const
{
    return m_floatPosition;
}

bool DoublePosition::isFloatPositionNull() const
{
    return ((m_floatPosition.x() == 0) && (m_floatPosition.y() == 0));
}

void DoublePosition::increaseX(int pace)
{
    int tmp = m_floatPosition.x() + pace;
    if(tmp >= MAX_FLOAT_POS)
    {
        tmp = 0;
        m_integerPosition.setX(m_integerPosition.x()+ 1);
    }
    else if((tmp < 25)&&(tmp > -25))
    {
        tmp = 0;
    }
    else if(tmp < 0)
    {
        if(pace < 0) pace = -pace;
        tmp = MAX_FLOAT_POS - pace;
        m_integerPosition.setX(m_integerPosition.x()- 1);
    }

    m_floatPosition.setX(tmp);
}

void DoublePosition::increaseY(int pace)
{
    int tmp = m_floatPosition.y() + pace;
    if(tmp >= MAX_FLOAT_POS)
    {
        tmp = 0;
        m_integerPosition.setY(m_integerPosition.y()+ 1);
    }
    else if((tmp < 25)&&(tmp > -25))
    {
        tmp = 0;
    }
    else if(tmp < 0)
    {
        if(pace < 0) pace = -pace;
        tmp = MAX_FLOAT_POS - pace;
        m_integerPosition.setY(m_integerPosition.y()- 1);
    }

    m_floatPosition.setY(tmp);
}
void DoublePosition::setIntegerPosition(const QPoint &integerPosition)
{
    m_integerPosition = integerPosition;
}

void DoublePosition::setIntegerPositionX(int x)
{
    m_integerPosition.setX(x);
}

void DoublePosition::setIntegerPositionY(int y)
{
    m_integerPosition.setY(y);
}
void DoublePosition::setFloatPosition(const QPoint &floatPosition)
{
    m_floatPosition = floatPosition;
}

void DoublePosition::setFloatPositionX(int x)
{
    m_floatPosition.setX(x);
}

void DoublePosition::setFloatPositionY(int y)
{
    m_floatPosition.setY(y);
}

qreal DoublePosition::euclidianDistanceTo(const DoublePosition &position)
{
    qreal res = 0.0, tmpDouble;
    tmpDouble = (qreal)(m_floatPosition.x() - position.floatPosition().x()) / MAX_FLOAT_POS;
    res += qPow(tmpDouble + (qreal)(m_integerPosition.x() - position.integerPosition().x()), 2);

    tmpDouble = (qreal)(m_floatPosition.y() - position.floatPosition().y()) / MAX_FLOAT_POS;
    res += qPow(tmpDouble + (qreal)(m_integerPosition.y() - position.integerPosition().y()), 2);

    res = qSqrt(res);
    return res;
}

bool operator==(const DoublePosition &a, const DoublePosition &b)
{
    return ((qAbs(a.floatPosition().x() - b.floatPosition().x()) < 50)
            &&(qAbs(a.floatPosition().y() - b.floatPosition().y()) < 50)
            &&(a.integerPosition().x() == b.integerPosition().x())
            &&(a.integerPosition().y() == b.integerPosition().y()));
}

bool operator!=(const DoublePosition &a, const DoublePosition &b)
{
    return !(a==b);
}



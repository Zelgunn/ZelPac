#include "firework.h"

Firework::Firework(int width, int height)
    : m_width(width), m_height(height)
{
    init();
}

void Firework::init()
{
    m_center = QPoint(qrand()%m_width, qrand()%m_height);
    m_linesAngles.clear();
    m_linesColor.clear();
    m_lenght = 1;
    m_maxLenght = (qrand()%90 + 10) * 500;
    m_pixsize = qSqrt(m_maxLenght) * 4;
    m_localCenter = QPoint(m_pixsize/2, m_pixsize/2);
    m_fading = 0.5;

    int tmp;

    while(m_linesAngles.size() < 25)
    {
        m_linesAngles.append(qrand()%360);

        tmp = qrand()%3;
        m_linesColor.append(QColor((tmp == 0)*255,(tmp == 1)*255,(tmp == 2)*255));
    }
}

QPixmap Firework::paintFirework()
{
    QPixmap res(m_pixsize, m_pixsize);
    res.fill(QColor(0,0,0,0));
    QPainter painter(&res);

    QPen pen;
    pen.setWidth(1);

    qreal lenght = qSqrt(m_lenght);

    QLineF line;
    for(int i=0; i<m_linesAngles.size(); i++)
    {
        pen.setColor(m_linesColor.at(i));
        painter.setPen(pen);

        line.setP1(m_localCenter);
        line.setAngle(m_linesAngles.at(i));
        line.setLength(lenght);

        painter.drawLine(line);
    }

    QRadialGradient grad(m_localCenter, lenght);
    grad.setColorAt(m_fading, QColor(0,0,0,255));
    grad.setColorAt(1, QColor(0,0,0,510*(m_fading-0.5)));

    painter.setBrush(grad);
    painter.setPen(QColor(0,0,0,0));

    painter.drawEllipse(QPointF(m_localCenter), lenght+1, lenght+1);

    m_lenght += qMax((m_maxLenght - m_lenght)*(m_maxLenght - m_lenght)/500000, 100.0);
    if(m_lenght > 0.5 * m_maxLenght)
    {
        m_fading += 0.005;
        if(m_fading >= 1.0)
            init();
    }

    return res;
}

QPoint Firework::center() const
{
    return m_center;
}



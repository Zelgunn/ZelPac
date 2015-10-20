#include "ghosttimer.h"

GhostTimer::GhostTimer()
{
    m_currentStep = 0;
    m_currentTimer = 0.0;
}

GhostTimer::GhostTimer(QDomElement elem)
{
    m_currentStep = 0;
    m_currentTimer = 0.0;

    QDomNode node = elem.firstChild();
    while(!node.isNull())
    {
        elem = node.toElement();
        if(elem.tagName() == "Step")
        {
            m_stepsTime.append(elem.attribute("time").toDouble());
            m_ghostMode.append(elem.attribute("mode").toInt());
        }
        node = node.nextSibling();
    }
}

int GhostTimer::currentMode() const
{
    return m_ghostMode.at(m_currentStep);
}

void GhostTimer::increaseTimer()
{
    if(m_currentStep + 1 >= m_ghostMode.size())
        return;
    m_currentTimer += 1.0/60.0;
    if(m_currentTimer > m_stepsTime.at(m_currentStep))
    {
        m_currentStep ++;
        m_currentTimer = 0.0;
    }
}

void GhostTimer::addStep(double duration, int mode)
{
    m_stepsTime.append(duration);
    m_ghostMode.append(mode);
}

QList<double> GhostTimer::stepsTime() const
{
    return m_stepsTime;
}

QList<int> GhostTimer::ghostMode() const
{
    return m_ghostMode;
}


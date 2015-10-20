#include "arduinohandler.h"

ArduinoHandler::ArduinoHandler()
{
    m_serialPort = Q_NULLPTR;
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(mainloop()));
}

void ArduinoHandler::start()
{
    QList <QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    // Recherche du port connecté l'Arduino
    for(int i=0; i<infos.size(); i++)
    {
        QSerialPortInfo info = infos.at(i);
        m_serialPort = new QSerialPort(info);
        if((info.description() == "Arduino Uno")&&(m_serialPort->open(QIODevice::ReadOnly)))
        {
            m_serialPort->setBaudRate(9600);
            mainloop();
            return;
        }
    }
    mainloop();
}

void ArduinoHandler::mainloop()
{
    sf::Joystick::update();
    if(sf::Joystick::isConnected(0))
    {
        if((sf::Joystick::isButtonPressed(0, sf::Joystick::X))
                || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) == 100 ))
            emit downPressed();
        else if((sf::Joystick::isButtonPressed(0, sf::Joystick::Y))
                || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) == 100 ))
            emit rightPressed();
        else if((sf::Joystick::isButtonPressed(0, sf::Joystick::Z))
                || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) == -100 ))
            emit leftPressed();
        else if((sf::Joystick::isButtonPressed(0, sf::Joystick::R))
                || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) == -100 ))
            emit upPressed();
    }

    if(!m_timer->isActive())
        m_timer->start(20);

    if((!m_serialPort)||(!m_serialPort->isOpen()))
        return;

    QByteArray data = m_serialPort->readAll();
    if(data.size() > 0)
    {
        switch (data.at(1)) {
        case '0': emit rightPressed();
            break;
        case '1': emit leftPressed();
            break;
        case '2': emit upPressed();
            break;
        case '3': emit downPressed();
            break;
        }
    }
}

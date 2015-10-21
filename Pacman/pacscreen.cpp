#include "pacscreen.h"

Pacscreen::Pacscreen(QString dataFile, QWidget *parent) :
    QWidget(parent)
{
    setWindowState(Qt::WindowFullScreen);
    setWindowTitle("Pacman v1.0");

    new QShortcut(tr("Right"), this, SLOT(on_Right()));
    new QShortcut(tr("Left"), this, SLOT(on_Left()));
    new QShortcut(tr("Up"), this, SLOT(on_Up()));
    new QShortcut(tr("Down"), this, SLOT(on_Down()));
    new QShortcut(tr("Return"), this, SLOT(on_Enter()));

    m_handler = new ArduinoHandler;
    m_handler->start();

    QObject::connect(m_handler, SIGNAL(rightPressed()), this, SLOT(on_Right()));
    QObject::connect(m_handler, SIGNAL(leftPressed()), this, SLOT(on_Left()));
    QObject::connect(m_handler, SIGNAL(upPressed()), this, SLOT(on_Up()));
    QObject::connect(m_handler, SIGNAL(downPressed()), this, SLOT(on_Down()));

    m_audioPlayer = new QMediaPlayer;

    m_playlist = new QMediaPlaylist;
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->addMedia(QUrl("qrc:/sounds/sounds/Get Jinxed.mp3"));
    m_audioPlayer->setVolume(50);

    m_audioPlayer->setPlaylist(m_playlist);
    //m_audioPlayer->play();

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(16);
    initGame(dataFile);
}

Pacscreen::~Pacscreen()
{

}

void Pacscreen::initGame(const QString &filename)
{
    m_fps=0;
    m_frameCount=0;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);
    m_game = new Game(&dom);
    file.close();

    QObject::connect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
}

void Pacscreen::onKeyboardInput(int direction)
{
    if(m_game == Q_NULLPTR)
    {
        initGame();
    }
    else if (!m_game->timer()->isActive())
    {
        m_game->resumeGame();
    }
    else if(direction != Unit::DirectionNotValid)
    {
        m_game->setPacmanDirection(direction);
    }
}

void Pacscreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,width(),height()), Qt::black);
    if(m_game != Q_NULLPTR)
    {
        QPixmap image = m_game->image();

        int x = width() - image.width(),y = height() - image.height();
        painter.drawPixmap(x/2,y/2, image);

        paintLifes(&painter);
        paintScore(&painter);
        if(m_game->isChangingLevel())
            paintLevelName(&painter);
    }
    paintFPS(&painter);

    painter.end();
}

void Pacscreen::paintLifes(QPainter *painter)
{
    int tmp = 50;
    static QPixmap life = QPixmap(":/textures/images/pacman_life.png").scaled(24,24);
    for(int i=0; i<m_game->lifes(); i++)
    {
        painter->drawPixmap(tmp, 100, life);
        tmp += 30;
    }
}

void Pacscreen::paintScore(QPainter *painter)
{
    QString scoreString = "Score : ";
    scoreString.append(QString::number(m_game->score()));
    painter->setPen(QPen(QColor(255,255,255)));
    painter->setFont(QFont("Arial", 16, QFont::Bold));
    painter->drawText(50,50,scoreString);
}

void Pacscreen::paintFPS(QPainter *painter)
{
    painter->setFont(QFont("Arial", 16, QFont::Bold));
    int time; QString fps = "FPS : ";
    if(m_lastFrameTime.isValid())
    {
        time = m_lastFrameTime.msecsTo(QTime::currentTime());
        m_fps += time;
        m_frameCount ++;
        fps.append(QString::number(1000*m_frameCount/m_fps));
        painter->drawText(width() - 150, 50, fps);
    }
    m_lastFrameTime = QTime::currentTime();
}

void Pacscreen::paintLevelName(QPainter *painter)
{


    QString name = m_game->levels()[m_game->currentLevel()]->name();

    QFont font("Times", 30, QFont::Bold);
    painter->setFont(font);

    int w = width() * 3/5;
    int h = height() / 10;
    QRect rect;
    rect.setX((width() - w)/2);
    rect.setY((height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setBrush(QBrush(QColor(0,0,0,100)));

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, name);
}

void Pacscreen::on_Right()
{
    onKeyboardInput(Unit::Right);
}

void Pacscreen::on_Left()
{
    onKeyboardInput(Unit::Left);
}

void Pacscreen::on_Up()
{
    onKeyboardInput(Unit::Up);
}

void Pacscreen::on_Down()
{
    onKeyboardInput(Unit::Down);
}

void Pacscreen::on_Enter()
{
    onKeyboardInput(Unit::DirectionNotValid);
}

void Pacscreen::on_GameFinished()
{
    m_game->timer()->stop();
    QObject::disconnect(m_game->timer(), SIGNAL(timeout()), this, SLOT(repaint()));
    QObject::disconnect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
    delete m_game;
    m_game = Q_NULLPTR;
}

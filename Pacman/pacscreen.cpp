#include "pacscreen.h"

Pacscreen::Pacscreen(QWidget *parent) :
    QWidget(parent)
{
    setWindowState(Qt::WindowFullScreen);
    setWindowTitle("Pacman v1.0");

    new QShortcut(tr("Right"), this, SLOT(on_Right()));
    new QShortcut(tr("Left"), this, SLOT(on_Left()));
    new QShortcut(tr("Up"), this, SLOT(on_Up()));
    new QShortcut(tr("Down"), this, SLOT(on_Down()));

    m_handler = new ArduinoHandler;
    m_handler->start();

    QObject::connect(m_handler, SIGNAL(rightPressed()), this, SLOT(on_Right()));
    QObject::connect(m_handler, SIGNAL(leftPressed()), this, SLOT(on_Left()));
    QObject::connect(m_handler, SIGNAL(upPressed()), this, SLOT(on_Up()));
    QObject::connect(m_handler, SIGNAL(downPressed()), this, SLOT(on_Down()));

    m_audioPlayer = new QMediaPlayer;

    m_playlist = new QMediaPlaylist;
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->addMedia(QUrl::fromLocalFile("C:\\Users\\Degva_000\\Documents\\C++\\Pacman\\Get Jinxed.mp3"));
    m_audioPlayer->setVolume(50);

    m_audioPlayer->setPlaylist(m_playlist);

    initGame();
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

    QObject::connect(m_game->timer(), SIGNAL(timeout()), this, SLOT(repaint()));
    QObject::connect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
    m_game->startGame();
}

void Pacscreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect screen = QApplication::desktop()->screenGeometry();
    painter.fillRect(screen, Qt::black);
    if(m_game != Q_NULLPTR)
    {
        QPixmap image = m_game->image();

        int x = screen.width() - image.width(),y = screen.height() - image.height();
        painter.drawPixmap(x/2,y/2, image);

        QString scoreString = "Score : ";
        scoreString.append(QString::number(m_game->score()));
        painter.setPen(QPen(QColor(255,255,255)));
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(50,50,scoreString);

        int tmp = 50;
        QPixmap life = QPixmap(":/textures/images/pacman_life.png").scaled(24,24);
        for(int i=0; i<m_game->lifes(); i++)
        {
            painter.drawPixmap(tmp, 100, life);
            tmp += 30;
        }
    }
    int time; QString fps = "FPS : ";
    if(m_lastFrameTime.isValid())
    {
        time = m_lastFrameTime.msecsTo(QTime::currentTime());
        m_fps += time;
        m_frameCount ++;
        fps.append(QString::number(1000*m_frameCount/m_fps));
        painter.drawText(screen.width() - 150, 50, fps);
    }
    m_lastFrameTime = QTime::currentTime();

    painter.end();
}

void Pacscreen::on_Right()
{
    if(m_game == Q_NULLPTR)
        initGame();
    else
        m_game->setPacmanDirection(Unit::Right);
}

void Pacscreen::on_Left()
{
    if(m_game == Q_NULLPTR)
        initGame();
    else
        m_game->setPacmanDirection(Unit::Left);
}

void Pacscreen::on_Up()
{
    if(m_game == Q_NULLPTR)
        initGame();
    else
        m_game->setPacmanDirection(Unit::Up);
}

void Pacscreen::on_Down()
{
    if(m_game == Q_NULLPTR)
        initGame();
    else
        m_game->setPacmanDirection(Unit::Down);
}

void Pacscreen::on_GameFinished()
{
    m_game->timer()->stop();
    QObject::disconnect(m_game->timer(), SIGNAL(timeout()), this, SLOT(repaint()));
    QObject::disconnect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
    delete m_game;
    m_game = Q_NULLPTR;
}

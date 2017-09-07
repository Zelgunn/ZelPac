#include "pacscreen.h"

Pacscreen::Pacscreen(QString dataFile, QWidget *parent) :
    QWidget(parent)
{
    setWindowState(Qt::WindowFullScreen);
    setWindowTitle("Pacman v2.0 release");

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
    m_playlist->addMedia(QUrl("qrc:/sounds/sounds/Music (Bensound).mp3"));
    m_audioPlayer->setVolume(50);

    m_audioPlayer->setPlaylist(m_playlist);
    m_audioPlayer->play();

    m_startAnimationDone = false;
    m_game = Q_NULLPTR;

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(16);
    initGame(dataFile);

    for(int i=0; i<10; i++)
        m_fireworks.append(new Firework(width(), height()));
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
    if(m_game != Q_NULLPTR)
    {
        QObject::disconnect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
        delete m_game;
    }
    m_game = new Game(&dom);
    file.close();

    QObject::connect(m_game, SIGNAL(gameFinished()), this, SLOT(on_GameFinished()));
}

void Pacscreen::onKeyboardInput(int direction)
{
    if((m_game == Q_NULLPTR)||(m_game->victory())||(m_game->defeat()))
    {
        if(restingTimeOff())
            initGame();
    }
    else if (!m_startAnimationDone)
    {
        m_startAnimationDone = true;
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

bool Pacscreen::restingTimeOff()
{
    if(m_restingTime.isNull()) return true;
    return m_restingTime.msecsTo(QTime::currentTime()) > 1000;
}

void Pacscreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,width(),height()), Qt::black);
    if(m_game != Q_NULLPTR)
    {
        if(m_startAnimationDone)
        {
            if(m_game->isChangingLevel())
            {
                paintFirstFrame(&painter);
                paintLevelName(&painter);
            }
            else
            {
                if(m_game->victory())
                {
                    paintVictoryAnimation(&painter);
                }
                else if(m_game->defeat())
                {
                    paintDefeatAnimation(&painter);
                }
                else
                {
                    paintGame(&painter);
                }
            }
        }
        else
        {
            paintStartAnimation(&painter);
        }
        paintLifes(&painter);
        paintScore(&painter);
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
    painter->setBrush(QColor(255,255,255));
    painter->setPen(QColor(255,255,255));
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

void Pacscreen::paintGame(QPainter *painter)
{
    QPixmap image = m_game->image();
    int x = width() - image.width(),y = height() - image.height();
    painter->drawPixmap(x/2,y/2, image);
}

void Pacscreen::paintFirstFrame(QPainter *painter)
{
    QPixmap *image = m_game->firstFrame();
    int x = width() - image->width(),y = height() - image->height();
    painter->drawPixmap(x/2,y/2, *image);
    delete image;
}

void Pacscreen::paintStartAnimation(QPainter *painter)
{
    static bool init = false;
    static QList<QPoint> hiddenPoints;
    Level* level = m_game->levels()[m_game->currentLevel()];
    Grid* grid = level->grid();

    if(!init)
    {
        for(int i=0; i<grid->size().width(); i++)
        {
            for(int j=0; j<grid->size().height(); j++)
            {
                hiddenPoints.append(QPoint(i,j));
            }
        }

        init = true;
    }

    QPixmap *image = m_game->firstFrame();
    int x = width() - image->width(),y = height() - image->height();
    painter->drawPixmap(x/2,y/2, *image);
    delete image;

    QSize tileSize(grid->tilesSize(), grid->tilesSize());
    painter->setBrush(QColor(0,0,0));
    for(int i=0; i<hiddenPoints.size(); i++)
    {
        QPoint point(x/2,y/2);
        point += hiddenPoints.at(i) * grid->tilesSize();
        painter->drawRect(QRect(point, tileSize));
    }

    if(hiddenPoints.size() > 0)
    {
        hiddenPoints.removeFirst();
        hiddenPoints.removeLast();
    }
    else
        m_startAnimationDone = true;
}

void Pacscreen::paintVictoryAnimation(QPainter *painter)
{
    static int blink = 0;
    static QColor textColor = QColor(255,255,255).convertTo(QColor::Hsv);;

    Firework *firework;
    for(int i=0; i<m_fireworks.size(); i++)
    {
        firework = m_fireworks.at(i);
        painter->drawPixmap(firework->center(), firework->paintFirework());
    }

    textColor.setHsv((textColor.hue()+1)%360, 100, 255);
    painter->setBrush(textColor);
    painter->setPen(textColor);

    QFont font("Times", 30, QFont::Bold);
    painter->setFont(font);

    int w = width() * 3/5;
    int h = height() / 10 + 100;
    QRect rect;
    rect.setX((width() - w)/2);
    rect.setY((height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setBrush(QBrush(QColor(0,0,0,100)));

    painter->drawText(rect, Qt::AlignCenter, "Victoire !");

    blink = (blink +1)%90;
    if(restingTimeOff() && (blink < 45))
    {
        rect.setY(rect.y() + 100);
        font = QFont("Times", 16, QFont::Bold);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignCenter, "Appuyez sur une touche pour jouer de nouveau");
    }
}

void Pacscreen::paintDefeatAnimation(QPainter *painter)
{
    static int blink = 0;
    int w = width() * 3/5;
    int h = height() / 10 + 100;
    QRect rect;
    rect.setX((width() - w)/2);
    rect.setY((height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setBrush(QBrush(QColor(255,0,0,200)));
    painter->setPen(QPen(QColor(255,0,0,200)));

    QFont font("Times", 30, QFont::Bold);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, "Défaite");

    blink = (blink +1)%90;
    if(restingTimeOff() && (blink < 45))
    {
        rect.setY(rect.y() + 100);
        font = QFont("Times", 16, QFont::Bold);
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignCenter, "Appuyez sur une touche pour jouer de nouveau");
    }
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
    m_restingTime = QTime::currentTime();
}

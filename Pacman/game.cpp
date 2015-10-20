#include "game.h"

Game::Game(QDomDocument *dom)
{
    QDomElement elem = dom->documentElement();
    QDomNode node = elem.firstChild();
    Level *level;

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();

            if(elem.tagName() == "TileSize")
            {
                m_textureHandler.setTilesSize(elem.attribute("value", "24").toInt());
            }

            if(elem.tagName() == "Level")
            {
                level = new Level(elem, &m_textureHandler);

                QObject::connect(level->player(), SIGNAL(pelletEaten()), this, SLOT(onPelletEaten()));
                QObject::connect(level->player(), SIGNAL(energizerEaten()), this, SLOT(onEnergizerEaten()));

                m_levels.append(level);
            }

            if(elem.tagName() == "Texture")
            {
                m_textureHandler.addTexture(elem.attribute("filename"));
            }
        }
        node = node.nextSibling();
    }

    m_currentLevel = 0;
    m_score = 0;
    m_consecutivesEnergizers = 0;
    m_lifes = 3;
    m_shortSoundsPlayer = new QMediaPlayer;

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(nextGameFrame()));
}

Game::~Game()
{
    m_timer->stop();
    QObject::disconnect(m_timer, SIGNAL(timeout()), this, SLOT(nextGameFrame()));
    delete m_timer;

    Level *level;
    while(m_levels.size())
    {
        level = m_levels.last();
        m_levels.removeLast();

        QObject::disconnect(level->player(), SIGNAL(pelletEaten()), this, SLOT(onPelletEaten()));
        QObject::disconnect(level->player(), SIGNAL(energizerEaten()), this, SLOT(onEnergizerEaten()));

        delete level;
    }

    delete m_shortSoundsPlayer;
}

void Game::startGame()
{
    m_timer->start(TIME_BETWEEN_FRAMES);
    Level *level = m_levels.at(m_currentLevel);

    level->startLevel();
}

void Game::nextGameFrame()
{
    static float chaseTimer = 0.0;
    static QTime lastFrameTime;

    if(!lastFrameTime.isNull())
    {
        chaseTimer += lastFrameTime.msecsTo(QTime::currentTime())/1000.0;
    }
    lastFrameTime = QTime::currentTime();

    Level *level = m_levels.at(m_currentLevel);

    // Mouvements
    QList<Ghost*> collision;
    level->movePacman();
    level->moveGhosts();
    collision.append(level->checkUnitsPositions());
    level->updateTimer();
    //
    if(level->eatenPellets()==level->pelletCount())
    {
        m_currentLevel++;
        if(m_currentLevel == m_levels.size())
        {
            m_timer->stop();
            emit gameFinished();
            return;
        }
        else
        {
            Level *level = m_levels.at(m_currentLevel);
            level->startLevel();
        }
    }

    if(collision.size() > 0)
    {
        if(!Ghost::areGhostsScared())
        {
            m_lifes--;
            if(m_lifes>0)
            {
                level->resetUnitsPosition();
                QTimer::singleShot(2000, this, SLOT(resumeGame()));
            }
            else
            {
                emit gameFinished();
                return;
            }
            m_timer->stop();
        }
        else
        {
            for(int i=0; i<collision.size(); i++)
            {
                m_score += 200 * qPow(2, m_consecutivesEnergizers);
                m_consecutivesEnergizers ++;
                QTimer::singleShot(500, this, SLOT(resumeGame()));
                m_timer->stop();
            }
        }
    }

    if(!Ghost::areGhostsScared())
        m_consecutivesEnergizers = 0;

    QPixmap *image = level->render();
    m_image = *image;
    delete image;
}

void Game::resumeGame()
{
    m_timer->start(TIME_BETWEEN_FRAMES);
}

void Game::onPelletEaten()
{
    m_score += 10;
    Level *level = m_levels.at(m_currentLevel);
    int eatenPellets = level->eatenPellets() +1;
    level->setEatenPellets(eatenPellets);
    switch (eatenPellets) {
    case 30: inky->setFree(true);
        break;
    case 80: clyde->setFree(true);
        break;
    }
}

void Game::onEnergizerEaten()
{
    m_score += 50;
}

QPixmap Game::image() const
{
    return m_image;
}

void Game::setPacmanDirection(int direction)
{
    Level *level = m_levels.at(m_currentLevel);
    level->setPacmanDirection(direction);
}

QTimer* Game::timer() const
{
    return m_timer;
}

int Game::lifes() const
{
    return m_lifes;
}

int Game::score() const
{
    return m_score;
}

QList<Level *> Game::levels() const
{
    return m_levels;
}

int Game::currentLevel() const
{
    return m_currentLevel;
}

void Game::setCurrentLevel(int currentLevel)
{
    m_currentLevel = currentLevel;
}

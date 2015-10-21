#include "level.h"

Level::Level(QDomElement elem, TextureHandler *textureHandler)
{
    m_textureHandler = textureHandler;
    m_name = elem.attribute("name");

    QDomNode node = elem.firstChild();
    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();

            if(elem.tagName() == "Grid")
            {
                m_grid = new Grid(elem, m_textureHandler);
            }

            if(elem.tagName() == "Player")
            {
                m_player = new PacmanUnit(m_grid, elem);
                QObject::connect(m_player, SIGNAL(energizerEaten()), this, SLOT(onEnergizerEaten()));
            }

            if(elem.tagName() == "Blinky")
            {
                m_ghosts[Ghost::Blinky] = Ghost(m_grid, elem, Ghost::Blinky);
                m_ghosts[Ghost::Blinky].setFree(true);
            }

            if(elem.tagName() == "Pinky")
            {
                m_ghosts[Ghost::Pinky] = Ghost(m_grid, elem, Ghost::Pinky);
                m_ghosts[Ghost::Pinky].setFree(true);
            }

            if(elem.tagName() == "Inky")
            {
                m_ghosts[Ghost::Inky] = Ghost(m_grid, elem, Ghost::Inky);
            }

            if(elem.tagName() == "Clyde")
            {
                m_ghosts[Ghost::Clyde] = Ghost(m_grid, elem, Ghost::Clyde);
            }

            if(elem.tagName() == "GhostTimer")
            {
                m_ghostTimer = GhostTimer(elem);
            }

            if(elem.tagName() == "ScaredMode")
            {
                m_scaredDuration = elem.attribute("duration").toInt();
                m_flashsCount = elem.attribute("flashs").toInt();
            }

            if(elem.tagName() == "SpecTextures")
            {
                m_grid->setEmptyTile(elem.attribute("empty").at(0).toLatin1() - 'a');
                m_grid->setPelletTile(elem.attribute("pellet").at(0).toLatin1() - 'a');
                m_grid->setEnergizerTile(elem.attribute("energizer").at(0).toLatin1() - 'a');
            }
        }
        // Itération
        node = node.nextSibling();
    }
    m_eatenPellets = 0;
    m_pelletCount = 0;
    m_flashOn = false;
    m_flasher = new QTimer;
    QObject::connect(m_flasher, SIGNAL(timeout()), this, SLOT(reverseFlash()));
    int tilesCount = m_grid->size().width() * m_grid->size().height();
    for(int i=0; i<tilesCount; i++)
        if(m_grid->tileTexture(i) == m_grid->pelletTile())
            m_pelletCount ++;
}

Level::~Level()
{
    QObject::disconnect(m_player, SIGNAL(energizerEaten()), this, SLOT(onEnergizerEaten()));

    delete m_player;
    delete m_grid;

    QObject::disconnect(m_flasher, SIGNAL(timeout()), this, SLOT(reverseFlash()));
    delete m_flasher;
}

PacmanUnit *Level::player() const
{
    return m_player;
}

Ghost Level::ghost(int ghostIdx) const
{
    return m_ghosts[ghostIdx];
}

void Level::setGhostsChasingMode(int ghostsChasingMode)
{
    for(int i=0; i<4; i++)
    {
        m_ghosts[i].setState(ghostsChasingMode);
    }
}

void Level::setGhostsScared(bool scared)
{
    if(scared != Ghost::areGhostsScared())
    {
        Ghost::setGhostsScared(scared);
        for(int i=0; i<4; i++)
        {
            m_ghosts[i].reverseDirection();
        }
    }
    for(int i=0; i<4; i++)
        m_ghosts[i].reverseSpeeds(scared);
    pacman->reverseSpeeds(scared);
}

int Level::pelletCount() const
{
    return m_pelletCount;
}

int Level::eatenPellets() const
{
    return m_eatenPellets;
}

void Level::setEatenPellets(int eatenPellets)
{
    m_eatenPellets = eatenPellets;
}

Grid *Level::grid() const
{
    return m_grid;
}

void Level::setGrid(Grid *grid)
{
    m_grid = grid;
}

int Level::flashsCount() const
{
    return m_flashsCount;
}

int Level::scaredDuration() const
{
    return m_scaredDuration;
}


void Level::setPacmanDirection(int direction)
{
    m_player->setDirection(direction);
}

QList<Ghost*> Level::checkUnitsPositions()
{
    QList<Ghost*> res;
    Ghost *ghost;
    for(int i=0; i<4; i++)
    {
        ghost = &(m_ghosts[i]);
        if(ghost->fullPosition().euclidianDistanceTo(pacman->fullPosition()) < 0.5)
        {
            if(Ghost::areGhostsScared())
            {
                if(ghost->state() != Ghost::Caught)
                {
                    ghost->setCaught();
                    res.append(ghost);
                }
            }
            else
                res.append(ghost);
        }
    }
    return res;
}

QPixmap *Level::render()
{
    QPixmap *res = new QPixmap(m_grid->updateBaseImage());

    addUnitsToImage(res);

    return res;
}

void Level::movePacman()
{
    m_player->move();
}

void Level::moveGhosts()
{
    for(int i=0; i<4; i++)
        m_ghosts[i].move();
}

void Level::updateTimer()
{
    if(!Ghost::areGhostsScared())
    {
        m_ghostTimer.increaseTimer();
        setGhostsChasingMode(m_ghostTimer.currentMode());
    }
}

void Level::resetUnitsPosition()
{
    for(int i=0; i<4; i++)
        m_ghosts[i].resetPosition();
    m_player->resetPosition();
}

void Level::startLevel()
{
    blinky = &(m_ghosts[Ghost::Blinky]);
    pinky = &(m_ghosts[Ghost::Pinky]);
    inky = &(m_ghosts[Ghost::Inky]);
    clyde = &(m_ghosts[Ghost::Clyde]);
    pacman = m_player;
    setGhostsChasingMode(Ghost::Scatter);
}

void Level::onEnergizerEaten()
{
    setGhostsScared(true);
    QTimer::singleShot(m_scaredDuration, this, SLOT(onEnergizerEnding()));
    QTimer::singleShot(m_scaredDuration + m_flashsCount*500, this, SLOT(leaveScaredMove()));
}

void Level::onEnergizerEnding()
{
    m_flashOn = true;
    m_flasher->start(500);
}

void Level::reverseFlash()
{
    m_flashOn = !m_flashOn;
}

void Level::leaveScaredMove()
{
    setGhostsScared(false);
    m_flashOn = false;
    m_flasher->stop();
    for(int i=0; i<4; i++)
        m_ghosts[i].setSpeed(m_ghosts[i].baseSpeed());
}

void Level::addUnitsToImage(QPixmap *res)
{
    addPacmanToImage(res);
    addGhostsToImage(res);
}

void Level::addGhostsToImage(QPixmap *res)
{
    Ghost *ghost;
    QPixmap image;
    int x,y;
    QPainter painter(res);
    int tilesSize = m_grid->tilesSize();

    for(int g=0; g<4; g++)
    {
        ghost = &(m_ghosts[g]);
        if(Ghost::areGhostsScared())
        {
            if(ghost->state() == Ghost::Caught)
                image = ghost->ghostEyesImage(tilesSize);
            else
                image = Ghost::scaredGhostImage(m_flashOn);
        }
        else
            image = ghost->ghostImage(tilesSize);
        x = ghost->position().x()*tilesSize+ ghost->positionInTile().x()/FLOAT_PACE;
        y = ghost->position().y()*tilesSize + ghost->positionInTile().y()/FLOAT_PACE;

        painter.drawPixmap(x, y, image);
    }

    painter.end();
}

void Level::addPacmanToImage(QPixmap *res)
{
    QPixmap pacmanImage = m_player->renderPacmanImage(m_grid->tilesSize());
    m_player->nextFrame();

    int tileSize = m_grid->tilesSize();
    int x = m_player->position().x() * tileSize + m_player->positionInTile().x()/FLOAT_PACE;
    int y = m_player->position().y() * tileSize + m_player->positionInTile().y()/FLOAT_PACE;

    QPainter painter(res);

    painter.drawPixmap(x, y, pacmanImage);

    painter.end();
}
QString Level::name() const
{
    return m_name;
}


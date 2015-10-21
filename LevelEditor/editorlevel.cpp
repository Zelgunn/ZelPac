#include "editorlevel.h"

EditorLevel::EditorLevel() :
    m_isValid(false)
{
    m_gridTextures = Q_NULLPTR;
    m_collisionsGrid = Q_NULLPTR;
    m_scaredTime = 0;
}

EditorLevel::EditorLevel(const QString &name, QSize gridSize, int tileSize, TextureHandler *textureHandler) :
    m_name(name), m_gridSize(gridSize), m_tileSize(tileSize), m_textureHandler(textureHandler),  m_emptyTileTexture(-1), m_pelletTileTexture(-1), m_energizerTileTexture(-1), m_isValid(true)
{
    if(m_textureHandler == Q_NULLPTR)
        m_isValid = false;
    int elemCount = gridSize.width() * gridSize.height();

    m_scaredTime = 0;
    m_gridTextures = (int*) malloc(sizeof(int) * elemCount);
    m_collisionsGrid = (int*) malloc(sizeof(int) * elemCount);
    for(int i=0; i<elemCount; i++)
    {
        m_gridTextures[i] = -1;
        m_collisionsGrid[i] = -1;
    }

    m_unitsData = (UnitData*) malloc(sizeof(UnitData) * 5);
    for(int i=0; i<5; i++)
    {
        m_unitsData[i] = initUnitData();
    }
    m_emptyTileTexture = 0;
    m_pelletTileTexture = 0;
    m_energizerTileTexture = 0;
}

EditorLevel::EditorLevel(QDomElement elem, TextureHandler *textureHandler) :
    m_textureHandler(textureHandler), m_isValid(true)
{
    m_name = elem.attribute("name");
    m_scaredTime = 0;
    m_unitsData = (UnitData*) malloc(sizeof(UnitData) * 5);

    QStringList unitNames;
    unitNames << "Player" << "Blinky" << "Pinky" << "Inky" << "Clyde";

    QDomNode node = elem.firstChild();
    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            if(elem.tagName() == "Grid")
            {
                loadGrid(elem);
            }

            if(unitNames.contains(elem.tagName()))
            {
                int idx = unitNames.indexOf(elem.tagName());
                m_unitsData[idx].halfx = (elem.attribute("fx") == "half");
                m_unitsData[idx].halfy = (elem.attribute("fy") == "half");
                m_unitsData[idx].position.setX(elem.attribute("ix").toInt());
                m_unitsData[idx].position.setY(elem.attribute("iy").toInt());
                m_unitsData[idx].speed = elem.attribute("speed").toInt();
                m_unitsData[idx].sspeed = elem.attribute("sspeed").toInt();
            }

            if(elem.tagName() == "SpecTextures")
            {
                m_emptyTileTexture = elem.attribute("empty").toLatin1().at(0) - 'a';
                m_pelletTileTexture = elem.attribute("pellet").toLatin1().at(0) - 'a';
                m_energizerTileTexture = elem.attribute("energizer").toLatin1().at(0) - 'a';
            }

            if(elem.tagName() == "GhostTimer")
            {
                loadGhostTimer(elem);
            }

            if(elem.tagName() == "ScaredMode")
            {
                m_scaredTime = elem.attribute("duration").toDouble();
            }
        }

        node = node.nextSibling();
    }
}

EditorLevel::EditorLevel(const QString &name, const EditorLevel *copy)
{
    m_name = name;
    m_gridSize = copy->gridSize();
    m_tileSize = copy->tileSize();
    m_textureHandler = copy->textureHandler();
    m_usedTextures = copy->usedTextures();
    m_baseImage = copy->baseImage();
    m_ghostHouse = copy->ghostHouse();
    m_emptyTileTexture = copy->emptyTileTexture();
    m_pelletTileTexture = copy->pelletTileTexture();
    m_energizerTileTexture = copy->energizerTileTexture();
    m_ghostHouse = copy->ghostHouse();
    m_isValid = true;

    // Elements dynamiques.
    int elemCount = m_gridSize.width() * m_gridSize.height();

    m_gridTextures = (int*) malloc(sizeof(int) * elemCount);
    m_collisionsGrid = (int*) malloc(sizeof(int) * elemCount);
    for(int i=0; i<elemCount; i++)
    {
        m_gridTextures[i] = copy->textureAt(i);
        m_collisionsGrid[i] = copy->collisionAt(i);
    }

    m_unitsData = (UnitData*) malloc(sizeof(UnitData) * 5);
    for(int i=0; i<5; i++)
    {
        m_unitsData[i] = copy->unitsData()[i];
    }
}

QDomElement EditorLevel::toXML(QDomDocument *dom) const
{
    QDomElement levelRoot = dom->createElement("Level");
    QDomElement elem, elem2;
    QString s;
    levelRoot.setAttribute("name", m_name);

    // Grille
    elem = dom->createElement("Grid");
        // TileCount
    elem2 = dom->createElement("TileCount");
    elem2.setAttribute("height", m_gridSize.height());
    elem2.setAttribute("width", m_gridSize.width());
    elem.appendChild(elem2);
        // TileSize
    elem2 = dom->createElement("TileSize");
    elem2.setAttribute("value", m_tileSize);
    elem.appendChild(elem2);
        // GridValues
    elem2 = dom->createElement("GridValues");
    for(int i=0; i < m_gridSize.width() * m_gridSize.height(); i++)
        s.append((char)(m_gridTextures[i]+'a'));
    elem2.setAttribute("values", s);
    elem.appendChild(elem2);
        // CollisionsGrid
    elem2 = dom->createElement("CollisionsGrid");
    s.clear();
    for(int i=0; i<m_gridSize.width() * m_gridSize.height(); i++)
        s.append(QString::number(m_collisionsGrid[i]));
    elem2.setAttribute("values", s);
    elem.appendChild(elem2);
        // GhostHouse
    elem2 = dom->createElement("GhostHouse");
    // UNAVAILABLE
//    elem2.setAttribute("x", m_ghostHouse.x());
//    elem2.setAttribute("y", m_ghostHouse.y());
//    elem2.setAttribute("width", m_ghostHouse.width());
//    elem2.setAttribute("height", m_ghostHouse.height());

    //<GhostHouse x="10" y="12" width="6" height="5"/>
    elem2.setAttribute("x", 10);
    elem2.setAttribute("y", 12);
    elem2.setAttribute("width", 6);
    elem2.setAttribute("height", 5);
    elem.appendChild(elem2);

      // UsedTextures
    for(int i=0; i<m_usedTextures.size(); i++)
    {
        elem2 = dom->createElement("UsedTexture");
        elem2.setAttribute("shortname", m_textureHandler->textureNameAt(m_usedTextures.at(i)));
        elem2.setAttribute("as", m_usedTextures.at(i));
        elem.appendChild(elem2);
    }
    levelRoot.appendChild(elem);

    QStringList unitsTags;
    unitsTags << "Player" << "Blinky" << "Pinky" << "Inky" << "Clyde";
    UnitData unitData;
    for(int i=0; i<5; i++)
    {
        unitData = m_unitsData[i];
        elem = dom->createElement(unitsTags.at(i));
        elem.setAttribute("ix", unitData.position.x());
        elem.setAttribute("iy", unitData.position.y());
        elem.setAttribute("speed", unitData.speed);
        elem.setAttribute("sspeed", unitData.sspeed);
        if(unitData.halfx) elem.setAttribute("fx", "half");
        if(unitData.halfy) elem.setAttribute("fy", "half");
        levelRoot.appendChild(elem);
    }

    // Timer
    elem = dom->createElement("GhostTimer");
    QList<double> durations = m_ghostTimer.stepsTime();
    QList<int> modes = m_ghostTimer.ghostMode();
    for(int i=0; i<modes.size(); i++)
    {
        elem2 = dom->createElement("Step");
        elem2.setAttribute("time", durations.at(i));
        elem2.setAttribute("mode", modes.at(i));
        elem.appendChild(elem2);
    }
    levelRoot.appendChild(elem);

    // Temps effrayé
    elem = dom->createElement("ScaredMode");
    elem.setAttribute("duration", m_scaredTime);
    elem.setAttribute("flashs", 5);
    levelRoot.appendChild(elem);

    // Textures spéciales
    elem = dom->createElement("SpecTextures");
    elem.setAttribute("empty", QString(m_emptyTileTexture + 'a'));
    elem.setAttribute("pellet", QString(m_pelletTileTexture + 'a'));
    elem.setAttribute("energizer", QString(m_energizerTileTexture + 'a'));
    levelRoot.appendChild(elem);

    return levelRoot;
}

QString EditorLevel::name() const
{
    return m_name;
}

void EditorLevel::setName(const QString &name)
{
    m_name = name;
}

bool EditorLevel::isValid() const
{
    return m_isValid;
}
QSize EditorLevel::gridSize() const
{
    return m_gridSize;
}
int EditorLevel::tileSize() const
{
    return m_tileSize;
}

TextureHandler *EditorLevel::textureHandler() const
{
    return m_textureHandler;
}

QList<int> EditorLevel::usedTextures() const
{
    return m_usedTextures;
}

QImage EditorLevel::baseImage() const
{
    return m_baseImage;
}

QRect EditorLevel::ghostHouse() const
{
    return m_ghostHouse;
}

int EditorLevel::textureAt(int i) const
{
    return m_gridTextures[i];
}

int EditorLevel::textureAt(const QPoint &tile) const
{
    return m_gridTextures[tile.x() + tile.y() * m_gridSize.width()];
}

void EditorLevel::setTileTexture(const QPoint &tile, int texture)
{
    if(!m_usedTextures.contains(texture))
        m_usedTextures.append(texture);
    m_gridTextures[tile.x() + tile.y() * m_gridSize.width()] = texture;
}

int EditorLevel::collisionAt(int i) const
{
    return m_collisionsGrid[i];
}

int EditorLevel::collisionAt(const QPoint &tile) const
{
    return m_collisionsGrid[tile.x() + tile.y() * m_gridSize.width()];
}

void EditorLevel::setCollisionAt(const QPoint &tile, int collision)
{
    m_collisionsGrid[tile.x() + tile.y() * m_gridSize.width()] = collision;
}

QImage EditorLevel::renderBaseImage()
{
    int h = m_gridSize.height() * m_tileSize, w = m_gridSize.width() * m_tileSize;
    QImage res(w,h, QImage::Format_ARGB32);
    QPainter painter(&res);
    QImage texture;

    int x, y;
    for(int i=0; i<m_gridSize.width(); i++)
    {
        x = i*m_tileSize;
        for(int j=0; j<m_gridSize.height(); j++)
        {
            y = j*m_tileSize;
            texture = m_textureHandler->textureAt(m_gridTextures[i + j*m_gridSize.width()]).scaled(m_tileSize, m_tileSize);
            painter.drawImage(x, y, texture);
        }
    }
    painter.end();

    m_baseImage = res;
    return res;
}

QImage EditorLevel::updateBaseImage(QPoint changedPoint)
{
    QPainter painter(&m_baseImage);

    int textureId = m_gridTextures[changedPoint.x() + changedPoint.y() * m_gridSize.width()];
    QImage texture = m_textureHandler->textureAt(textureId).scaled(m_tileSize, m_tileSize);;
    painter.drawImage(changedPoint.x() * m_tileSize, changedPoint.y() * m_tileSize, texture);
    painter.end();

    return m_baseImage;
}

QPoint EditorLevel::screenToTile(QPoint screenPosition)
{
    QPoint res(screenPosition.x() / m_tileSize, screenPosition.y() / m_tileSize);
    return res;
}

void EditorLevel::loadGrid(QDomElement elem)
{
    QDomNode node = elem.firstChild();
    QString s;

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            if(elem.tagName() == "TileCount")
            {
                m_gridSize.setWidth(elem.attribute("width", "10").toInt());
                m_gridSize.setHeight(elem.attribute("height", "10").toInt());
            }

            if(elem.tagName() == "TileSize")
            {
                m_tileSize = elem.attribute("value", "24").toInt();
            }

            if(elem.tagName() == "GridValues")
            {
                s = elem.attribute("values", "");
                m_gridTextures = (int*)malloc(sizeof(int)*s.size());

                for(int i=0; i<s.size(); i++)
                    m_gridTextures[i] = (int)(s.at(i).toLatin1() - 'a');

                for(int i=0; i<s.size(); i++)
                {
                    if(!m_usedTextures.contains(m_gridTextures[i]))
                        m_usedTextures.append(m_gridTextures[i]);
                }
            }

            if(elem.tagName() == "CollisionsGrid")
            {
                s = elem.attribute("values", "");
                m_collisionsGrid = (int*)malloc(sizeof(int)*s.size());
                for(int i=0; i<s.size(); i++)
                    m_collisionsGrid[i] = (int)(s.at(i).toLatin1() - '0');
            }

            if(elem.tagName() == "GhostHouse")
            {
//                m_ghostHouse.setX(elem.attribute("x").toInt());
//                m_ghostHouse.setY(elem.attribute("y").toInt());
//                m_ghostHouse.setWidth(elem.attribute("width").toInt());
//                m_ghostHouse.setHeight(elem.attribute("height").toInt());
                m_ghostHouse = QRect(10, 12, 6, 5);
            }
        }

        node = node.nextSibling();
    }
}

void EditorLevel::loadGhostTimer(QDomElement elem)
{
    QDomNode node = elem.firstChild();

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            m_ghostTimer.addStep(elem.attribute("time").toDouble(), elem.attribute("mode").toInt());
        }
        node = node.nextSibling();
    }
}

void EditorLevel::setScaredTime(double scaredTime)
{
    m_scaredTime = scaredTime;
}

double EditorLevel::scaredTime() const
{
    return m_scaredTime;
}
GhostTimer EditorLevel::ghostTimer() const
{
    return m_ghostTimer;
}

void EditorLevel::setGhostTimer(const GhostTimer &ghostTimer)
{
    m_ghostTimer = ghostTimer;
}

void EditorLevel::appendGhostTimerStep(double time, int mode)
{
    m_ghostTimer.addStep(time, mode);
}

int EditorLevel::energizerTileTexture() const
{
    return m_energizerTileTexture;
}

int EditorLevel::pelletTileTexture() const
{
    return m_pelletTileTexture;
}

int EditorLevel::emptyTileTexture() const
{
    return m_emptyTileTexture;
}

void EditorLevel::setEnergizerTileTexture(int energizerTileTexture)
{
    m_energizerTileTexture = energizerTileTexture;
}

void EditorLevel::setPelletTileTexture(int pelletTileTexture)
{
    m_pelletTileTexture = pelletTileTexture;
}

void EditorLevel::setEmptyTileTexture(int emptyTileTexture)
{
    m_emptyTileTexture = emptyTileTexture;
}


UnitData *EditorLevel::unitsData() const
{
    return m_unitsData;
}

void EditorLevel::setUnitsDataAt(UnitData unitsData, int idx)
{
    m_unitsData[idx] = unitsData;
}

UnitData EditorLevel::initUnitData()
{
    UnitData res;
    res.halfx = false;
    res.halfy = false;
    res.position = QPoint(0,0);
    res.speed = 0;
    res.sspeed = 0;

    return res;
}

#include "editorgame.h"

EditorGame::EditorGame()
{

}

EditorGame::EditorGame(QDomDocument dom)
{
    QDomElement elem = dom.documentElement();
    QDomNode node = elem.firstChild();

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            if(elem.tagName() == "Level")
            {
                EditorLevel *level;
                level = new EditorLevel(elem, textureHandler());
                m_levels.append(level);
            }

            if(elem.tagName() == "Texture")
            {
                m_textureHandler.addTexture(elem.attribute("filename"));
            }
        }
        node = node.nextSibling();
    }
}

EditorGame::~EditorGame()
{
    EditorLevel *level;
    while(m_levels.size() > 0)
    {
        level = m_levels.first();
        m_levels.pop_front();
        delete level;
    }
}

void EditorGame::addLevel(EditorLevel *level)
{
    m_levels.append(level);
}

int EditorGame::levelsCount() const
{
    return m_levels.size();
}

int EditorGame::textureCount() const
{
    return m_textureHandler.count();
}

TextureHandler *EditorGame::textureHandler()
{
    return &m_textureHandler;
}

void EditorGame::save(QString filename) const
{
    QDomDocument dom;
    QDomElement elem;
    QDomProcessingInstruction qdpi = dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
    dom.appendChild(qdpi);
    QDomElement root = dom.createElement("root");
    dom.appendChild(root);

    // Fix pacman
    elem = dom.createElement("TileSize");
    elem.setAttribute("value", 24);
    root.appendChild(elem);

    // Textures
    for(int i=0; i<textureCount(); i++)
    {
        elem = dom.createElement("Texture");
        elem.setAttribute("filename", m_textureHandler.filenameAt(i));
        root.appendChild(elem);
    }

    // Niveaux
    for(int i=0; i<m_levels.size(); i++)
    {
        elem = m_levels.at(i)->toXML(&dom);
        root.appendChild(elem);
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }

    QTextStream out(&file);
    out << dom.toString();

    file.close();
}

EditorLevel *EditorGame::level(int index) const
{
    if((index < 0)||(index >= m_levels.size()))
        return Q_NULLPTR;
    return m_levels.at(index);
}


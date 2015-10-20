#include "texturehandler.h"

TextureHandler::TextureHandler()
{

}

void TextureHandler::addTexture(QString completeFilename)
{
    QString shortFilename = completeFilename.section('/', -1);
    if(m_filenames.contains(shortFilename))
        return;

    m_texturenames.append(shortFilename);
    m_filenames.append(completeFilename);
    QPixmap pixmap(completeFilename);
    m_textures.append(pixmap.scaled(m_tilesSize, m_tilesSize));
}

QPixmap TextureHandler::textureAt(int index) const
{
    if((index < 0) || (index >= m_textures.size()))
    {
        QPixmap errorTexture(32,32);
        errorTexture.fill(Qt::red);
        return errorTexture;
    }

    return m_textures.at(index);
}

QString TextureHandler::filenameAt(int index) const
{
    return m_filenames.at(index);
}

QString TextureHandler::textureNameAt(int index) const
{
    return m_texturenames.at(index);
}

bool TextureHandler::contains(QString filename) const
{
    QString tmp = filename.section('/', -1);
    return m_filenames.contains(tmp);
}

int TextureHandler::indexOfTextureName(QString textureName) const
{
    for(int i=0; i<m_texturenames.size(); i++)
        if(m_texturenames.at(i) == textureName) return i;
    return -1;
}

int TextureHandler::count() const
{
    return m_textures.size();
}

int TextureHandler::tilesSize() const
{
    return m_tilesSize;
}

void TextureHandler::setTilesSize(int tilesSize)
{
    m_tilesSize = tilesSize;
}


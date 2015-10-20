#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <QPixmap>

class TextureHandler
{
public:
    TextureHandler();

    void addTexture(QString completeFilename);
    QPixmap textureAt(int index) const;
    QString filenameAt(int index) const;
    QString textureNameAt(int index) const;
    bool contains(QString filename) const;
    int indexOfTextureName(QString textureName) const;
    int count() const;

    int tilesSize() const;
    void setTilesSize(int tilesSize);

private:
    /** @brief Taille des cases. */
    int m_tilesSize;
    QList<QPixmap> m_textures;
    QStringList m_filenames;
    QStringList m_texturenames;
};

#endif // TEXTUREHANDLER_H

#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <QImage>

/**
 * @brief The NewGhostModeDialog class
 * @author Valentin D.d.G.
 * @version 1.0
 */
class TextureHandler
{
public:
    TextureHandler();

    void addTexture(QString completeFilename);
    QImage textureAt(int index) const;
    QString filenameAt(int index) const;
    QString textureNameAt(int index) const;
    bool contains(QString filename) const;
    int indexOfTextureName(QString textureName) const;
    int count() const;

private:
    QList<QImage> m_textures;
    QStringList m_filenames;
    QStringList m_texturenames;
};

#endif // TEXTUREHANDLER_H

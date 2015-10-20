#ifndef EDITORGAME_H
#define EDITORGAME_H

#include <QDomDocument>
#include <QFile>

#include "texturehandler.h"
#include "editorlevel.h"

class EditorGame
{
public:
    EditorGame();
    EditorGame(QDomDocument dom);
    ~EditorGame();

    EditorLevel *level(int index) const;
    void addLevel(EditorLevel* level);
    int levelsCount() const;
    int textureCount() const;

    TextureHandler *textureHandler();

    void save(QString filename) const;

private:
    TextureHandler m_textureHandler;
    QList<EditorLevel *> m_levels;
};

#endif // EDITORGAME_H

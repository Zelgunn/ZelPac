#ifndef EDITORGAME_H
#define EDITORGAME_H

#include <QDomDocument>
#include <QFile>

#include "texturehandler.h"
#include "editorlevel.h"

/**
 * @brief La classe EditorGame gère l'ensemble des paramètres pouvant être passés au programme Pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La classe gère un ensemble de niveaux dont les paramètres sont via une UI.
 * EditorGame peut s'instancier avec un document XML et s'exporter de la même façon afin de sauvegarder les données.
 */
class EditorGame
{
public:
    /**
     * @brief Constructeur de base de EditorGame.
     * @details Crée une instance de EditorGame vide qui ne dispose d'aucun niveau ni d'aucun paramètres.
     * Lors de cette instanciation, le gestionnaire de texture est créé afin d'être utilisé par l'ensemble des niveaux.
     */
    EditorGame();
    /**
     * @brief Constructeur XML de EditorGame.
     * @param dom Document XML contenant les paramètres sauvegardés.
     * @details Crée une instance de EditorGame qui charge les paramètres se trouvant dans le document XML.
     */
    EditorGame(QDomDocument dom);
    /**
     * @brief Destructeur de EditorGame.
     * @details Désalloue la mémoire de l'objet en plus de celles des niveaux qu'il contient.
     */
    ~EditorGame();

    /**
     * @brief Getter
     * @param index Index du niveau à renvoyer.
     * @return Pointeur sur le niveau à l'index donné.
     */
    EditorLevel *level(int index) const;

    /**
     * @brief Ajoute un niveau à l'objet.
     * @param level Pointeur sur le niveau ajouté.
     */
    void addLevel(EditorLevel* level);

    /**
     * @brief Getter
     * @return Nombre de niveaux contenus dans l'objet.
     */
    int levelsCount() const;

    /**
     * @brief Getter
     * @return Nombre de textures contenues dans le gestionnaire de textures.
     */
    int textureCount() const;

    /**
     * @brief Getter
     * @return Pointeur sur le gestionnaire de texture.
     */
    TextureHandler *textureHandler();

    /**
     * @brief Sauvegarde l'ensemble des paramètres dans le fichier donné.
     * @param filename Chemin (complet) vers le fichier.
     */
    void save(QString filename) const;

private:
    /** @brief Gestionnaire de textures. */
    TextureHandler m_textureHandler;
    /** @brief Niveaux du jeu. */
    QList<EditorLevel *> m_levels;
};

#endif // EDITORGAME_H

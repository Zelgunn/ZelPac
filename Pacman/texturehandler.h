#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <QPixmap>

/**
 * @brief Gestionnaire de textures.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Ce gestionnaire de textures permet de faire le lien
 * entre le nom complet d'un fichier, son nom en court et son index dans le gestionnaire.
 * Le gestionnaire par défaut modifie la taille des textures.
 */

class TextureHandler
{
public:
    /**
     * @brief Constructeur de base de TextureHandler
     * @details Crée un gestionnaire totalement vide.
     */
    TextureHandler();

    /**
     * @brief Ajoute une texture via le chemin du fichier.
     * @param completeFilename Chemin complet vers la texture ajoutée.
     */
    void addTexture(QString completeFilename);

    /**
     * @brief Getter
     * @param index Index de la texture dans le gestionnaire.
     * @return Texture à l'index donné.
     */
    QPixmap textureAt(int index) const;

    /**
     * @brief Getter
     * @param index Index de la texture dans le gestionnaire.
     * @return Chemin complet de la texture à l'index donné.
     */
    QString filenameAt(int index) const;

    /**
     * @brief Getter
     * @param index Index de la texture dans le gestionnaire.
     * @return Nom du fichier de la texture à l'index donné.
     */
    QString textureNameAt(int index) const;

    /**
     * @brief Vérifie si le gestionnaire connait déjà la texture donnée.
     * @param filename Chemin complet vers la texture.
     * @return Vrai si le gestionnaire contient la texture, sinon Faux.
     */
    bool contains(QString filename) const;

    /**
     * @brief Récupère l'index de la texture dans le gestionnaire.
     * @param textureName Nom du fichier de la texture.
     * @return Index de la texture qui porte le nom donné.
     */
    int indexOfTextureName(QString textureName) const;

    /**
     * @brief Getter
     * @return Nombre de textures différentes stockées.
     */
    int count() const;

    /**
     * @brief Getter
     * @return Taille des textures du gestionnaire.
     */
    int tilesSize() const;

    /**
     * @brief Setter
     * @param tilesSize Taille des textures du gestionnaire.
     */
    void setTilesSize(int tilesSize);

private:
    /** @brief Taille des cases. */
    int m_tilesSize;
    /** @brief Tableau des textures. */
    QList<QPixmap> m_textures;
    /** @brief Tableau des chemins complets des textures. */
    QStringList m_filenames;
    /** @brief Tableau des noms des fichiers des textures. */
    QStringList m_texturenames;
};

#endif // TEXTUREHANDLER_H

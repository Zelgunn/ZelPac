#ifndef GRID_H
#define GRID_H

#include <QPixmap>
#include <QDomDocument>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTime>
#include <QtCore/qmath.h>
#include <math.h>

#include "pacmanparameters.h"
#include "texturehandler.h"

/**
 * @brief Classe de grille de Pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La Grille s'occupe d'attribuer à chaque case une texture et un indicateur de collision.
 *
 * La classe génère une image de base qui sera mise à jour le minimum possible (une case par frame en théorie)
 *  afin d'être utilisée par d'autres classes ultérieurement.
 * La grille s'occupe aussi de baliser la maison des fantômes.
 */

class Grid
{
public:
    /**
     * @brief Types de collisions
     */
    enum CollisionsID{CollisionOff, /**< Collision désactivée */
                      CollisionOn   /**< Collision activée */
                     };
    /**
     * @brief Constructeur de base de Grid
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @details L'élément XML doit ressembler au template suivant :
     * - \<Grid>
     *  - \<TileCount height="3" width="3"/>
     *  - \<TileSize height="24" width="24"/>
     *  - \<GridValues values="aaacdebbb"/>
     *  - \<Texture filename="Texture1.png"/>
     *  - \<Texture filename="Texture2.png"/>
     *  - \<Texture filename="Texture_Pastille.png"/>
     *  - \<Texture filename="Texture_Energisant.png"/>
     *  - \<Texture filename="Texture_Vide.png"/>
     *  - \<CollisionsGrid values="000111000"/>
     *  - \<GhostHouse x="1" y="1" width="1" height="1"/>
     * - \</ Grid>
     *
     * TileCount : Nombre de cases (largeur/hauteur)
     * \n TileSize : Taille des cases (largeur/hauteur)
     * \n GridValues : Textures (a : première, ...)
     * \n Texture : Nom de fichier de la texture
     * \n CollisionGrid : Collisions
     * \n GhostHouse : Dimensions de la maison des fantômes
     */
    Grid(QDomElement elem, TextureHandler *textureHandler);

    ~Grid();

    /**
     * @brief Getter
     * @return Taille de la grille (nombre de cases).
     */
    QSize size() const;

    /**
     * @brief Ajoute la texture à la liste des textures de la grille.
     * @param filename Chemin vers la texture
     */
    void addTexture(QString filename);

    /**
     * @brief Donne la texture de la liste des textures de la grille à l'index donné.
     * @param index Index de la texture
     * @return Texture à l'index donné
     */
    QPixmap textureAt(int index);

    /**
     * @brief Donne le numéro de la texture à la case donnée
     * @param p Case à inspecter
     * @return Numéro de la texture
     * @details Le numéro correspond à l'index de textureAt
     */
    int tileTexture(QPoint p);

    /**
     * @brief Donne le numéro de la texture à la case donnée.
     * @param i Index de la case en une dimension.
     * @return Numéro de la texture.
     * @details Le numéro correspond à l'index de textureAt.
     */
    int tileTexture(int i);

    /**
     * @brief Modifie la texture à la case donnée.
     * @param p Case où la modification se fait.
     * @param texture Nouvelle texture de la case.
     */
    void setTileTexture(QPoint p, int texture);

    /**
     * @brief Getter
     * @return Taille d'une case (en pixels).
     */
    int tilesSize() const;

    /**
     * @brief Donne la valeur de collision au point donné.
     * @param pos Case à inspecter.
     * @return Valeur de collision.
     */
    int collisionAt(QPoint pos) const;

    /**
     * @brief Calcule l'image de base de la grille.
     * @return Image de base de la grille.
     */
    QPixmap processBaseImage();

    /**
     * @brief Met à jour l'image de base de la grille.
     * @return Image de base de la grille (mise à jour).
     */
    QPixmap updateBaseImage();

    /**
     * @brief Getter
     * @return Dimension de la maison des fantômes.
     */
    QRect ghostHouse() const;

    /**
     * @brief Getter
     * @return Numero de case vide.
     */
    int emptyTile() const;

    /**
     * @brief Setter
     * @param emptyTile Numero de case vide.
     */
    void setEmptyTile(int emptyTile);

    /**
     * @brief Getter
     * @return Numero de case avec pastille.
     */
    int pelletTile() const;

    /**
     * @brief Setter
     * @param pelletTile
     */
    void setPelletTile(int pelletTile);

    /**
     * @brief Getter
     * @return Numero de case avec énergisant.
     */
    int energizerTile() const;

    /**
     * @brief Setter
     * @param energizerTile
     */
    void setEnergizerTile(int energizerTile);

private:
    // Raw data
    /** @brief Nombre de cases. */
    QSize m_size;
    /** @brief Textures des cases. */
    int *m_gridValues;
    /** @brief Dimension de la maison des fantômes. */
    QRect m_ghostHouse;
    /** @brief Valeur de collisions des cases. */
    int *m_collisionGrid;
    /** @brief Numero de la case vide. */
    int m_emptyTile;
    /** @brief Numero de la case avec pastille. */
    int m_pelletTile;
    /** @brief Numero de la case avec énergisant. */
    int m_energizerTile;

    // Display
    /** @brief Liste des textures. */
    QList<QPixmap> m_textures;
    /** @brief Image de base de la grille. */
    QPixmap m_baseImage;
    /** @brief Gestionnaire de texture. */
    TextureHandler *m_textureHandler;

    // Autres
    /** @brief Cases modifiées depuis la dernière mise à jour de l'image de base. */
    QList<QPoint> m_changedPoints;
};

#endif // GRID_H

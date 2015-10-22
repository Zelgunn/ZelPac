#ifndef EDITORLEVEL_H
#define EDITORLEVEL_H

#include <QString>
#include <QPainter>
#include <QDomElement>
#include "texturehandler.h"
#include "ghosttimer.h"

#include <QDebug>

/**
 * @struct UnitData_str
 * @brief Petite structure regroupant les paramètres principaux d'une unité.
 */
typedef struct UnitData_str
{
    /** @brief Position de l'unité. */
    QPoint position;
    /** @brief Si vrai, la position est décalé d'une demi case vers la droite. */
    bool halfx;
    /** @brief Si vrai, la position est décalé d'une demi case vers le bas. */
    bool halfy;
    /** @brief Vitesse normale de l'unité. */
    int speed;
    /** @brief Vitesse de l'unité lorsque les fantômes sont appeurés. */
    int sspeed;
} UnitData;


/**
 * @brief La classe EditorLevel permet de paramétrer un niveau dans Pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 */

class EditorLevel
{
public:
    /**
     * @brief Identifiants des unités
     */
    enum UnitIDs {Pacman,   /**< Identifiant du Pacman */
                  Blinky,   /**< Identifiant de Shadow (aka Blinky) */
                  Pinky,    /**< Identifiant de Speedy (aka Pinky) */
                  Inky,     /**< Identifiant de Bashful (aka Inky) */
                  Clyde     /**< Identifiant de Pokey (aka Clyde) */
                 };

    /**
     * @brief Constructeur EditorLevel invalide.
     */
    EditorLevel();

    /**
     * @brief Constructeur principale d'EditorLevel.
     * @param name Nom du niveau.
     * @param gridSize Taille du niveau (en nombre de cases).
     * @param tileSize Tailles des cases du niveau (en pixels).
     * @param textureHandler Gestionnaire de texture commun.
     * @details Construit un EditorLevel valide.
     */
    EditorLevel(const QString &name, QSize gridSize, int tileSize, TextureHandler* textureHandler);

    /**
     * @brief Surcharge du constructeur de base pour XML.
     * @param elem Element XML à charger.
     * @param textureHandler Gestionnaire de texture commun.
     */
    EditorLevel(QDomElement elem, TextureHandler *textureHandler);

    /**
     * @brief Constructeur de copie d'objet
     * @param name Nom du niveau.
     * @param copy Niveau à partir duquel se fait la copie.
     * @details Dupplique en mémoire les éléments alloués dynamiquement.
     */
    EditorLevel(const QString &name, const EditorLevel* copy);

    /**
     * @brief Ajoute l'élément XML dans l'arbre donné, les paramètres dépendent de l'objet.
     * @param[out] dom Document XML où l'élément est rajouté.
     */
    QDomElement toXML(QDomDocument *dom) const;

    /**
     * @brief Getter
     * @return Nom du niveau.
     */
    QString name() const;

    /**
     * @brief Setter
     * @param name Nouveau nom du niveau.
     */
    void setName(const QString &name);

    /**
     * @brief Vérifie la validité de l'objet.
     * @return true si l'objet a des paramètres corrects, false sinon.
     */
    bool isValid() const;

    /**
     * @brief Getter
     * @return Taille de la grille (en nombre de cases).
     */
    QSize gridSize() const;

    /**
     * @brief Getter
     * @return Taille des cases de la grille (en pixels).
     */
    int tileSize() const;

    /**
     * @brief Getter
     * @return Gestionnaire de textures.
     */
    TextureHandler *textureHandler() const;
	
	/**
     * @brief Getter
     * @return Liste des index des textures utilisée par le niveau.
	 */
	QList<int> usedTextures() const;
	 
    /**
     * @brief Getter
     * @return Image de base du niveau.
     */
    QImage baseImage() const;

    /**
     * @brief Getter
     * @return Dimensions de la maison des fantômes du niveau.
     */
    QRect ghostHouse() const;

    /**
     * @brief Getter
     * @param i Index de la case à inspecter.
     * @return Index de la texture utilisée à l'index donné.
     */
    int textureAt(int i) const;

    /**
     * @brief Getter
     * @param tile Case à inspecter.
     * @return Index de la texture utilisée à la case donnée.
     */
    int textureAt(const QPoint &tile) const;
	 
    /**
     * @brief Modifie la texture au point donné.
     * @param tile Position de la case.
     * @param texture Nouvelle texture.
     * @details Si l'indice de la texture est correct et n'a pas encore été utilisé par le niveau,
     * il est ajouté à sa liste.
     */
    void setTileTexture(const QPoint &tile, int texture);

    /**
     * @brief Getter
     * @param i Index de la case à inspecter.
     * @return Type de collision utilisé à l'index donnée.
     */
    int collisionAt(int i) const;

    /**
     * @brief Getter
     * @param tile Case à inspecter.
     * @return Type de collision utilisé à la case donnée.
     */
    int collisionAt(const QPoint &tile) const;

    /**
     * @brief Setter
     * @param tile Case à modifier.
     * @param collision Nouveau type de collision à appliquer.
     */
    void setCollisionAt(const QPoint &tile, int collision);

    /**
     * @brief Calcule entièrement l'image à partir des valeurs stockées.
     * @return Image de base du niveau.
     */
    QImage renderBaseImage();

    /**
     * @brief Met à jour l'image au point donné.
     * @param changedPoint Point où la mise à jour est effectuée.
     * @return Image de base du niveau mise à jour.
     */
    QImage updateBaseImage(QPoint changedPoint);


    /**
     * @brief Trouve la case correpondant à la position dans l'image.
     * @param screenPosition Position à l'écran.
     * @return Case sous le curseur.
     */
    QPoint screenToTile(QPoint screenPosition);

    /**
     * @brief Créé une structure dont les valeurs sont nulles.
     * @return UnitData initialisée.
     */
    static UnitData initUnitData();

    /**
     * @brief Getter
     * @return Liste des données des unités.
     */
    UnitData *unitsData() const;

    /**
     * @brief Setter
     * @param unitsData Nouvelle données de l'unité.
     * @param idx Index de l'unité modifiée.
     */
    void setUnitsDataAt(UnitData unitsData, int idx);

    /**
     * @brief Setter
     * @param emptyTileTexture Index de la texture des cases vides.
     */
    void setEmptyTileTexture(int emptyTileTexture);

    /**
     * @brief Setter
     * @param pelletTileTexture Index de la texture des cases avec pastilles.
     */
    void setPelletTileTexture(int pelletTileTexture);

    /**
     * @brief Setter
     * @param energizerTileTexture Index de la texture des cases avec énergisant.
     */
    void setEnergizerTileTexture(int energizerTileTexture);

    /**
     * @brief Getter
     * @return Index de la texture des cases vides.
     */
    int emptyTileTexture() const;

    /**
     * @brief Getter
     * @return Index de la texture des cases avec pastilles.
     */
    int pelletTileTexture() const;

    /**
     * @brief Getter
     * @return Index de la texture des cases avec énergisant.
     */
    int energizerTileTexture() const;

    /**
     * @brief Getter
     * @return Timers de changement de mode des fantômes.
     */
    GhostTimer ghostTimer() const;

    /**
     * @brief Setter
     * @param ghostTimer Nouveau timers de changement de mode des fantômes.
     */
    void setGhostTimer(const GhostTimer &ghostTimer);

    /**
     * @brief Ajoute une étape à la liste des timers.
     * @param time Durée de l'étape.
     * @param mode Type d'étape.
     */
    void appendGhostTimerStep(double time, int mode);

    /**
     * @brief Getter
     * @return Durée de l'effet de peur.
     */
    double scaredTime() const;

    /**
     * @brief Setter
     * @param scaredTime Durée de l'effet de peur.
     */
    void setScaredTime(double scaredTime);

protected:
    /**
     * @brief Charge les données de la grille à partir d'un fichier XML.
     * @param elem Element XML d'où les données de la grille sont chargées.
     */
    void loadGrid(QDomElement elem);

    /**
     * @brief Charge le timer des fantômes à partir d'un fichier XML.
     * @param elem Element XML d'où les données du timer sont chargées.
     */
    void loadGhostTimer(QDomElement elem);

private:
    QString m_name;             /** @brief Nom du niveau. */
    int *m_gridTextures;        /** @brief Grille des textures (indexée). */
    int *m_collisionsGrid;      /** @brief Grille des collisions. */
    QSize m_gridSize;           /** @brief Taille de la grille. */
    int m_tileSize;           /** @brief Taille d'une case (en pixels). */
    TextureHandler *m_textureHandler;   /** @brief Gestionnaire de textures. */
    QList<int> m_usedTextures;  /** @brief Index des textures employées. */
    int m_emptyTileTexture;     /** @brief Texture utilisée pour les cases vides. */
    int m_pelletTileTexture;    /** @brief Texture utilisée pour les cases avec pastille. */
    int m_energizerTileTexture; /** @brief Texture utilisée pour les cases avec énegisant. */
    QImage m_baseImage;         /** @brief Image de base de la grille. */
    QRect m_ghostHouse;         /** @brief Dimensions de la maison des fantômes */
    UnitData *m_unitsData;      /** @brief Données des unités */
    GhostTimer m_ghostTimer;    /** @brief Timers de changement de mode des fantômes. */
    double m_scaredTime;        /** @brief Durée de l'effet de peur */

    bool m_isValid;             /** @brief Flag de validité de l'objet. */
};

#endif // EDITORLEVEL_H

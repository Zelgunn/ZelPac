#ifndef LEVEL_H
#define LEVEL_H

#include <QDomDocument>
#include <QTimer>

#include "grid.h"
#include "pacmanunit.h"
#include "ghost.h"
#include "pacmanparameters.h"
#include "ghosttimer.h"
#include "texturehandler.h"

/**
 * @brief La classe de Niveau de Pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Cette classe s'occupe des paramètres propres à un seul niveau
 * (Grille, Nombre de pastilles/énergisants, Unités...).
 */

class Level : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base de la classe Level.
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @param textureHandler Gestionnaire de texture déjà instancié d'où la grille du niveau va extraire les textures.
     */
    Level(QDomElement elem, TextureHandler *textureHandler);

    ~Level();

    /**
     * @brief Getter
     * @return Joueur du niveau.
     */
    PacmanUnit *player() const;

    /**
     * @brief Getter
     * @param ghostIdx Identificateur du fantôme.
     * @return Fantôme du niveau avec pour identificateur celui donné en paramètre.
     */
    Ghost ghost(int ghostIdx) const;

    /**
     * @brief Modifie le comportement des fantômes de ce niveau.
     * @param ghostsChasingMode Nouveau comportement.
     */
    void setGhostsChasingMode(int ghostsChasingMode);

    /**
     * @brief Active le mode Appeuré des fantômes.
     * @param scared Si true, active le mode Appeuré, sinon le désactive.
     * @details (Dés)Active le mode Appeuré et s'il n'est pas déjà (in)actif,
     * force les fantômes à changer de sens et met à jour leur vitesse.
     */
    void setGhostsScared(bool scared);

    /**
     * @brief Getter
     * @return Nombre total de pastilles au début du niveau.
     */
    int pelletCount() const;

    /**
     * @brief Getter
     * @return Nombre total de pastilles mangées depuis le début du niveau.
     */
    int eatenPellets() const;

    /**
     * @brief Modifie le nombre de pastilles mangées par pacman.
     * @param eatenPellets Nouveau nombre de pastilles mangées.
     */
    void setEatenPellets(int eatenPellets);

    /**
     * @brief Getter
     * @return Grille du niveau.
     */
    Grid *grid() const;

    /**
     * @brief Change de grille utilisée par le niveau.
     * @param grid Nouvelle grille.
     */
    void setGrid(Grid *grid);

    /**
     * @brief Getter
     * @return Temps pendant le lequel un énergisant fait effet (avant les flashs).
     */
    int scaredDuration() const;

    /**
     * @brief Getter
     * @return Nombre de flashs d'avertissement avant fin de l'Appeuré.
     */
    int flashsCount() const;

    /**
     * @brief Tente de modifier la direction du pacman.
     * @param direction Nouvelle direction du pacman.
     */
    void setPacmanDirection(int direction);

    /**
     * @brief Vérifie si les unités ne se trouvent pas trop proches les unes des autres.
     * @return Liste des fantômes qui sont trop proches de pacman.
     */
    QList<Ghost *> checkUnitsPositions();

    /**
     * @brief Récupère l'image de base de la grille et y ajoute les unités.
     * @return Image après ajout des unités sur le terrain.
     */
    QPixmap *render();

    /**
     * @brief Calcule le déplacement du pacman du niveau.
     */
    void movePacman();

    /**
     * @brief Calcule le déplacement des fantômes du niveau.
     */
    void moveGhosts();

    /**
     * @brief Met à jour le timer qui gère le mode des fantômes.
     */
    void updateTimer();

    /**
     * @brief Réinitialise la position de toutes unités à leur point de départ.
     */
    void resetUnitsPosition();

    /**
     * @brief Initalise le niveau.
     */
    void startLevel();

    /**
     * @brief Getter
     * @return Nom du niveau.
     */
    QString name() const;

public slots:
    /**
     * @brief Répercute les conséquences d'un énergisant sur le mode Appeuré.
     */
    void onEnergizerEaten();
    /**
     * @brief Démarre le clignotement des fantômes.
     */
    void onEnergizerEnding();

    /**
     * @brief Permet d'inverser le flash, faisant clignoter les fantômes.
     */
    void reverseFlash();

    /**
     * @brief Quitte le mode Appeuré.
     */
    void leaveScaredMove();

protected:
    /**
     * @brief Ajoute toutes les unités à l'image
     * @param[out] res Image modifiée (passage par pointeur pour éviter la copie).
     */
    void addUnitsToImage(QPixmap *res);
    /**
     * @brief Ajoute tous les fantômes à l'image
     * @param[out] res Image modifiée (passage par pointeur pour éviter la copie).
     */
    void addGhostsToImage(QPixmap *res);
    /**
     * @brief Ajoute pacman à l'image
     * @param[out] res Image modifiée (passage par pointeur pour éviter la copie).
     */
    void addPacmanToImage(QPixmap *res);

private:
    /** @brief Pointeur sur le pacman de ce niveau. */
    PacmanUnit *m_player;
    /** @brief Tableau contenant les fantômes de ce niveau. */
    Ghost m_ghosts[4];
    /** @brief Nombre de pastilles total. */
    int m_pelletCount;
    /** @brief Nombre de pastilles mangées. */
    int m_eatenPellets;
    /** @brief Grille du niveau. */
    Grid *m_grid;
    /** @brief Timer des modes des fantômes du niveau. */
    GhostTimer m_ghostTimer;
    /** @brief Durée du mode Appeuré du niveau. */
    int m_scaredDuration;
    /** @brief Nombre de flashs avant la fin du mode Appeuré du niveau. */
    int m_flashsCount;
    /** @brief Flag de clignotement. */
    bool m_flashOn;
    /** @brief Timer de clignotement. */
    QTimer *m_flasher;
    /** @brief Gestionnaire de texture. */
    TextureHandler *m_textureHandler;
    /** @brief Nom du niveau */
    QString m_name;
};

#endif // LEVEL_H

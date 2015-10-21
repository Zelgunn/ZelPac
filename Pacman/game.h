#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QDomDocument>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QSize>
#include <QMediaPlayer>

#include "level.h"
#include "pacmanparameters.h"
#include "texturehandler.h"

/**
 * @brief La classe Game gère l'ensemble des paramètres de Pacman
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La classe Game s'occupe de gérer l'ensemble des niveaux, en leur faisant
 *      calculer les déplacements des unités. La classe Game s'occupe aussi de tous
 *      les éléments qui sont communs à tous les niveaux (tels que le score ou le
 *      nombre de vies du Pacman).
 *      Enfin, la classe Game effectue le rendu final de la grille (sans les à côtés commme
 *      l'affichage du score) qu'il fait calculer au niveau actuellement en cours.
 *
 */

class Game : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base de la classe Game.
     * @param dom Document XML d'où le jeu charge toutes les données.
     *
     */
    Game(QDomDocument *dom);

    /**
     * @brief Destructeur de la classe
     */
    ~Game();

    /**
     * @brief Démarre le jeu avec FPS (cf. pacmanparameters.h) images par seconde.
     * @details Démarre le QTimer m_timer qui se délenche toutes les TIME_BETWEEN_FRAMES
     *      secondes (FPS = 1000.0/TIME_BETWEEN_FRAMES). A chaque déclenchement de m_timer,
     *      une nouvelle frame est calculée.
     */
    void startGame();

    /**
     * @brief Getter
     * @return Niveau actuellement utilisé par le Jeu.
     */
    int currentLevel() const;

    /**
     * @brief Setter
     * @param currentLevel Change le niveau utilisé par le Jeu.
     */
    void setCurrentLevel(int currentLevel);

    /**
     * @brief Getter
     * @return La liste de tous les niveaux du jeu.
     */
    QList<Level*> levels() const;

    /**
     * @brief Getter
     * @return Score actuel du joueur.
     */
    int score() const;

    /**
     * @brief Getter
     * @return Nombre de vies du joueur.
     */
    int lifes() const;

    /**
     * @brief Getter
     * @return Timer du Jeu.
     */
    QTimer *timer() const;

    /**
     * @brief Getter
     * @return Grille du jeu sous forme d'image.
     */
    QPixmap image();

    /**
     * @brief Modifie la direction actuelle du joueur.
     * @param direction Nouvelle direction
     * @details Essaie de modifier la direction actuelle du joueur par le paramètre.
     *      Si le joueur ne peut pas effectuer ce mouvement immédiatement, la direction
     *      est stockée et sera prise dès que possible.
     *      Si une direction est déjà stockée afin d'être utilisée dès que possible, elle est écrasée.
     *      Si la nouvelle direction est égale à la direction actuelle, la prochaine direction est
     *      écrasée (afin d'annuler la commande).
     */
    void setPacmanDirection(int direction);

    /**
     * @brief Getter
     * @return Vrai si le jeu est en pause suite à un changement de niveau.
     */
    bool isChangingLevel() const;

    /**
     * @brief Calcul l'image que donnerait le jeu s'il commençait tout juste.
     * @return Première frame du jeu.
     */
    QPixmap *firstFrame() const;

    /**
     * @brief Getter
     * @return Vrai si le joueur a gagné, sinon Faux.
     */
    bool victory() const;

    /**
     * @brief Getter
     * @return Vrai si le joueur a perdu, sinon Faux.
     */
    bool defeat() const;

public slots:
    /**
     * @brief Calcule la prochaine frame.
     */
    void nextGameFrame();

    /**
     * @brief Relance le timer du jeu.
     */
    void resumeGame();

protected slots:
    /**
     * @brief Modifie les valeurs du jeu lorsque le joueur mange une pastille.
     */
    void onPelletEaten();

    /**
     * @brief Modifie les valeurs du jeu lorsque le joueur mange un énergisant et fait peur aux fantômes.
     */
    void onEnergizerEaten();

signals:
    /**
     * @brief Signal émit lorsque le joueur gagne ou perd.
     */
    void gameFinished();

private:
    /** @brief Index du niveau actuel. */
    int m_currentLevel;
    /** @brief Liste de tous les niveaux chargés. */
    QList<Level*> m_levels;
    /** @brief Score du joueur. */
    int m_score;
    /** @brief Nombre de vie du joueur. */
    int m_lifes;
    /** @brief Nombre de fantômes attrapés en un seul énergisant. */
    int m_consecutivesEnergizers;
    /** @brief Timer du Jeu (calcule de frame). */
    QTimer *m_timer;
    /** @brief Rendu de la grille. */
    QPixmap m_image;
    /** @brief Rendu audio du jeu. */
    QMediaPlayer *m_shortSoundsPlayer;
    /** @brief Gestionnaire de textures. */
    TextureHandler m_textureHandler;
    /** @brief Flag de changement de niveau */
    bool m_isChangingLevel;
    /** @brief Flag de victoire */
    bool m_victory;
    /** @brief Flag de défaite */
    bool m_defeat;
    /** @brief Flag pour vérifier si l'image du niveau courant est déjà calculée */
    bool m_currentLevelImageReady;
};

#endif // GAME_H

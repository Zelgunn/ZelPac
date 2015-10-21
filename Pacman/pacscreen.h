#ifndef PACSCREEN_H
#define PACSCREEN_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QShortcut>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>

#include "game.h"
#include "arduinohandler.h"
#include "firework.h"

/**
 * @brief Widget affichant l'ensemble du jeu.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Grâce à l'utilisation d'un PaintEvent, cette classe fille de QWidget permet d'afficher
 *      tous les éléments graphiques du jeu en plein écran.
 *
 * Cette classe permet aussi de recevoir les commandes utilisateurs venant du clavier ou d'une manette
 * Arduino Uno.
 */

class Pacscreen : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base de la classe Pacscreen.
     * @param dataFile Fichier chargé par le jeu contenant l'intégralité des paramètres modulables.
     * @param parent Parent de la fenêtre
     */
    explicit Pacscreen(QString dataFile = ":/textures/pacman.xml", QWidget *parent = 0);

    /**
     * @brief Destructeur de la classe Pacscreen.
     */
    ~Pacscreen();

    /**
     * @brief Démarre une session de jeu.
     * @param filename Fichier XML contenant les paramètres de la session.
     */
    void initGame(const QString &filename = ":/textures/pacman.xml");

    /**
     * @brief Fonction de traitement des entrées de flèches directionnelles.
     * @param direction Touche appuyée.
     * @details Lorsque cette fonction est appelée, si le jeu est totalement arrêté (victoire ou défaite),
     *  alors il redémarre. Dans le cas où celui ci est uniquement en pause (perte de vie, fin de niveau) alors il reprend.
     *  Sinon, le signal de la touche est propagé afin de changer la direction du joueur.
     */
    void onKeyboardInput(int direction);

    /**
     * @brief Marque une pause entre une victoire ou une défaite du joueur avant de continuer.
     * @return Vrai si la pause est terminée ou s'il n'y a pas de pause, sinon Faux.
     */
    bool restingTimeOff();

protected:
    /**
     * @brief paintEvent de la classe, afin d'afficher l'ensemble du jeu.
     * @details Cet évenement est appelé à chaque fois que le timer du Jeu calcule une frame et affiche
     *      la frame (il peut y avoir une frame d'écart, soit 16-17 ms).
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief Dessine les vies du pacman sur le PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant les vies.
     */
    void paintLifes(QPainter *painter);

    /**
     * @brief Dessine le score du pacman sur le PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant le score.
     */
    void paintScore(QPainter *painter);

    /**
     * @brief Dessine les FPS du jeu sur le PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant les FPS.
     */
    void paintFPS(QPainter *painter);

    /**
     * @brief Dessine le nom du niveau sur le PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant le nom du niveau.
     */
    void paintLevelName(QPainter *painter);

    /**
     * @brief Dessine le jeu sur PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant le jeu.
     */
    void paintGame(QPainter *painter);

    /**
     * @brief Dessine la première image du jeu sur PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant la première image du jeu.
     */
    void paintFirstFrame(QPainter *painter);

    /**
     * @brief Dessine l'animation d'introduction sur PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant l'animation d'introduction.
     */
    void paintStartAnimation(QPainter *painter);

    /**
     * @brief Dessine l'animation de victoire sur PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant l'animation de victoire.
     */
    void paintVictoryAnimation(QPainter *painter);

    /**
     * @brief Dessine l'animation de défaite sur PaintDevice sur lequel est branché le painter.
     * @param painter Painter dessinant l'animation de défaite.
     */
    void paintDefeatAnimation(QPainter *painter);

private slots:
    /**
     * @brief Modifie la direction du Pacman vers la Droite.
     */
    void on_Right();
    /**
     * @brief Modifie la direction du Pacman vers la Gauche.
     */
    void on_Left();
    /**
     * @brief Modifie la direction du Pacman vers le Haut.
     */
    void on_Up();
    /**
     * @brief Modifie la direction du Pacman vers le Bas.
     */
    void on_Down();

    /**
     * @brief Réveille le jeu si celui-ci est en pause (fin de jeu, fin de niveau, ...).
     */
    void on_Enter();

    /**
     * @brief Fin d'une session de jeu.
     */
    void on_GameFinished();

private:
    /** @brief Pointeur sur le Jeu. */
    Game *m_game;
    /** @brief Gestionnaire de connexion Arduino Uno. */
    ArduinoHandler *m_handler;
    /** @brief Audio Player. */
    QMediaPlayer *m_audioPlayer;
    /** @brief Playlist du player. */
    QMediaPlaylist *m_playlist;
    /** @brief Moment de la dernière frame. */
    QTime m_lastFrameTime;
    /** @brief Nombre d'images par seconde réel. */
    int m_fps;
    /** @brief Nombre d'images affichées depuis le démarrage. */
    int m_frameCount;
    /** @brief Timer de rafraichissement d'images */
    QTimer *m_timer;
    /** @brief Flag qui vérifie si l'animation d'introduction a été jouée. */
    bool m_startAnimationDone;
    /** @brief Feux d'artifices tirés en cas de victoire (classe pour animation compartimentée) */
    QList<Firework *> m_fireworks;
    /** @brief Marqueur temporel pour éviter que le joueur ne relance immédiatement le jeu */
    QTime m_restingTime;
};

#endif // PACSCREEN_H

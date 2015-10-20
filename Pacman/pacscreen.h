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
     * @param parent Parent de la fenêtre
     */
    explicit Pacscreen(QWidget *parent = 0);

    /**
     * @brief Destructeur de la classe Pacscreen.
     */
    ~Pacscreen();

    /**
     * @brief Démarre une session de jeu.
     * @param filename Fichier XML contenant les paramètres de la session.
     */
    void initGame(const QString &filename = ":/textures/pacman.xml");

protected:
    /**
     * @brief paintEvent de la classe, afin d'afficher l'ensemble du jeu.
     * @details Cet évenement est appelé à chaque fois que le timer du Jeu calcule une frame et affiche
     *      la frame (il peut y avoir une frame d'écart, soit 16-17 ms).
     */
    void paintEvent(QPaintEvent *);

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
};

#endif // PACSCREEN_H

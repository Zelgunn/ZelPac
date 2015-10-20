#ifndef PACMANUNIT_H
#define PACMANUNIT_H

#include "unit.h"

/**
 * @brief Classe de l'unité Pacman
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Cette classe héritée de la classe Unit permet d'ajouter quelques fonctionnalités
 * uniques au Pacman (comme l'enregistrement d'un coup en avance) ainsi qu'une aide pour le dessin
 * du Pacman.
 */

class PacmanUnit : public QObject, public Unit
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base du PacmanUnit.
     * @param parent Parent de l'objet.
     * @details Note : Cette version du constructeur n'est pas destinée à être utilisée en dehors
     *      de l'initialisation d'une classe qui la contient. L'objet créé n'est pas valide.
     */
    PacmanUnit(QObject * parent = 0);

    /**
     * @brief Surcharge du constructeur (avec grille) pour utiliser XML.
     * @param grid La grille sur laquelle se trouve Pacman.
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @param parent
     * @details Construit un pacman (unité) attaché à une grille et dont les paramètres sont chargés à partir d'une balise XML.
     *  - speed : Vitesse par défaut de l'unité.
     *  - sspeed : Vitesse lorsque les fantômes sont apeurés.
     *  - Autres : Voir DoublePosition(QDomElement elem).
     *
     * Exemple : \<Player ix="14" iy="23" speed="77" sspeed = "91" fx = "half"/>
     */
    PacmanUnit(Grid *grid, QDomElement elem, QObject * parent = 0);

    /**
     * @brief Fait passer le dessin du pacman à la prochain frame (pas de calul d'image).
     */
    void nextFrame();
    /**
     * @brief Fait directement passer le dessin du pacman à la frame donnée (pas de calul d'image).
     * @param frame Nouvelle frame du pacman.
     * @details Utile pour créer un Pacman Dummy afin d'afficher une position particulière.
     */
    void setFrame(int frame);

    /**
     * @brief Getter
     * @return Frame actuelle du pacman.
     */
    int frame() const;

    /**
     * @brief Modifie la prochaine direction que prendra Pacman dès qu'il pourra.
     * @param direction Nouvelle prochaine direction.
     */
    void setNextDirection(int direction);

    /**
     * @brief Calcule le prochain déplacement du Pacman.
     */
    void move();

    /**
     * @brief Tente de modifier la direction du pacman.
     * @param direction Nouvelle direction.
     * @details Essaie de modifier la direction actuelle du joueur par le paramètre.
     *      Si le joueur ne peut pas effectuer ce mouvement immédiatement, la direction
     *      est stockée et sera prise dès que possible.
     *      Si une direction est déjà stockée afin d'être utilisée dès que possible, elle est écrasée.
     *      Si la nouvelle direction est égale à la direction actuelle, la prochaine direction est
     *      écrasée (afin d'annuler la commande).
     */
    void setDirection(int direction);

    /**
     * @brief Réinitialise la position de Pacman à son origine.
     */
    void resetPosition();

    /**
     * @brief Calcule l'image de Pacman en fonction de sa direction et de sa frame actuelle.
     * @param size Taille de l'image résultante.
     * @return Image du Pacman faisant face dans la direction du Pacman et à la bonne frame.
     */
    QPixmap renderPacmanImage(int size = 24);

signals:
    /**
     * @brief Emit lorsque le Pacman arrive sur une case avec une pastille.
     */
    void pelletEaten();
    /**
     * @brief Emit lorsque le Pacman arrive sur une case avec un énergisant.
     */
    void energizerEaten();

private:
    /** @brief Frame actuelle du Pacman. */
    int m_frame;
    /** @brief Direction actuelle du Pacman. */
    int m_nextDirection;
};

/**
 * @brief Variable globale du pacman.
 */
extern PacmanUnit *pacman;

#endif // PACMANUNIT_H

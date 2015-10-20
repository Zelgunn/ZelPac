#ifndef GHOST_H
#define GHOST_H

#include "pacmanunit.h"
#include "pacmanparameters.h"

/**
 * @brief Classe d'unité de type Fantôme avec les comportements de chaque fantôme.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La classe crée dès le lancement du programme les 4 fantômes en variables globales
 *      afin d'y accéder à partir de n'importe où. Les méthodes de la classe permettent aux
 *      fantômes d'avoir différents comportement, en fonction de leur identité ou de leur
 *      état actuel.
 */

class Ghost : public Unit
{
public:
    /**
     * @brief Identités des fantômes.
     */
    enum GhostsID{Blinky,   /**< Shadow "Blinky" */
                 Pinky,     /**< Speedy "Pinky" */
                 Inky,      /**< Bashful "Inky" */
                 Clyde      /**< Pokey "Clyde" */
                 };

    /**
     * @brief Modes de poursuite des fantômes.
     */
    enum ghostChasingModeID{Chase,    /**< Les fantômes utilisent leur IA personnelle pour attraper le Pacman */
                           Scatter,   /**< Les fantômes se dirigent vers leur coin spécifique */
                           Caught     /**< Les fantômes attrapés se dirigent vers leur maison et y reste jusqu'à expiration */
                           };
    /**
     * @brief Constructeur de base de la classe Ghost.
     * @details Note : Cette version du constructeur n'est pas destinée à être utilisée en dehors
     *      de l'initialisation d'une classe qui la contient. L'objet créé n'est pas valide.
     */
    Ghost();
    /**
     * @brief Surchage du constructeur de base de la classe Ghost.
     * @param grid La grille sur laquelle se trouve le fantôme.
     * @param id Identité du fantôme (cf. GhostsID).
     * @details Un fantôme créé par ce constructeur est valide mais dispose de tous les paramètres par défaut.
     */
    Ghost(Grid *grid, int id);
    /**
     * @brief Surcharge du constructeur pour utiliser XML.
     * @param grid La grille sur laquelle se trouve le fantôme.
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @param id Identité du fantôme (cf. GhostsID).
     * @details Construit un fantôme attachée à une grille et dont les paramètres sont chargés à partir d'une balise XML.
     *  - speed : Vitesse par défaut de l'unité.
     *  - sspeed : Vitesse lorsque les fantômes sont apeurés.
     *  - cornerx : Coordonnée horizontale du coin du fantôme.
     *  - cornery : Coordonnée verticale du coin du fantôme.
     *  - Autres : Voir DoublePosition(QDomElement elem).
     *
     * Les vitesses doivent être proches d'un multiple de MAX_FLOAT_POS. Par exemple, avec MAX_FLOAT_POS
     * égal à 1000, 80 donne un résultat assez éloigné (80*12 = 960, 80*13 = 1040) alors que 91
     * donne une valeur très proche (91*11 = 1001).
     * Si la valeur est trop éloignée, les unités auront des erreurs dans leur déplacement.
     * De base, l'écart maximal absolu est de 25.
     *
     * A la construction, l'élément XML doit aussi contenir la position qu'aura l'unité.
     *
     * Exemple : \<Blinky ix="13" iy="11" speed="71" sspeed = "50" cornerx="26" cornery="-2" fx = "half"/>
     */
    Ghost(Grid *grid, QDomElement elem, int id);

    /**
     * @brief Vérifie si le fantôme se trouve dans la maison définie par sa grille.
     * @return true si le fantôme se trouve dans la maison, sinon false.
     */
    bool isInsideGhostHouse() const;

    /**
     * @brief Calcule la prochaine position du fantôme en fonction de tous les paramètres et l'y déplace.
     */
    void move();

    /**
     * @brief Inverse le sens de déplacement du fantôme (lors des changements de mode de poursuite).
     */
    void reverseDirection();

    /**
     * @brief Choisit la meilleure direction parmi les options qui s'offrent au fantômes.
     * @param tileTarget Cible pour trouver la meilleure direction.
     * @param ignorePreviousDirection false de base. Si true, le fantôme ignore le fait qu'il n'a
     *      pas le droit de se retourner en temps normal.
     * @return Meilleure direction à suivre (cf. Unit::Directions).
     * @details Le fantôme regarde les quatres cases qui l'entourent et empile celles où il peut se déplacer.
     *
     * Ensuite il cherche celle dans la pile qui a la plus courte distance euclidienne vers la cible.
     *
     * En temps normal, un fantôme ne peut pas se retourner ni se déplacer sur une case avec la collision activée.
     */
    int findNextdirection(QPoint tileTarget, bool ignorePreviousDirection = false);

    /**
     * @brief Getter
     * @return Etat de poursuite actuel du fantôme.(cf. ghostChasingModeID).
     */
    int state() const;

    /**
     * @brief Modifie l'état de poursuite du fantôme.
     * @param state Nouvel état de poursuite.
     */
    void setState(int state);

    /**
     * @brief Getter
     * @return true si le fantôme a le droit de sortir.
     * @details De base, seuls Blinky et Pinky sont capables de sortir. Inky attend que Pacman ait mangé 30 pastilles
     *      et Clyde attend qu'il en ai mangé un tiers.
     */
    bool isFree() const;

    /**
     * @brief Setter
     * @param free Si true alors le fantôme devient capable de sortir de la maison et démarre son IA.
     */
    void setFree(bool free);

    /**
     * @brief Vérifie si la partie réelle de la position du fantôme est nulle.
     * @return true si la partie réelle n'est pas nulle, sinon false.
     */
    bool isOnTheMove() const;

    /**
     * @brief Getter
     * @return true si l'état est à Chase, sinon false.
     */
    bool isChasing() const;

    /**
     * @brief Getter
     * @return true si l'état est à Scatter, sinon false.
     */
    bool isScattering() const;

    /**
     * @brief Marque le fantôme comme attrapé.
     */
    void setCaught();

    /**
     * @brief Getter
     * @return true si le fantôme est attrapé, false sinon.
     */
    bool isCaught() const;

    /**
     * @brief Getter
     * @return Le coin du mode Scatter du fantôme.
     */
    QPoint scatterCorner() const;

    /**
     * @brief Modifie le comportement en Appeuré ou Normal.
     * @param scared Si true, alors le comportement devient Appeuré sinon Normal.
     */
    static void setGhostsScared(bool scared);

    /**
     * @brief Getter
     * @return true si les fantômes sont appeurés, sinon false.
     */
    static bool areGhostsScared();

    /**
     * @brief Dessine l'image du fantôme en fonction de ses paramètres (identité, direction, ...).
     * @param size Taille de l'image du fantôme.
     * @return Image finale du fantôme.
     */
    QPixmap ghostImage(int size = 24);

    /**
     * @brief Dessine les yeux du fantôme en fonction de sa direction.
     * @param size Taille de l'image finale.
     * @return Images des yeux du fantôme.
     */
    QPixmap ghostEyesImage(int size = 24);

    /**
     * @brief Dessine l'image d'un fantôme effrayé (flashant ou non).
     * @param flashOn Si true, alors le fantôme est blanc, sinon bleu foncé.
     * @param size Taille de l'image finale.
     * @return Image de fantôme effrayé.
     */
    static QPixmap scaredGhostImage(bool flashOn = false, int size = 24);

protected:
    /**
     * @brief Getter
     * @return Cible de Blinky
     */
    static QPoint blinkyChaseTarget();

    /**
     * @brief Getter
     * @return Cible de Pinky
     */
    static QPoint pinkyChaseTarget();

    /**
     * @brief Getter
     * @return Cible de Inky
     */
    static QPoint inkyChaseTarget();

    /**
     * @brief Getter
     * @return Cible de Clyde
     */
    static QPoint clydeChaseTarget();

    /**
     * @brief Déplacement au sein d'une même case.
     */
    void localMove();

    /**
     * @brief Sélection d'une nouvelle direction pour poursuivre le Pacman.
     */
    void chase();

    /**
     * @brief Sélection d'une nouvelle direction pour aller vers le coin du fantôme.
     */
    void scatter();

    /**
     * @brief Sélection aléatoire d'une direction pour fuir le Pacman.
     */
    void escape();

    /**
     * @brief Sélection d'une nouvelle direction vers la maison et saut d'une case d'un coup.
     */
    void goHome();

    /**
     * @brief Libère le fantôme de la maison pour partir à la chasse.
     */
    void release();

private:
    /** @brief Direction précédente du fantôme qu'il lui est interdit de prendre. */
    int m_previousDirection;
    /** @brief Etat de poursuite actuel du fantôme. */
    int m_state;
    /** @brief Identité du fantôme (cf. GhostsID). */
    int m_id;
    /** @brief Flag libre ou non. */
    bool m_free;
    /** @brief Coin du fantôme. */
    QPoint m_scatterCorner;
};

extern Ghost *blinky,   /** @brief Shadow "Blinky" */
             *pinky,    /** @brief Speedy "Pinky" */
             *inky,     /** @brief Bashful "Inky" */
             *clyde;    /** @brief Pokey "Clyde" */

#endif // GHOST_H

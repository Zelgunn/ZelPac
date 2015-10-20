#ifndef UNIT_H
#define UNIT_H

#include <QPoint>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QDomElement>

#include "pacmanparameters.h"
#include "doubleposition.h"
#include "grid.h"

/**
 * @brief Classe représentant une unité du pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details La classe Unit fournit les éléments de base afin de diriger une unité à travers
 *      une grille de Pacman.
 */
class Unit
{
public:
    /**
     * @brief Enum des Directions
     */
    enum Directions{Up,     /**< Vers le Haut */
                    Down,   /**< Vers le Bas */
                    Left,   /**< Vers la Gauche */
                    Right,  /**< Vers la Droite */
                    DirectionNotValid = -1 /**< Direction invalide */
                   };
    /**
     * @brief Constructeur de base de la classe Unit.
     * @details Note : Cette version du constructeur n'est pas destinée à être utilisée en dehors
     *      de l'initialisation d'une classe qui la contient. L'objet créé n'est pas valide.
     */
    Unit();

    /**
     * @brief Surcharge du constructeur de base référençant la grille.
     * @param grid La grille sur laquelle se trouve l'unité.
     * @details Une unité créée par ce constructeur est valide mais dispose de tous les paramètres par défaut.
     */
    Unit(Grid *grid);

    /**
     * @brief Surcharge du constructeur (avec grille) pour utiliser XML.
     * @param grid La grille sur laquelle se trouve l'unité.
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @details Construit une unité attachée à une grille et dont les paramètres sont chargés à partir d'une balise XML.
     *  - speed : Vitesse par défaut de l'unité.
     *  - sspeed : Vitesse lorsque les fantômes sont apeurés.
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
     * Exemple : \<Player ix="14" iy="23" speed="77" sspeed = "91" fx = "half"/>
     */
    Unit(Grid *grid, QDomElement elem);

    /**
     * @brief Getter
     * @return Position entière de l'unité.
     */
    QPoint position() const;

    /**
     * @brief Getter
     * @return Position réelle de l'unité.
     */
    QPoint positionInTile() const;

    /**
     * @brief Getter
     * @return DoublePosition de l'unité (partie entière et flottante).
     */
    DoublePosition fullPosition() const;

    /**
     * @brief Setter
     * @param direction Nouvelle direction de l'unité.
     */
    void setDirection(int direction);

    /**
     * @brief Getter
     * @return Direction actuelle de l'unité.
     */
    int direction() const;

    /**
     * @brief Getter
     * @return Vitesse actuelle de l'unité.
     */
    char speed() const;

    /**
     * @brief Setter
     * @param speed Nouvelle vitesse actuelle de l'unité.
     */
    void setSpeed(char speed);

    /**
     * @brief Getter
     * @return Vitesse lorsque les fantômes sont appeurés.
     */
    char scaredSpeed() const;

    /**
     * @brief Setter
     * @param scaredSpeed Nouvelle vitesse lorsque les fantômessont appeurés.
     */
    void setScaredSpeed(char scaredSpeed);

    /**
     * @brief Getter
     * @return Vitesse de base de l'unité.
     */
    char baseSpeed() const;

    /**
     * @brief Setter
     * @param baseSpeed Nouvelle vitesse de base de l'unité.
     */
    void setBaseSpeed(char baseSpeed);

    /**
     * @brief Modifie la vitesse actuelle, soit par celle de base, soit par la vitesse lorsque
     *  les fantômes ont peur.
     * @param reverse Si reverse est Vrai alors on utilise la vitesse lorsque les fantômes
     *  ont peur, sinon on prend celle de base.
     */
    void reverseSpeeds(bool reverse);

    /**
     * @brief Déplace l'unité sur la case ciblée.
     * @param tile Case cible.
     */
    void moveToTile(QPoint tile);

    /**
     * @brief Déplace l'unité sur la prochaine case dans la direction choisie.
     * @param direction Direction choisie de déplacement.
     */
    void moveToTile(int direction);

    /**
     * @brief Déplace l'unité de sa vitesse dans la direction choisie.
     * @param direction Direction choisie de déplacement.
     * @details Déplace l'unité de (environ) sa vitesse actuelle dans la direction
     *  donnée en paramètre. La valeur est ajoutée/soustraire à la valeur réelle de
     *  la position actuelle de l'unité.
     *
     *  Si la nouvelle valeur excède la largeur de la grille ou se retrouve sous 0, l'unité
     *  est déplacée de l'autre côté de la grille.
     */
    void moveTo(int direction);

    /**
     * @brief Réinitialise la position de l'unité à sa position d'origine.
     * @details Déplace l'unité à la position qu'il avait lorsqu'elle a été créée.
     */
    void resetPosition();

    /**
     * @brief Vérifie si l'unité peut se déplacer dans la direction donnée.
     * @param direction Direction à vérifier.
     * @return False si la collision est égale à Grid::CollisionOn sinon True.
     * @details La fonction calcule la nouvelle position dans la direction choisie et regarge la collision
     *      à cette nouvelle position. Si la collision est On alors on renvoie Faux sinon Vrai.
     *
     * Note : Il est possible d'ajouter des types spéciaux de collision, permettant de se déplacer avec de nouvelles
     *  valeurs.
     */
    bool isMoveLegal(int direction);

    /**
     * @brief Calcule l'écart selon la direction donnée
     * @param direction Direction de l'écart
     * @return Ecart (norme toujours égale à 1) dans la direction donnée.
     */
    static QPoint directionToMove(int direction);

    /**
     * @brief Calcule la distance euclidienne d'un point à l'autre.
     * @param p1 Premier point
     * @param p2 Second point
     * @return Distance euclidienne entre p1 et p2 (valeur réelle)
     * @details racine(x1 - x2)^2 + (y1 - y2)^2)
     */
    static qreal euclidianDistance(QPoint p1, QPoint p2);

protected:
    // Variables
    /** @brief Position actuelle de l'unité*/
    DoublePosition m_position;
    /** @brief Position de base de l'unité*/
    DoublePosition m_origine;
    /** @brief Direction actuelle de l'unité*/
    char m_direction;        // cf enum Directions
    /** @brief Vitesse actuelle de l'unité*/
    char m_speed;            // Entre 0 et 100
    /** @brief Vitesse de l'unité lorsque les fantômes ont peur*/
    char m_scaredSpeed;
    /** @brief Vitesse de base de l'unité*/
    char m_baseSpeed;
    /** @brief Grille sur laquelle circule l'unité*/
    Grid *m_grid;
};

#endif // UNIT_H

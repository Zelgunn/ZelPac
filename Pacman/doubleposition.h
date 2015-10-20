#ifndef DOUBLEPOSITION_H
#define DOUBLEPOSITION_H

#include <QPoint>
#include <QDomElement>
#include <QtCore/qmath.h>
#include "pacmanparameters.h"

/**
 * @brief Classe imitant une position à virgule avec deux entiers.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details DoublePosition utilise deux couples d'entiers afin de simuler un
 *      couple de réels afin d'éviter les pertes de précisions dûes à la
 *      représentation binaire.
 */

class DoublePosition
{
public:
    /**
     * @brief Constructeur de base DoublePosition
     * @details Constuit une DoublePosition dont les valeurs sont nulles.
     */
    DoublePosition();

    /**
     * @brief Surcharge du constructeur de base.
     * @param[in] intPos    Partie entière de la position
     * @param[in] floatPos  Partie réelle de la position
     * @details Constuit une DoublePosition dont les valeurs sont égales
     *      aux paramètres intPos et floatPos donnés.
     */
    DoublePosition(QPoint intPos, QPoint floatPos);

    /**
     * @brief Surcharge du constructeur de base pour XML.
     * @param[in] elem Balise XML contenant les paramètres de la position
     * @details Construit une DoublePosition en fonction de l'élément XML donné.
     *      - ix : Coordonnée X entière.
     *      - iy : Coordonnée Y entière.
     *      - fx : Coordonnée X réelle. (half pour une demi-case, 0 pour les autres valeurs).
     *      - fy : Coordonnée Y réelle. (half pour une demi-case, 0 pour les autres valeurs).
     */
    DoublePosition(QDomElement elem);

    /**
     * @brief Getter (m_integerPosition).
     * @return Partie entière de la position.
     */
    QPoint integerPosition() const;

    /**
     * @brief Getter (m_floatPosition).
     * @return Partie réelle de la position.
     */
    QPoint floatPosition() const;

    /**
     * @brief Analyse la partie réelle de la position.
     * @return true si la partie réelle est nulle, sinon false.
     */
    bool isFloatPositionNull() const;

    /**
     * @brief Augmente la coordonnée X réelle.
     * @param[in] pace Pas d'augmentation de la partie réelle (en X).
     * @details Augmente de pace la partie réelle selon X.
     * \n Si la partie réelle excède MAX_FLOAT_POS alors elle devient nulle et la partie entière est augmentée de 1.
     * \n De même, si la partie réelle devient inférieure à 0, la partie entière diminue de 1 et la partie réelle
     *  est mise à MAX_FLOAT_POS moins le pas.
     * \n Si la partie réelle s'approche de 0, elle devient alors nulle.
     */
    void increaseX(int pace);

    /**
     * @brief Augmente la coordonnée Y réelle.
     * @param[in] pace Pas d'augmentation de la partie réelle (en Y).
     * @details Augmente de pace la partie réelle selon Y.
     * \n Si la partie réelle excède MAX_FLOAT_POS alors elle devient nulle et la partie entière est augmentée de 1.
     * \n De même, si la partie réelle devient inférieure à 0, la partie entière diminue de 1 et la partie réelle
     *  est mise à MAX_FLOAT_POS moins le pas.
     * \n Si la partie réelle s'approche de 0, elle devient alors nulle.
     */
    void increaseY(int pace);

    /**
     * @brief Setter (m_integerPosition).
     * @param integerPosition Nouvelle position entière
     */
    void setIntegerPosition(const QPoint &integerPosition);

    /**
     * @brief Setter (X de la partie entière)
     * @param x Nouveau X de la partière entière
     */
    void setIntegerPositionX(int x);

    /**
     * @brief Setter (Y de la partie entière)
     * @param y Nouveau Y de la partière entière
     */
    void setIntegerPositionY(int y);

    /**
     * @brief Setter (m_floatPosition).
     * @param floatPosition Nouvelle position (après la virgule)
     */
    void setFloatPosition(const QPoint &floatPosition);

    /**
     * @brief Setter (X de la partie réelle)
     * @param x Nouveau X de la partière réelle
     */
    void setFloatPositionX(int x);

    /**
     * @brief Setter (Y de la partie réelle)
     * @param y Nouveau Y de la partière réelle
     * @return
     */
    void setFloatPositionY(int y);

    /**
     * @brief Calcule la distance euclidienne d'une position à l'autre.
     * @param position Position à partir de laquelle calculer la distance.
     * @return Distance euclidienne entre les deux positions (en double).
     * @details ((ix1.fx1 - ix2.fx2)^2 + (iy1.fy1 - iy2.fy2)^2)^(1/2)
     */
    qreal euclidianDistanceTo(DoublePosition const &position);

private:
    /** @brief Partie entière de la position. */
    QPoint m_integerPosition;
    /** @brief Partie réelle de la position. */
    QPoint m_floatPosition;
};

/**
 * @brief Surchage de l'opérateur == .
 * @param a Premier objet à comparer.
 * @param b Second objet à comparer.
 * @return true s'il y a moins de 50 de différence dans la valeur flottante sinon false.
 */
bool operator==(DoublePosition const &a, DoublePosition const& b);

/**
 * @brief Surchage de l'opérateur != .
 * @param a Premier objet à comparer.
 * @param b Second objet à comparer.
 * @return true s'il y a plus de 50 de différence dans la valeur flottante sinon false.
 */
bool operator!=(DoublePosition const &a, DoublePosition const& b);

#endif // DOUBLEPOSITION_H

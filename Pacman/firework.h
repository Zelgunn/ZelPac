#ifndef FIREWORK_H
#define FIREWORK_H

#include <QLineF>
#include <QTime>
#include <QPainter>
#include <QtCore/qmath.h>
#include <QRadialGradient>

/**
 * @brief Classe d'animation de feu d'artifices.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Cette classe s'instancie avec la taille maximale de la destination
 *  (par exemple, la taille de l'écran) et génère automatiquement une image.
 *  L'image générée ne prend pas en compte la position de l'objet (afin de placer cette image librement)
 *  et fait la taille maximale que fera l'animation.
 *  L'image générée est modifiée à chaque fois que la fonction est appelée afin de calculer la prochaine
 *  image de l'animation.
 *
 *  L'animation en elle même affiche une dispersion de lignes qui simulent un feu d'artifice. Les lignes peuvent prendre plusieurs
 *  couleurs aléatoires et s'estompent au fil du temps.
 *  La façon dont les lignes s'estompent simule un effet de deccélaration soudaine (comme dans le cas d'une explosion).
 */

class Firework
{
public:
    /**
     * @brief Constructeur de base de la classe Firework.
     * @param width Largeur de la destination qui affichera l'animation.
     * @param height Hauteur de la destination qui affichera l'animation.
     * @details Lors de la construction de l'objet, les différents paramètres aléatoire de la classe (tels
     *  que la taille de l'animation, les lignes qui la compose, leur longueur) sont automatiquement
     *  réglés (à une valeur aléatoire). Cependant, la classe ne modifie pas la graine aléatore utilisée.
     *  Afin de réinitialiser les paramètres de façon aléatoire, voire la fonction init();
     */
    Firework(int width, int height);

    /**
     * @brief Réinitialise les paramètres aléatoires de l'objet.
     */
    void init();

    /**
     * @brief Calcule et renvoie la prochaine image de l'animation.
     * @return Une frame de l'animation (dans l'ordre).
     */
    QPixmap paintFirework();

    /**
     * @brief Getter
     * @return Centre de l'animation.
     * @details Généré aléatoirement au sein de la taille donnée lors de la construction de l'objet.
     */
    QPoint center() const;

private:
    /** @brief Largeur de la destination */
    int m_width;
    /** @brief Hauteur de la destination */
    int m_height;

    /** @brief Centre dans la destination (aléatoire). */
    QPoint m_center;

    /** @brief Angles des lignes de l'animation (aléatoire). */
    QList<qreal> m_linesAngles;
    /** @brief Couleurs des lignes de l'animation (aléatoire). */
    QList<QColor> m_linesColor;

    /** @brief Carré de la longeur actuelle des lignes de l'animation (aléatoire, augmente). */
    qreal m_lenght;
    /** @brief Carré de la longueur maximale des lignes de l'animation (aléatoire). */
    qreal m_maxLenght;

    /** @brief Centre local de l'animation. (aléatoire selon m_lenght)*/
    QPoint m_localCenter;
    /** @brief Taille en pixel de l'animation. (aléatoire selon m_lenght) */
    int m_pixsize;
    /** @brief Indice d'estompage de l'animation. */
    qreal m_fading;
};

#endif // FIREWORK_H

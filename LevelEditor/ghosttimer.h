#ifndef GHOSTTIMER_H
#define GHOSTTIMER_H

#include <QDomElement>
#include <QList>

/**
 * @brief Classe de Timer pour les modes des fantômes.
 * @author Valentin D.d.G.
 * @version 1.1
 * @details Cette classe permet de suivre le mode de poursuite des fantômes en fonction
 * du temps écoulé dans la partie.
 *
 * Il est nécessaire d'appeler manuellement la fonction de mise à jour
 * du timer.
 */

class GhostTimer
{
public:
    enum ghostChasingModeID{Scatter,    /**< Les fantômes se dirigent vers leur coin spécifique */
                           Chase,       /**< Les fantômes utilisent leur IA personnelle pour attraper le Pacman */
                           Caught       /**< Les fantômes attrapés se dirigent vers leur maison et y reste jusqu'à expiration */
                           };

    /**
     * @brief Constructeur de base de la classe GhostTimer.
     * @details Note : Cette version du constructeur n'est pas destinée à être utilisée en dehors
     *      de l'initialisation d'une classe qui la contient. L'objet créé n'est pas valide.
     */
    GhostTimer();

    /**
     * @brief Surcharge du constructeur pour utiliser XML.
     * @param elem Données XML à charger lors de la contruction de l'objet.
     * @details L'élément XML doit ressembler au template suivant :
     * - \<GhostTimer>
     *      - \<Step time="7" mode="0"/>     // Mode Scatter (0) pendant 7 secondes.
     *      - \<Step time="20" mode="1"/>    // Puis Chase(1) pendant 20 secondes.
     *      - \<Step time="7" mode="0"/>     // Puis Scatter (0) jusqu'à la fin (time ignoré).
     * - \</GhostTimer>
     */
    GhostTimer(QDomElement elem);

    /**
     * @brief Getter
     * @return Mode de poursuite actuel du timer.
     */
    int currentMode() const;

    /**
     * @brief Augmente la valeur du timer de 16 mscs.
     * @details Si la valeur excède la prochaine limite de temps, le timer passe au prochain mode
     * et réinitialise sa valeur de temps.
     */
    void increaseTimer();

    /**
     * @brief Ajoute une étape au timer
     * @param duration Durée de l'étape.
     * @param mode Mode utilisé pendant la nouvelle étape.
     */
    void addStep(double duration, int mode);

    /**
     * @brief Getter
     * @return Temps entre chaque mode.
     */
    QList<double> stepsTime() const;

    /**
     * @brief Getter
     * @return Liste des modes du timer.
     */
    QList<int> ghostMode() const;

private:
    /** @brief Temps entre chaque mode. */
    QList<double> m_stepsTime;
    /** @brief Liste des modes du timer. */
    QList<int> m_ghostMode;
    /** @brief Valeur actuelle du timer. */
    double m_currentTimer;
    /** @brief Index actuel du timer. */
    int m_currentStep;
};

#endif // GHOSTTIMER_H

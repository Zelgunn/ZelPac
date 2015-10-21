#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QColor>
#include <QFileDialog>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QSpinBox>
#include <QCheckBox>
#include <QProcess>

#include <QDebug>

#include "newleveldialog.h"
#include "newghostmodedialog.h"
#include "textureselectdialog.h"
#include "map.h"
#include "editorgame.h"
#include "editorunit.h"

typedef struct unitWidgetSet_str
{
    QSpinBox *speed;
    QSpinBox *sspeed;
    QSpinBox *posx;
    QSpinBox *posy;
    QCheckBox *halfx;
    QCheckBox *halfy;
} UnitWidgetSet;

namespace Ui {
class MainWindow;
}

/**
 * @brief Classe principale graphique.
 * @author Valentin D.d.G.
 * @version 2.0
 * @details La classe MainWindow assure la gestion de l'ensemble des objets, de la connexion entre le modèle de
 * données et l'interface.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de base de la classe MainWindow.
     * @param parent Parent de la classe - Null par défaut.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destructeur de la classe MainWindow
     * @details Détruit l'interface qui est alloué dynamiquement.
     */
    ~MainWindow();

    /**
     * @brief Charge les icônes de collisions de l'UI.
     * @details Les icônes chargées sont créée automatiquement (ce ne sont que des carrés de couleur unie).
     */
    void loadManualIcons();
    /**
     * @brief Active les parties bloquées de l'interface.
     * @details Après le chargement ou la création d'un nouveau document, active l'interface (bloqué par sécurité).
     */
    void enableUI();

    /**
     * @brief Charge les données concernant les quatres fantômes et pacman.
     */
    void loadUnits();

    /**
     * @brief Getter
     * @return Pointeur vers le dernier niveau utilisé (à des fin de sauvegarde).
     */
    EditorLevel *previousLevel() const;

    /**
     * @brief Getter
     * @return Pointeur sur le niveau actuellement utilisé.
     */
    EditorLevel *currentLevel() const;

protected:
    /**
     * @brief Connecte le modèle de données des unités à leur équivalent dans l'UI.
     */
    void initUnitsWidgetSets();

    /**
     * @brief Active les signaux entre le modèle de données et leur équivalent dans l'UI.
     */
    void etablishUIConnections();

private slots:
    /**
     * @brief Evénement personnalisé déclenché lors d'un clic sur le niveau.
     * @param event Paramètre généré par l'événement.
     */
    void onMapClicked(QGraphicsSceneMouseEvent *event);

    /**
     * @brief Evénement personnalisé déclenché lors de l'utilisation de la molette de la souris.
     * @param event Paramètre généré par l'événement.
     */
    void onViewWheelEvent(QWheelEvent *event);

    /**
     * @brief Met à jour l'UI et le modèle de données quand un pion est déplacé manuellement.
     */
    void onUnitMoved();

    /**
     * @brief Met à jour le modèle de donnée et la partie dessin de l'UI quand les valeurs de l'UI sont modifiées.
     */
    void onUIValuesChanged();

    /**
     * @brief Spécialisation de onUIValuesChanged pour la vitesse des unités.
     */
    void onSpeedChanged();

    /**
     * @brief Met à jour le modèle de données duniveau à l'index donné selon les valeurs de l'UI.
     * @param idx Index du niveau mis à jour.
     */
    void updateUnitsFromUI(int idx);

    /**
     * @brief Affiche la fenêtre de création d'un nouveau niveau.
     */
    void on_actionNouveau_triggered();

    /**
     * @brief Affiche la texture sélectionné dans l'UI.
     * @param currentRow Index de la texture choisie.
     */
    void on_listWidget_currentRowChanged(int currentRow);

    /**
     * @brief Affiche la fenêtre de sauvegarde du document en cours de travail (et le sauvegarde).
     */
    void on_actionSauvegarder_sous_triggered();

    /**
     * @brief Affiche la fenêtre de sélection d'un document XML existant (et l'ouvre).
     */
    void on_actionOuvrir_triggered();

    /**
     * @brief Navigue entre des différents modes d'édition (Obsolète)
     * @param index Mode utilisé.
     */
    void on_toolBox_currentChanged(int index);

    /**
     * @brief Modifie le mode d'édition de collision en On.
     */
    void on_rb_collisionsOn_clicked();

    /**
     * @brief Modifie le mode d'édition de collision en Off.
     */
    void on_rb_CollisionsOff_clicked();

    /**
     * @brief Affiche le niveau à l'index currentRow lorsque la sélection dans la liste des niveaux change.
     * @param index Ligne actuellement sélectionnée dans la liste - donne l'index du niveau.
     */
    void on_levelsComboBox_currentIndexChanged(int index);

    /**
     * @brief Affiche la sélection d'une nouvelle texture et l'ajoute au gestionnaire de texture.
     */
    void on_addTextureButton_clicked();

    /**
     * @brief Affiche/Cache le filtre de texture selon les collisions.
     * @param checked Affiche (Vrai)/Cache (Faux)
     */
    void on_checkBox_showCollisions_toggled(bool checked);

    /**
     * @brief Affiche la fenêtre pour ajouter un nouveau timer (et l'ajoute).
     */
    void on_tb_newModeTimer_clicked();

    /**
     * @brief Affiche la fenêtre de sélection de la texture pour les cases vides.
     */
    void on_tb_emptyTile_clicked();

    /**
     * @brief Affiche la fenêtre de sélection de la texture pour les cases avec pastille.
     */
    void on_tb_pelletTile_clicked();

    /**
     * @brief Affiche la fenêtre de sélection de la texture pour les cases avec énergisant.
     */
    void on_tb_energizerTile_clicked();

    /**
     * @brief Modifie la durée pendant laquelle les fantômes sont appeurés.
     * @param arg1 Nouvelle durée.
     */
    void on_dsb_scaredTime_valueChanged(double arg1);

    /**
     * @brief Démarre le programme Pacman avec le document actuel (et le sauvegarde).
     */
    void on_actionLancer_dans_Pacman_triggered();

private:
    /** @brief Fichier d'interface. */
    Ui::MainWindow *ui;
    /** @brief Modèle de données. */
    EditorGame *m_game;
    /** @brief Elément graphique reflettant l'état du niveau. */
    Map *m_map;
    /** @brief Modèles de données des unités (pour les connexion). */
    EditorUnit **m_units;
    /** @brief Widgets connectés aux modèles de données des unités. */
    UnitWidgetSet *m_unitsWidgetSets;
    /** @brief Flag permettant d'ignorer temporairement les changements, accélérant les chargements. */
    bool m_ignoreChanges;
    /** @brief Index du dernier niveau utilisé. */
    int m_previousLevelIndex;
    /** @brief Flag vérifiant si le niveau charge (pour le chargement de multiples niveaux). */
    bool m_loading;
    /** @brief Chemin du document actuel. */
    QString m_filename;
};

#endif // MAINWINDOW_H

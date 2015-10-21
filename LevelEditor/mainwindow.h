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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadManualIcons();
    void enableUI();
    void loadUnits();

    EditorLevel *previousLevel() const;
    EditorLevel *currentLevel() const;

protected:
    void initUnitsWidgetSets();
    void etablishUIConnections();

private slots:
    void onMapClicked(QGraphicsSceneMouseEvent *event);
    void onViewWheelEvent(QWheelEvent *event);
    void onUnitMoved();
    void onUIValuesChanged();
    void onSpeedChanged();
    void updateUnitsFromUI(int idx);

    void on_actionNouveau_triggered();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_actionSauvegarder_sous_triggered();
    void on_actionOuvrir_triggered();
    void on_toolBox_currentChanged(int index);
    void on_rb_collisionsOn_clicked();
    void on_rb_CollisionsOff_clicked();
    void on_levelsComboBox_currentIndexChanged(int index);
    void on_addTextureButton_clicked();
    void on_checkBox_showCollisions_toggled(bool checked);

    void on_tb_newModeTimer_clicked();
    void on_tb_emptyTile_clicked();
    void on_tb_pelletTile_clicked();
    void on_tb_energizerTile_clicked();

    void on_dsb_scaredTime_valueChanged(double arg1);

    void on_actionLancer_dans_Pacman_triggered();

private:
    Ui::MainWindow *ui;
    EditorGame *m_game;
    Map *m_map;
    EditorUnit **m_units;
    UnitWidgetSet *m_unitsWidgetSets;
    bool m_ignoreChanges;
    int m_previousLevelIndex;
    bool m_loading;
    QString m_filename;

    //QString gameFilename;
};

#endif // MAINWINDOW_H

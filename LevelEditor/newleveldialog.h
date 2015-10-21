#ifndef NEWLEVELDIALOG_H
#define NEWLEVELDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class NewLevelDialog;
}

/**
 * @brief Classe graphique de fenêtre pour l'ajout d'un nouveau niveaau.
 * @author Valentin D.d.G.
 * @version 1.0
 */
class NewLevelDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de base de NewLevelDialog
     * @param previousGridAvailable Si vrai, active les champs de récupération de la grille précédente.
     * @param parent Parent de la fenêtre.
     */
    explicit NewLevelDialog(bool previousGridAvailable, QWidget *parent = 0);

    /**
     *
     */
    ~NewLevelDialog();

    QSize gridSize() const;
    int tileCountWidth() const;
    int tileCountHeight() const;
    int tileSize() const;
    QString levelName() const;
    bool usePreviousGrid() const;

private slots:
    void on_levelNameLineEdit_textChanged(const QString &);

private:
    Ui::NewLevelDialog *ui;
};

#endif // NEWLEVELDIALOG_H

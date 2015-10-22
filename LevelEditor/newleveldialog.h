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
     * Destructeur de NewLevelDialog
     */
    ~NewLevelDialog();

    /**
     * @brief Getter
     * @return Taille de la grille (nombre de case).
     */
    QSize gridSize() const;

    /**
     * @brief Getter
     * @return Nombre de case en largeur.
     */
    int tileCountWidth() const;

    /**
     * @brief Getter
     * @return Nombre de case en hauteur.
     */
    int tileCountHeight() const;

    /**
     * @brief Getter
     * @return Taille d'une case (en pixels).
     */
    int tileSize() const;

    /**
     * @brief Getter
     * @return Nom du niveau.
     */
    QString levelName() const;

    /**
     * @brief Getter
     * @return Vrai si l'option "Utiliser grille précédente" a été cochée, Faux sinon.
     */
    bool usePreviousGrid() const;

private slots:

    /**
     * @brief Bloque le reste de l'UI si le nom du niveau n'est pas renseigné.
     */
    void on_levelNameLineEdit_textChanged(const QString &);

private:

    /**
     * @brief Fichier d'interface de la fenêtre.
     */
    Ui::NewLevelDialog *ui;
};

#endif // NEWLEVELDIALOG_H

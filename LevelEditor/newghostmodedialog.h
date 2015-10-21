#ifndef NEWGHOSTMODEDIALOG_H
#define NEWGHOSTMODEDIALOG_H

#include <QDialog>

namespace Ui {
class NewGhostModeDialog;
}

/**
 * @brief Classe graphique de fenêtre pour l'ajout d'un nouveau mode de poursuite.
 * @author Valentin D.d.G.
 * @version 1.0
 */
class NewGhostModeDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de base de NewGhostModeDialog.
     * @param parent Parent de la fenêtre.
     */
    explicit NewGhostModeDialog(QWidget *parent = 0);

    /**
     * @brief Destructeur de NewGhostModeDialog.
     */
    ~NewGhostModeDialog();

    /**
     * @brief Getter
     * @return Durée où le mode est actif.
     */
    int duration() const;

    /**
     * @brief Getter
     * @return Type de mode actif.
     */
    int mode() const;

private:
    /** @brief Fichier d'interface de fenêtre. */
    Ui::NewGhostModeDialog *ui;
};

#endif // NEWGHOSTMODEDIALOG_H

#ifndef TEXTURESELECTDIALOG_H
#define TEXTURESELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "texturehandler.h"

namespace Ui {
class TextureSelectDialog;
}

/**
 * @brief Classe de fenêtre de sélection de texture.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details En fonction du gestionnaire de texture passé en paramètre,
 *  la fenêtre affiche les textures que le gestionnaire contient et les affiche en forme de liste.
 *  Lorsque la fenêtre se ferme, il est possible de récupérer l'index de la dernière texture sélectionnée
 *  via la fonction selectedTextureIndex.
 */
class TextureSelectDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de base de la fenêtre TextureSelectDialog.
     * @param textureHandler Gestionnaire de texture que la fenêtre liste.
     * @param parent Parent de la fenêtre.
     */
    explicit TextureSelectDialog(TextureHandler *textureHandler, QWidget *parent = 0);

    /**
     * Destructeur de TextureSelectDialog.
     */
    ~TextureSelectDialog();

    /**
     * @brief Getter
     * @return Index de la dernière texture sélectionnée.
     */
    int selectedTextureIndex() const;

private slots:
    /**
     * @brief Met à jour la prévisualisation de la texture choisie.
     * @param currentRow Index de la texture.
     */
    void on_listWidget_currentRowChanged(int currentRow);

    /**
     * @brief Ferme la fenêtre sur un double clic sur une des textures (sélection rapide).
     */
    void on_listWidget_itemDoubleClicked(QListWidgetItem *);

private:

    /**
     * @brief Fichier d'interface de la fenêtre.
     */
    Ui::TextureSelectDialog *ui;

    /**
     * @brief Gestionnaire de textures.
     */
    TextureHandler *m_textureHandler;
};

#endif // TEXTURESELECTDIALOG_H

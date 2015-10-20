#ifndef TEXTURESELECTDIALOG_H
#define TEXTURESELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "texturehandler.h"

namespace Ui {
class TextureSelectDialog;
}

class TextureSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextureSelectDialog(TextureHandler *textureHandler, QWidget *parent = 0);
    ~TextureSelectDialog();
    int selectedTextureIndex() const;

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *);

private:
    Ui::TextureSelectDialog *ui;
    TextureHandler *m_textureHandler;
};

#endif // TEXTURESELECTDIALOG_H

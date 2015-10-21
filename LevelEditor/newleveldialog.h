#ifndef NEWLEVELDIALOG_H
#define NEWLEVELDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class NewLevelDialog;
}

class NewLevelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewLevelDialog(bool previousGridAvailable, QWidget *parent = 0);
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

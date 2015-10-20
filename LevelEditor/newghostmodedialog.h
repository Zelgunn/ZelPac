#ifndef NEWGHOSTMODEDIALOG_H
#define NEWGHOSTMODEDIALOG_H

#include <QDialog>

namespace Ui {
class NewGhostModeDialog;
}

class NewGhostModeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGhostModeDialog(QWidget *parent = 0);
    ~NewGhostModeDialog();
    int duration() const;
    int mode() const;

private:
    Ui::NewGhostModeDialog *ui;
};

#endif // NEWGHOSTMODEDIALOG_H

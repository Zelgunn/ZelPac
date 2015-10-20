#include "newghostmodedialog.h"
#include "ui_newghostmodedialog.h"

NewGhostModeDialog::NewGhostModeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGhostModeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Nouveau timer");
}

NewGhostModeDialog::~NewGhostModeDialog()
{
    delete ui;
}

int NewGhostModeDialog::duration() const
{
    return ui->spinBox->value();
}

int NewGhostModeDialog::mode() const
{
    return ui->comboBox->currentIndex();
}

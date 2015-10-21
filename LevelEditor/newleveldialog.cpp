#include "newleveldialog.h"
#include "ui_newleveldialog.h"

NewLevelDialog::NewLevelDialog(bool previousGridAvailable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLevelDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->cb_usePreviousGrid->setEnabled(previousGridAvailable);
}

NewLevelDialog::~NewLevelDialog()
{
    delete ui;
}

QSize NewLevelDialog::gridSize() const
{
    return QSize(tileCountWidth(), tileCountHeight());
}

int NewLevelDialog::tileSize() const
{
    return ui->sb_tileSize->value();
}

int NewLevelDialog::tileCountWidth() const
{
    return ui->spin_tileCountWidth->value();
}

int NewLevelDialog::tileCountHeight() const
{
    return ui->spin_tileCountHeight->value();
}

QString NewLevelDialog::levelName() const
{
    return ui->levelNameLineEdit->text();
}

bool NewLevelDialog::usePreviousGrid() const
{
    return ui->cb_usePreviousGrid->isChecked();
}

void NewLevelDialog::on_levelNameLineEdit_textChanged(const QString &)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->levelNameLineEdit->text().isEmpty());
}

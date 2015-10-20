#include "textureselectdialog.h"
#include "ui_textureselectdialog.h"

TextureSelectDialog::TextureSelectDialog(TextureHandler *textureHandler, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextureSelectDialog)
{
    m_textureHandler = textureHandler;
    ui->setupUi(this);
    QString filename;
    for(int i=0; i<textureHandler->count(); i++)
    {
        filename = textureHandler->filenameAt(i);
        ui->listWidget->addItem(filename.section('/', -1));
        ui->listWidget->item(i)->setIcon(QIcon(filename));
        ui->listWidget->item(i)->setToolTip(filename);
    }

    ui->listWidget->setCurrentRow(0);
}

TextureSelectDialog::~TextureSelectDialog()
{
    delete ui;
}

int TextureSelectDialog::selectedTextureIndex() const
{
    return ui->listWidget->currentRow();
}

void TextureSelectDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->l_texturePreview->setPixmap(QPixmap::fromImage(m_textureHandler->textureAt(currentRow)));
}

void TextureSelectDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *)
{
    accept();
}

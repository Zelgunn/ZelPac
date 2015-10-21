#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Editeur de niveau v1.0");
    ui->graphicsView->setScene(new QGraphicsScene);
    m_map = new Map;
    ui->graphicsView->scene()->addItem(m_map);
    m_game = new EditorGame;

    ui->label_SelectedTexture->hide();
    loadManualIcons();

    m_units = (EditorUnit**)malloc(sizeof(EditorUnit*) * 5);
    m_unitsWidgetSets = (UnitWidgetSet*)malloc(sizeof(UnitWidgetSet) * 5);
    m_ignoreChanges = false;
    m_previousLevelIndex = -1;
    m_loading = false;
    m_filename = "tmp.xml";

    QObject::connect(ui->graphicsView, SIGNAL(wheelEventOnView(QWheelEvent*)), this, SLOT(onViewWheelEvent(QWheelEvent*)));
    QObject::connect(m_map, SIGNAL(mapClicked(QGraphicsSceneMouseEvent*)), this, SLOT(onMapClicked(QGraphicsSceneMouseEvent*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadManualIcons()
{
    QImage tmp(64,64, QImage::Format_ARGB32);
    tmp.fill(QColor(50,50,200));
    ui->label_CollisionOff->setPixmap(QPixmap::fromImage(tmp));

    tmp.fill(QColor(150,50,200));
    ui->label_CollisionOn->setPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::enableUI()
{
    ui->checkBox_showCollisions->setEnabled(true);
    ui->rb_CollisionsOff->setEnabled(true);
    ui->rb_collisionsOn->setEnabled(true);
    ui->tb_emptyTile->setEnabled(true);
    ui->tb_energizerTile->setEnabled(true);
    ui->tb_pelletTile->setEnabled(true);
    ui->tb_newModeTimer->setEnabled(true);
}

void MainWindow::loadUnits()
{
    if(m_game->levelsCount() == 0) return;

    int tileSize = currentLevel()->tileSize();
    if((m_game->levelsCount() == 1)||(m_loading))
    {
        QStringList imagesFilenames;
        imagesFilenames << ":/images/images/pacman_life.png" << ":/images/images/Ghost_Blinky.png"
                        << ":/images/images/Ghost_Pinky.png" << ":/images/images/Ghost_Inky.png"
                        << ":/images/images/Ghost_Clyde.png";
        QImage tmpImage;

        for(int i=0; i<5; i++)
        {
            tmpImage = QImage(imagesFilenames.at(i));
            tmpImage = tmpImage.scaled(QSize(tileSize, tileSize));
            m_units[i] = new EditorUnit(QPixmap::fromImage(tmpImage));
            ui->graphicsView->scene()->addItem(m_units[i]);
            QObject::connect(m_units[i], SIGNAL(unitMoved()), this, SLOT(onUnitMoved()));
        }
        etablishUIConnections();
    }

    EditorLevel *level = currentLevel();

    UnitWidgetSet unitWidgetSet;
    UnitData unitData;

    for(int i=0; i<5; i++)
    {
        unitData = level->unitsData()[i];
        unitWidgetSet = m_unitsWidgetSets[i];

        unitWidgetSet.halfx->setChecked(unitData.halfx);
        unitWidgetSet.halfy->setChecked(unitData.halfy);

        unitWidgetSet.posx->setMaximum(level->gridSize().width() - 1);
        unitWidgetSet.posy->setMaximum(level->gridSize().height() - 1);
        unitWidgetSet.posx->setValue(unitData.position.x());
        unitWidgetSet.posy->setValue(unitData.position.y());

        unitWidgetSet.speed->setValue(unitData.speed);
        unitWidgetSet.sspeed->setValue(unitData.sspeed);
    }
    m_ignoreChanges = false;
    onUIValuesChanged();
}

EditorLevel *MainWindow::previousLevel() const
{
    if(m_previousLevelIndex < 0)
        return Q_NULLPTR;
    return m_game->level(m_previousLevelIndex);
}

EditorLevel *MainWindow::currentLevel() const
{
    return m_game->level(ui->levelsComboBox->currentIndex());
}

void MainWindow::initUnitsWidgetSets()
{
    // Pacman
    m_unitsWidgetSets[EditorLevel::Pacman].speed = ui->sb_pacmanSpeed;
    m_unitsWidgetSets[EditorLevel::Pacman].sspeed = ui->sb_pacmanSSpeed;
    m_unitsWidgetSets[EditorLevel::Pacman].posx = ui->sb_pacmanx;
    m_unitsWidgetSets[EditorLevel::Pacman].posy = ui->sb_pacmany;
    m_unitsWidgetSets[EditorLevel::Pacman].halfx = ui->cb_pacmanx;
    m_unitsWidgetSets[EditorLevel::Pacman].halfy = ui->cb_pacmany;

    // Blinky
    m_unitsWidgetSets[EditorLevel::Blinky].speed = ui->sb_blinkySpeed;
    m_unitsWidgetSets[EditorLevel::Blinky].sspeed = ui->sb_blinkySSpeed;
    m_unitsWidgetSets[EditorLevel::Blinky].posx = ui->sb_blinkyx;
    m_unitsWidgetSets[EditorLevel::Blinky].posy = ui->sb_blinkyy;
    m_unitsWidgetSets[EditorLevel::Blinky].halfx = ui->cb_blinkyx;
    m_unitsWidgetSets[EditorLevel::Blinky].halfy = ui->cb_blinkyy;

    // Pinky
    m_unitsWidgetSets[EditorLevel::Pinky].speed = ui->sb_pinkySpeed;
    m_unitsWidgetSets[EditorLevel::Pinky].sspeed = ui->sb_pinkySSpeed;
    m_unitsWidgetSets[EditorLevel::Pinky].posx = ui->sb_pinkyx;
    m_unitsWidgetSets[EditorLevel::Pinky].posy = ui->sb_pinkyy;
    m_unitsWidgetSets[EditorLevel::Pinky].halfx = ui->cb_pinkyx;
    m_unitsWidgetSets[EditorLevel::Pinky].halfy = ui->cb_pinkyy;

    // Inky
    m_unitsWidgetSets[EditorLevel::Inky].speed = ui->sb_inkySpeed;
    m_unitsWidgetSets[EditorLevel::Inky].sspeed = ui->sb_inkySSpeed;
    m_unitsWidgetSets[EditorLevel::Inky].posx = ui->sb_inkyx;
    m_unitsWidgetSets[EditorLevel::Inky].posy = ui->sb_inkyy;
    m_unitsWidgetSets[EditorLevel::Inky].halfx = ui->cb_inkyx;
    m_unitsWidgetSets[EditorLevel::Inky].halfy = ui->cb_inkyy;

    // Clyde
    m_unitsWidgetSets[EditorLevel::Clyde].speed = ui->sb_clydeSpeed;
    m_unitsWidgetSets[EditorLevel::Clyde].sspeed = ui->sb_clydeSSpeed;
    m_unitsWidgetSets[EditorLevel::Clyde].posx = ui->sb_clydex;
    m_unitsWidgetSets[EditorLevel::Clyde].posy = ui->sb_clydey;
    m_unitsWidgetSets[EditorLevel::Clyde].halfx = ui->cb_clydex;
    m_unitsWidgetSets[EditorLevel::Clyde].halfy = ui->cb_clydey;

}

void MainWindow::etablishUIConnections()
{
    initUnitsWidgetSets();
    UnitWidgetSet unitWidgetSet;
    for(int i=0; i<5; i++)
    {
        unitWidgetSet = m_unitsWidgetSets[i];
        QObject::connect(unitWidgetSet.halfx, SIGNAL(clicked(bool)), this, SLOT(onUIValuesChanged()));
        QObject::connect(unitWidgetSet.halfy, SIGNAL(clicked(bool)), this, SLOT(onUIValuesChanged()));
        QObject::connect(unitWidgetSet.posx, SIGNAL(valueChanged(int)), this, SLOT(onUIValuesChanged()));
        QObject::connect(unitWidgetSet.posy, SIGNAL(valueChanged(int)), this, SLOT(onUIValuesChanged()));
        QObject::connect(unitWidgetSet.speed, SIGNAL(valueChanged(int)), this, SLOT(onSpeedChanged()));
        QObject::connect(unitWidgetSet.sspeed, SIGNAL(valueChanged(int)), this, SLOT(onSpeedChanged()));
    }
}

void MainWindow::onMapClicked(QGraphicsSceneMouseEvent* event)
{
    EditorLevel *level = m_game->level(ui->levelsComboBox->currentIndex());
    QPoint p = level->screenToTile(event->pos().toPoint());
    level->setTileTexture(p, ui->listWidget->currentRow());
    QImage image = level->updateBaseImage(p);
    m_map->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::onViewWheelEvent(QWheelEvent *event)
{
    int maxRow = m_game->textureCount();
    if(maxRow < 1)
        return;
    int nextRow = ui->listWidget->currentRow();

    if(event->delta()<0)
        nextRow = (nextRow + 1)%maxRow;
    else
    {
        nextRow = nextRow - 1;
        if(nextRow<0) nextRow = maxRow - 1;
    }

    ui->listWidget->setCurrentRow(nextRow);
}

void MainWindow::onUnitMoved()
{
    if((m_game->levelsCount() == 0)||(m_ignoreChanges))
        return;

    m_ignoreChanges = true;

    UnitWidgetSet unitWidgetSet;
    EditorUnit *unit;
    QPoint unitPosition;
    int tileSize = currentLevel()->tileSize();

    for(int i=0; i<5; i++)
    {
        unit = m_units[i];
        unitWidgetSet = m_unitsWidgetSets[i];

        unitPosition = unit->pos().toPoint();
        unitPosition.setX((unitPosition.x() + tileSize/2 - 1) / tileSize);
        unitPosition.setY((unitPosition.y() + tileSize/2 - 1) / tileSize);

        if(unitPosition.x() < 0) unitPosition.setX(0);
        if(unitPosition.y() < 0) unitPosition.setY(0);
        if(unitPosition.x() >= currentLevel()->gridSize().width()) unitPosition.setX(currentLevel()->gridSize().width() -1);
        if(unitPosition.y() >= currentLevel()->gridSize().height()) unitPosition.setY(currentLevel()->gridSize().height() -1);

        unitWidgetSet.posx->setValue(unitPosition.x());
        unitWidgetSet.posy->setValue(unitPosition.y());

        unitPosition.setX(unitPosition.x() * tileSize);
        unitPosition.setY(unitPosition.y() * tileSize);

        if(unitWidgetSet.halfx->isChecked())
            unitPosition.setX(unitPosition.x() + tileSize/2);

        if(unitWidgetSet.halfy->isChecked())
            unitPosition.setY(unitPosition.y() + tileSize/2);

        unit->setPos(unitPosition);
    }

    m_ignoreChanges = false;
}

void MainWindow::onUIValuesChanged()
{
    if((m_game->levelsCount() == 0)||(m_ignoreChanges))
        return;

    if(m_ignoreChanges) return;
    m_ignoreChanges = true;

    UnitWidgetSet unitWidgetSet;
    EditorUnit *unit;
    QPoint unitPosition;
    int tileSize = currentLevel()->tileSize();

    for(int i=0; i<5; i++)
    {
        unit = m_units[i];
        unitWidgetSet = m_unitsWidgetSets[i];

        unitPosition.setX(unitWidgetSet.posx->value() * tileSize);
        unitPosition.setY(unitWidgetSet.posy->value() * tileSize);

        if(unitWidgetSet.halfx->isChecked())
            unitPosition.setX(unitPosition.x() + tileSize/2);

        if(unitWidgetSet.halfy->isChecked())
            unitPosition.setY(unitPosition.y() + tileSize/2);

        unit->setPos(unitPosition);
    }

    m_ignoreChanges = false;
}

void MainWindow::onSpeedChanged()
{
    UnitData unitData;
    UnitWidgetSet unitWidgetSet;

    for(int i=0; i<5; i++)
    {
        unitData = currentLevel()->unitsData()[i];
        unitWidgetSet = m_unitsWidgetSets[i];

        unitData.speed = unitWidgetSet.speed->value();
        unitData.sspeed = unitWidgetSet.sspeed->value();
    }
}

void MainWindow::updateUnitsFromUI(int idx)
{
    if((m_game->levelsCount() == 0))
        return;

    EditorLevel *level = m_game->level(idx);

    UnitWidgetSet unitWidgetSet;
    UnitData unitData;

    for(int i=0; i<5; i++)
    {
        unitData = level->unitsData()[i];
        unitWidgetSet = m_unitsWidgetSets[i];

        unitData.halfx = unitWidgetSet.halfx->isChecked();
        unitData.halfy = unitWidgetSet.halfy->isChecked();

        unitData.position.setX(unitWidgetSet.posx->value());
        unitData.position.setY(unitWidgetSet.posy->value());

        unitData.speed = unitWidgetSet.speed->value();
        unitData.sspeed = unitWidgetSet.sspeed->value();

        level->setUnitsDataAt(unitData, i);
    }
}

void MainWindow::on_actionNouveau_triggered()
{
    NewLevelDialog dialog(m_game->levelsCount() != 0);
    EditorLevel *level;

    if(dialog.exec())
    {
        if(dialog.usePreviousGrid())
        {
            level = new EditorLevel(dialog.levelName(), currentLevel());
        }
        else
        {
            level = new EditorLevel(dialog.levelName(), dialog.gridSize(), dialog.tileSize(), m_game->textureHandler());
        }
        m_game->addLevel(level);
        ui->levelsComboBox->addItem(dialog.levelName());
        ui->levelsComboBox->setCurrentIndex(ui->levelsComboBox->count() - 1);
        enableUI();
    }
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->label_SelectedTexture->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(currentRow)));
}

void MainWindow::on_actionSauvegarder_sous_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Sauvegarder niveau"), "", tr("XML (*.xml)"));
    if(filename.isEmpty())
        return;

    m_filename = filename;
    updateUnitsFromUI(ui->levelsComboBox->currentIndex());
    m_game->save(filename);
}

void MainWindow::on_actionOuvrir_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Ouvrir niveau", "", tr("XML (*.xml)"));
    if(filename.isEmpty())
        return;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument dom;
    if(!dom.setContent(&file))
    {
        file.close();
        return;
    }

    delete m_game;
    m_loading = true;
    m_game = new EditorGame(dom);
    ui->levelsComboBox->clear();
    ui->listWidget->clear();

    TextureHandler* textureHandler = m_game->textureHandler();
    for(int i=0; i<textureHandler->count(); i++)
    {
        ui->listWidget->addItem(textureHandler->textureNameAt(i));
        ui->listWidget->item(i)->setIcon(QIcon(textureHandler->filenameAt(i)));
        ui->listWidget->item(i)->setToolTip(textureHandler->filenameAt(i));

    }

    for(int i=0; i<m_game->levelsCount(); i++)
    {
        ui->levelsComboBox->addItem(m_game->level(i)->name());
    }


    ui->label_SelectedTexture->show();
    ui->listWidget->setCurrentRow(0);
    file.close();
    m_loading = false;
    m_filename = filename;
    ui->actionLancer_dans_Pacman->setEnabled(true);
}

void MainWindow::on_toolBox_currentChanged(int index)
{
    Q_UNUSED(index);
    // TO DO : Mode Collision/Texture/Units
}

void MainWindow::on_rb_collisionsOn_clicked()
{

}

void MainWindow::on_rb_CollisionsOff_clicked()
{

}

void MainWindow::on_levelsComboBox_currentIndexChanged(int index)
{
    if(index >= m_game->levelsCount()) return;
    if(index < 0) return;
    if(m_previousLevelIndex >= 0)
        updateUnitsFromUI(m_previousLevelIndex);

    m_map->setPixmap(QPixmap::fromImage(m_game->level(index)->renderBaseImage()));

    loadUnits();
    m_previousLevelIndex = index;

    QList<double> durations = currentLevel()->ghostTimer().stepsTime();
    QList<int> modes = currentLevel()->ghostTimer().ghostMode();
    QString tmp;

    ui->lw_GhostModes->clear();
    for(int i=0; i<durations.size(); i++)
    {
        if(modes.at(i) == 0)
            tmp = "Poursuite";
        else
            tmp = "Dispersion";
        tmp.append(" \t Durée : ");
        tmp.append(QString::number(durations.at(i)));
        tmp.append("sec.");
        ui->lw_GhostModes->addItem(tmp);
    }
    ui->dsb_scaredTime->setValue(currentLevel()->scaredTime());
    //ui->sb

    ui->l_emptyTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(currentLevel()->emptyTileTexture())));
    ui->le_emptyTileFilename->setText(m_game->textureHandler()->textureNameAt(currentLevel()->emptyTileTexture()));

    ui->l_pelletTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(currentLevel()->pelletTileTexture())));
    ui->le_pelletTileFilename->setText(m_game->textureHandler()->textureNameAt(currentLevel()->pelletTileTexture()));

    ui->l_energizerTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(currentLevel()->energizerTileTexture())));
    ui->le_energizerTileFilename->setText(m_game->textureHandler()->textureNameAt(currentLevel()->energizerTileTexture()));
}

void MainWindow::on_addTextureButton_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Ouvrir texture..."), "", tr("Fichiers png (*.png);;Fichiers bmp(*.bmp);;Fichiers jpeg(*.jpg *.jpeg)"));
    if(filenames.isEmpty())
        return;

    QString filename;
    int c = ui->listWidget->count();
    for(int i=0; i<filenames.size(); i++)
    {
        filename = filenames.at(i);
        if(!m_game->textureHandler()->contains(filename))
        {
            m_game->textureHandler()->addTexture(filename);
            ui->listWidget->addItem(filename.section('/', -1));
            ui->listWidget->item(i + c)->setIcon(QIcon(filename));
            ui->listWidget->item(i + c)->setToolTip(filename);
        }
    }

    ui->label_SelectedTexture->show();
    ui->listWidget->setCurrentRow(0);
}

void MainWindow::on_checkBox_showCollisions_toggled(bool checked)
{
    if(m_game->levelsCount() == 0) return;
    Q_UNUSED(checked);
}

void MainWindow::on_tb_newModeTimer_clicked()
{
    NewGhostModeDialog dialog;
    if(dialog.exec())
    {
        QString tmp = "Mode : ";
        if(dialog.mode() == 0)
            tmp.append("Poursuite");
        else
            tmp.append("Dispersion");
        tmp.append(" \t Durée : ");
        tmp.append(QString::number(dialog.duration()));
        tmp.append("sec.");
        ui->lw_GhostModes->addItem(tmp);
        currentLevel()->appendGhostTimerStep(dialog.duration(), dialog.mode());
    }
}

void MainWindow::on_tb_emptyTile_clicked()
{
    TextureSelectDialog dialog(m_game->textureHandler());
    dialog.setWindowTitle("Sélection : Case vide");
    if(dialog.exec())
    {
        int selectedTextureIndex = dialog.selectedTextureIndex();
        ui->l_emptyTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(selectedTextureIndex)));
        ui->le_emptyTileFilename->setText(m_game->textureHandler()->textureNameAt(selectedTextureIndex));
        currentLevel()->setEmptyTileTexture(selectedTextureIndex);
    }
}

void MainWindow::on_tb_pelletTile_clicked()
{
    TextureSelectDialog dialog(m_game->textureHandler());
    dialog.setWindowTitle("Sélection : Pastille");
    if(dialog.exec())
    {
        int selectedTextureIndex = dialog.selectedTextureIndex();
        ui->l_pelletTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(selectedTextureIndex)));
        ui->le_pelletTileFilename->setText(m_game->textureHandler()->textureNameAt(selectedTextureIndex));
        currentLevel()->setPelletTileTexture(selectedTextureIndex);
    }
}

void MainWindow::on_tb_energizerTile_clicked()
{
    TextureSelectDialog dialog(m_game->textureHandler());
    dialog.setWindowTitle("Sélection : Energisant");
    if(dialog.exec())
    {
        int selectedTextureIndex = dialog.selectedTextureIndex();
        ui->l_energizerTilePreview->setPixmap(QPixmap::fromImage(m_game->textureHandler()->textureAt(selectedTextureIndex)));
        ui->le_energizerTileFilename->setText(m_game->textureHandler()->textureNameAt(selectedTextureIndex));
        currentLevel()->setEnergizerTileTexture(selectedTextureIndex);
    }
}

void MainWindow::on_dsb_scaredTime_valueChanged(double arg1)
{
    currentLevel()->setScaredTime(arg1);
}

void MainWindow::on_actionLancer_dans_Pacman_triggered()
{
    m_game->save(m_filename);
    QProcess::execute(QApplication::applicationDirPath() + "/Pacman.exe", QStringList(m_filename));
}

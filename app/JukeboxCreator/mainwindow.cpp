#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <vector>
#include <iostream>
#include <fstream>

#include "gtproject.h"
#include "gtsprite.h"
#include "gtscene.h"
#include "gtentityslot.h"
#include "romserializer.h"

#include "behaviormanagerdialog.h"

QString makeSlotLabel(int slotIndex, string slotName) {
    return QString("%1: %2").arg(QString::number(slotIndex), slotName.c_str());
}

MainWindow::MainWindow(GTProject& project, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loadedProject(project)
    , currentScene(loadedProject.scenes.front())
{
    ui->setupUi(this);

    graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(graphicsScene);

    QBrush bgRectBrush(QColor(63, 63, 63));
    graphicsScene->addRect(0, 0, 128*4, 128*4, QPen(), bgRectBrush);

    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::selectionChanged, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->ent_field_x, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_y, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_frame, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_hp, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_slot, &QComboBox::currentIndexChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_state, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);

    QObject::connect(ui->slot_field_sprite_combobox, &QComboBox::currentIndexChanged, this, &MainWindow::slot_properties_edited);
    QObject::connect(ui->slot_field_updater_combobox, &QComboBox::currentIndexChanged, this, &MainWindow::slot_added_behavior);
    QObject::connect(ui->slot_field_type, &QSpinBox::valueChanged, this, &MainWindow::slot_properties_edited);

    QObject::connect(ui->graphicsView->createAction, &QAction::triggered, this, &MainWindow::on_contextmenu_create);
    QObject::connect(ui->graphicsView->cloneAction, &QAction::triggered, this, &MainWindow::on_contextmenu_clone);
    QObject::connect(ui->graphicsView->deleteAction, &QAction::triggered, this, &MainWindow::on_contextmenu_delete);

    int spriteIndex = 0;
    int behaviorIndex = 0;
    int slotIndex = 0;
    for(auto sprite_itr = loadedProject.sprites.begin(); sprite_itr != loadedProject.sprites.end(); ++sprite_itr) {
        ui->slot_field_sprite_combobox->addItem(QString::fromStdString((*sprite_itr).name), QVariant::fromValue(spriteIndex));
        ++spriteIndex;
    }


    ui->slot_field_updater_combobox->addItem("Add a behavior...", QVariant::fromValue(-1));
    for(auto& behavior : loadedProject.behaviors) {
        ui->slot_field_updater_combobox->addItem(QString::fromStdString(behavior.name), QVariant::fromValue(behaviorIndex));
        ++behaviorIndex;
    }

    for(auto slot_itr = currentScene.entitySlots.begin(); slot_itr != currentScene.entitySlots.end(); ++slot_itr) {
        spriteIndex = 0;
        for(auto sprite_itr = loadedProject.sprites.begin(); sprite_itr != loadedProject.sprites.end(); ++sprite_itr) {
            if((*sprite_itr).name == (*slot_itr).sprite_name) {
                (*slot_itr).sprite_id = spriteIndex;
                break;
            }
            ++spriteIndex;
        }

        ui->ent_field_slot->addItem(makeSlotLabel(slotIndex, (*slot_itr).sprite_name), QVariant::fromValue(slotIndex));

        ++slotIndex;
    }

    int ent_index = 0;
    for(auto& ent : currentScene.entities) {
        AddEntityToView(ent, ent_index);
        ++ent_index;

    }

    ui->ent_field_slot->addItem("Add new...", QVariant::fromValue(-1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{
    loadedProject.Save();
}

void MainWindow::on_actionImport_Sprite_triggered()
{
    path spriteFileName = path(QFileDialog::getOpenFileName(this, tr("Open sprite image"), "~", tr("Bitmap images (*.bmp)")).toStdString());

    string spriteName = path(spriteFileName).filename().replace_extension("").string();

    path importedSpritePath = loadedProject.projectRoot / path("sprites") / spriteFileName.filename();
    filesystem::copy_file(spriteFileName, importedSpritePath);

    path framesFileName = path(spriteFileName).replace_extension("json");
    path importedFramesPath =loadedProject.projectRoot / path("sprites") / framesFileName.filename();
    if(!exists(framesFileName)) {
        framesFileName = path(QFileDialog::getOpenFileName(this, tr("Open framedata"), "~", tr("Aesprite JSON (*.json);;Game Sprite Info (*.gsi)")).toStdString());
        importedFramesPath =loadedProject.projectRoot / path("sprites") / framesFileName.filename();
    }

    if(exists(framesFileName)) {
        filesystem::copy_file(framesFileName, importedFramesPath);

        loadedProject.sprites.push_back(
            GTSprite(spriteName,
                     filesystem::proximate(path(importedSpritePath), loadedProject.projectRoot),
                      filesystem::proximate(path(importedFramesPath), loadedProject.projectRoot))
        );
    } else {
        loadedProject.sprites.push_back(
                    GTSprite(spriteName,
                             filesystem::proximate(path(importedSpritePath), loadedProject.projectRoot),
                              16));
    }
    loadedProject.sprites.back().InitImageData(loadedProject.projectRoot);

    QMessageBox::StandardButton slotReply;
    slotReply = QMessageBox::question(nullptr, "Sprite Imported", "Create a new slot with this sprite now?", QMessageBox::Yes | QMessageBox::No);

    if(slotReply == QMessageBox::Yes) {
        GTEntitySlot newSlot;
        newSlot.sprite_id = loadedProject.sprites.size() - 1;
        newSlot.sprite_name = spriteName;
        currentScene.entitySlots.push_back(newSlot);
        int newSlotIndex = currentScene.entitySlots.size()-1;
        ui->ent_field_slot->insertItem(newSlotIndex, makeSlotLabel(newSlotIndex, spriteName), QVariant::fromValue(newSlotIndex));
    }
    ui->slot_field_sprite_combobox->addItem(QString::fromStdString(spriteName), QVariant::fromValue(loadedProject.sprites.size() - 1));
}


void MainWindow::item_selection_changed()
{
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        ui->ent_field_x->setEnabled(true);
        ui->ent_field_y->setEnabled(true);
        ui->ent_field_hp->setEnabled(true);
        ui->ent_field_frame->setEnabled(true);
        ui->ent_field_slot->setEnabled(true);
        ui->ent_field_state->setEnabled(true);
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];
        QGraphicsItem *selection = ui->graphicsView->scene()->selectedItems()[0];
        selectedEnt.vx = selection->x() / 4;
        selectedEnt.vy = selection->y() / 4;
        selection->setX(selectedEnt.vx * 4);
        selection->setY(selectedEnt.vy * 4);

        ui->ent_field_x->blockSignals(true);
        ui->ent_field_y->blockSignals(true);
        ui->ent_field_hp->blockSignals(true);
        ui->ent_field_frame->blockSignals(true);
        ui->ent_field_slot->blockSignals(true);
        ui->ent_field_state->blockSignals(true);

        ui->slot_field_sprite_combobox->blockSignals(true);
        ui->slot_field_updater_combobox->blockSignals(true);
        ui->slot_field_type->blockSignals(true);

        ui->ent_field_frame->setMaximum(loadedProject.sprites[currentScene.entitySlots[selectedEnt.slot].sprite_id].frameCount());

        ui->ent_field_x->setValue(selectedEnt.vx);
        ui->ent_field_y->setValue(selectedEnt.vy);
        ui->ent_field_frame->setValue(selectedEnt.frame);
        ui->ent_field_slot->setCurrentIndex(selectedEnt.slot);
        ui->ent_field_hp->setValue(selectedEnt.hp);
        ui->ent_field_state->setValue(selectedEnt.state);

        ui->slot_field_sprite_combobox->setCurrentIndex(currentScene.entitySlots[selectedEnt.slot].sprite_id);
        ui->slot_field_behaviors->setModel(currentScene.entitySlots[selectedEnt.slot].behaviors);
        ui->slot_field_type->setValue(currentScene.entitySlots[selectedEnt.slot].type);

        ui->ent_field_x->blockSignals(false);
        ui->ent_field_y->blockSignals(false);
        ui->ent_field_hp->blockSignals(false);
        ui->ent_field_frame->blockSignals(false);
        ui->ent_field_slot->blockSignals(false);
        ui->ent_field_state->blockSignals(false);

        ui->slot_field_sprite_combobox->blockSignals(false);
        ui->slot_field_updater_combobox->blockSignals(false);
        ui->slot_field_type->blockSignals(false);
    } else {
        ui->ent_field_x->setEnabled(false);
        ui->ent_field_y->setEnabled(false);
        ui->ent_field_hp->setEnabled(false);
        ui->ent_field_frame->setEnabled(false);
        ui->ent_field_slot->setEnabled(false);
        ui->ent_field_state->setEnabled(false);
    }
}

void MainWindow::item_properties_edited()
{
    if(sender() == ui->graphicsView->scene()) {
        return;
    }
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];
        QGraphicsItem *selection = ui->graphicsView->scene()->selectedItems()[0];
        selectedEnt.vx = ui->ent_field_x->value();
        selectedEnt.vy = ui->ent_field_y->value();
        selectedEnt.frame = ui->ent_field_frame->value();
        selectedEnt.slot = ui->ent_field_slot->currentIndex();

        if(selectedEnt.slot == currentScene.entitySlots.size()) {
            GTEntitySlot eslot;
            eslot.sprite_id = 0;
            eslot.sprite_name = loadedProject.sprites[eslot.sprite_id].name;
            currentScene.entitySlots.push_back(eslot);
            ui->ent_field_slot->blockSignals(true);
            ui->ent_field_slot->insertItem(selectedEnt.slot, eslot.sprite_name.c_str(), QVariant::fromValue(selectedEnt.slot));
            ui->ent_field_slot->blockSignals(false);
        }


        selectedEnt.hp = ui->ent_field_hp->value();
        selectedEnt.state = ui->ent_field_state->value();

        UpdateEntityView(selectedEnt);
    }
}

void MainWindow::slot_properties_edited()
{
    if(sender() == ui->graphicsView->scene()) {
        return;
    }
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];
        int changedSlot = selectedEnt.slot;

        GTEntitySlot &selectedSlot = currentScene.entitySlots[changedSlot];
        selectedSlot.sprite_id = ui->slot_field_sprite_combobox->currentIndex();
        selectedSlot.sprite_name = loadedProject.sprites[selectedSlot.sprite_id].name;
        ui->ent_field_slot->setItemText(changedSlot, makeSlotLabel(changedSlot, currentScene.entitySlots[changedSlot].sprite_name));

        selectedSlot.type = ui->slot_field_type->value();

        for(auto& ent : currentScene.entities) {
            if(ent.slot == changedSlot) {
                UpdateEntityView(ent);
            }
        }
    }
}

void MainWindow::slot_added_behavior()
{
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int selectedBehavior = ui->slot_field_updater_combobox->currentIndex();
        if(selectedBehavior > 0) {
            int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
            GTEntity &selectedEnt = currentScene.entities[entIndex];
            int changedSlot = selectedEnt.slot;
            GTEntitySlot &selectedSlot = currentScene.entitySlots[changedSlot];
            selectedSlot.behaviors->push_back(&(loadedProject.behaviors[selectedBehavior - 1]));

            ui->slot_field_updater_combobox->setCurrentIndex(0);
        }
    }
}

void MainWindow::AddEntityToView(GTEntity &ent, int index)
{
    QGraphicsPixmapItem *pixItem = ui->graphicsView->scene()->addPixmap(loadedProject.sprites[currentScene.entitySlots[ent.slot].sprite_id] .getFrame(ent.frame));
    pixItem->setX(ent.vx * 4);
    pixItem->setY(ent.vy * 4);
    pixItem->setScale(4);
    pixItem->setFlag(QGraphicsItem::ItemIsSelectable);
    pixItem->setFlag(QGraphicsItem::ItemIsMovable);
    pixItem->setData(0, QVariant(index));
    const GTFrame& frameBox = loadedProject.sprites[currentScene.entitySlots[ent.slot].sprite_id].getFrameInfo(ent.frame);
    pixItem->setOffset(frameBox.x, frameBox.y);
    ent.pixItem = pixItem;
}

void MainWindow::UpdateEntityView(GTEntity &ent)
{
    ent.pixItem->setX(ent.vx * 4);
    ent.pixItem->setY(ent.vy * 4);

    ent.pixItem->setPixmap(loadedProject.sprites[currentScene.entitySlots[ent.slot].sprite_id].getFrame(ent.frame));
    const GTFrame& frameBox = loadedProject.sprites[currentScene.entitySlots[ent.slot].sprite_id].getFrameInfo(ent.frame);
    ent.pixItem->setOffset(frameBox.x, frameBox.y);
}

void MainWindow::on_pushButton_clicked()
{
    GTEntity ent;
    ent.vx = 32;
    ent.vy = 32;
    ent.frame = 0;
    ent.slot = 0;
    ent.state = 0;
    currentScene.entities.push_back(ent);

    AddEntityToView(currentScene.entities.back(), currentScene.entities.size() - 1);
}

void MainWindow::on_actionBuild_ROM_triggered()
{
    RomSerializer rs(loadedProject);
}

void MainWindow::on_contextmenu_create()
{
    QPointF newEntPoint = ui->graphicsView->mapToScene(ui->graphicsView->getContextMenuPos());
    GTEntity ent;
    ent.vx = newEntPoint.x() / 4;
    ent.vy = newEntPoint.y() / 4;
    ent.frame = 0;
    ent.slot = 0;
    ent.state = 0;
    currentScene.entities.push_back(ent);

    AddEntityToView(currentScene.entities.back(), currentScene.entities.size() - 1);
}

void MainWindow::on_contextmenu_clone()
{
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];

        GTEntity newEnt;
        newEnt.vx = selectedEnt.vx + 1;
        newEnt.vy = selectedEnt.vy + 1;
        newEnt.frame = selectedEnt.frame;
        newEnt.slot = selectedEnt.slot;
        newEnt.state = selectedEnt.slot;
        currentScene.entities.push_back(newEnt);

        AddEntityToView(currentScene.entities.back(), currentScene.entities.size()-1);
    }
}

void MainWindow::on_contextmenu_delete()
{
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];

        //TODO separate into removeByIndex function
        ui->graphicsView->scene()->removeItem(selectedEnt.pixItem);
        currentScene.entities.erase(currentScene.entities.begin()+entIndex);
        for(int i = 0; i < currentScene.entities.size(); ++i)
        {
            currentScene.entities[i].pixItem->setData(0, QVariant(i));
        }
    }
}

void MainWindow::on_actionManage_Behaviors_triggered()
{
    BehaviorManagerDialog bmd(loadedProject);
    bmd.exec();

    int behaviorIndex = 0;
    ui->slot_field_updater_combobox->blockSignals(true);
    ui->slot_field_updater_combobox->clear();
    ui->slot_field_updater_combobox->addItem("Add a behavior...", QVariant::fromValue(-1));
    for(auto& behavior : loadedProject.behaviors) {
        ui->slot_field_updater_combobox->addItem(QString::fromStdString(behavior.name), QVariant::fromValue(behaviorIndex));
        ++behaviorIndex;
    }
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = currentScene.entities[entIndex];
        ui->slot_field_behaviors->setModel(currentScene.entitySlots[selectedEnt.slot].behaviors);
    }
    ui->slot_field_updater_combobox->blockSignals(false);

}


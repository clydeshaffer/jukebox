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

    graphicsScene->addRect(0, 0, 128*4, 128*4);

    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::selectionChanged, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->ent_field_x, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_y, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_frame, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_hp, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_slot, &QComboBox::currentIndexChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_state, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);

    QObject::connect(ui->slot_field_sprite_combobox, &QComboBox::currentIndexChanged, this, &MainWindow::slot_properties_edited);
    //QObject::connect(ui->slot_field_updater_combobox, &QComboBox::currentIndexChanged, this, &MainWindow::slot_properties_edited);
    //QObject::connect(ui->slot_field_type, &QSpinBox::valueChanged, this, &MainWindow::slot_properties_edited);

    int spriteIndex = 0;
    int slotIndex = 0;
    for(auto sprite_itr = loadedProject.sprites.begin(); sprite_itr != loadedProject.sprites.end(); ++sprite_itr) {
        ui->slot_field_sprite_combobox->addItem(QString::fromStdString((*sprite_itr).name), QVariant::fromValue(spriteIndex));
        ++spriteIndex;
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
    filesystem::copy_file(framesFileName, importedFramesPath);

    loadedProject.sprites.push_back(
        GTSprite(spriteName,
                 filesystem::proximate(path(importedSpritePath), loadedProject.projectRoot),
                  filesystem::proximate(path(importedFramesPath), loadedProject.projectRoot))
    );
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

        ui->ent_field_x->blockSignals(true);
        ui->ent_field_y->blockSignals(true);
        ui->ent_field_hp->blockSignals(true);
        ui->ent_field_frame->blockSignals(true);
        ui->ent_field_slot->blockSignals(true);
        ui->ent_field_state->blockSignals(true);

        ui->ent_field_frame->setMaximum(loadedProject.sprites[currentScene.entitySlots[selectedEnt.slot].sprite_id].frameCount());

        ui->ent_field_x->setValue(selectedEnt.vx);
        ui->ent_field_y->setValue(selectedEnt.vy);
        ui->ent_field_frame->setValue(selectedEnt.frame);
        ui->ent_field_slot->setCurrentIndex(selectedEnt.slot);
        ui->ent_field_hp->setValue(selectedEnt.hp);
        ui->ent_field_state->setValue(selectedEnt.state);

        ui->slot_field_sprite_combobox->setCurrentIndex(currentScene.entitySlots[selectedEnt.slot].sprite_id);

        ui->ent_field_x->blockSignals(false);
        ui->ent_field_y->blockSignals(false);
        ui->ent_field_hp->blockSignals(false);
        ui->ent_field_frame->blockSignals(false);
        ui->ent_field_slot->blockSignals(false);
        ui->ent_field_state->blockSignals(false);
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

        for(auto& ent : currentScene.entities) {
            if(ent.slot == changedSlot) {
                UpdateEntityView(ent);
            }
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


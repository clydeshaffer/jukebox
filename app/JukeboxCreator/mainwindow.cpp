#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <vector>
#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sheetPixmap(new QPixmap("C:/Users/Clyde/Repos/fiend/assets/chara_hero.bmp"))
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    std::ifstream file("C:/Users/Clyde/Repos/fiend/build/assets/chara_hero.gsi", std::ios::binary);

    GTFrame frame;
    while (file.read(reinterpret_cast<char*>(&frame), sizeof(GTFrame))) {
        frames.push_back(frame);
        framePixmaps.push_back(sheetPixmap->copy(frame.gx, frame.gy, frame.width, frame.height));

    }

    GTEntity ent;
    ent.vx = 32;
    ent.vy = 32;
    ent.frame = 0;
    ent.slot = 0;
    ent.state = 0;
    entities.push_back(ent);

    QGraphicsPixmapItem *item = ui->graphicsView->scene()->addPixmap(framePixmaps.front());
    item->setX(ent.vx * 4);
    item->setY(ent.vy * 4);
    item->setScale(4);
    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setData(0, QVariant(0));
    item->setOffset(frames[0].x, frames[0].y);


    ent.vx = 64;
    ent.vy = 48;
    ent.frame = 0;
    ent.slot = 0;
    ent.state = 0;
    entities.push_back(ent);

    item = ui->graphicsView->scene()->addPixmap(framePixmaps.front());
    item->setX(ent.vx * 4);
    item->setY(ent.vy * 4);
    item->setScale(4);
    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setData(0, QVariant(1));
    item->setOffset(frames[0].x, frames[0].y);

    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::selectionChanged, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, &MainWindow::item_selection_changed);
    QObject::connect(ui->ent_field_x, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_y, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_frame, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_hp, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_slot, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
    QObject::connect(ui->ent_field_state, &QSpinBox::valueChanged, this, &MainWindow::item_properties_edited);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
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
        GTEntity &selectedEnt = entities[entIndex];
        QGraphicsItem *selection = ui->graphicsView->scene()->selectedItems()[0];
        selectedEnt.vx = selection->x() / 4;
        selectedEnt.vy = selection->y() / 4;

        ui->ent_field_x->blockSignals(true);
        ui->ent_field_y->blockSignals(true);
        ui->ent_field_hp->blockSignals(true);
        ui->ent_field_frame->blockSignals(true);
        ui->ent_field_slot->blockSignals(true);
        ui->ent_field_state->blockSignals(true);

        ui->ent_field_x->setValue(selectedEnt.vx);
        ui->ent_field_y->setValue(selectedEnt.vy);

        ui->ent_field_x->blockSignals(false);
        ui->ent_field_y->blockSignals(false);
        ui->ent_field_hp->blockSignals(false);
        ui->ent_field_frame->blockSignals(false);
        ui->ent_field_slot->blockSignals(false);
        ui->ent_field_state->blockSignals(false);

        ui->ent_field_frame->setValue(selectedEnt.frame);
        ui->ent_field_slot->setValue(selectedEnt.slot);
        ui->ent_field_hp->setValue(selectedEnt.hp);
        ui->ent_field_state->setValue(selectedEnt.state);
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
    qDebug() << sender();
    if(ui->graphicsView->scene()->selectedItems().length() == 1) {
        int entIndex = ui->graphicsView->scene()->selectedItems()[0]->data(0).toInt();
        GTEntity &selectedEnt = entities[entIndex];
        QGraphicsItem *selection = ui->graphicsView->scene()->selectedItems()[0];
        selectedEnt.vx = ui->ent_field_x->value();
        selectedEnt.vy = ui->ent_field_y->value();
        selectedEnt.frame = ui->ent_field_frame->value();
        selectedEnt.slot = ui->ent_field_slot->value();
        selectedEnt.hp = ui->ent_field_hp->value();
        selectedEnt.state = ui->ent_field_state->value();
        selection->setX(selectedEnt.vx * 4);
        selection->setY(selectedEnt.vy * 4);

        if(sender() == ui->ent_field_frame) {
            QGraphicsPixmapItem *pixItem = (QGraphicsPixmapItem*) selection;
            pixItem->setPixmap(framePixmaps[selectedEnt.frame]);
            pixItem->setOffset(frames[selectedEnt.frame].x, frames[selectedEnt.frame].y);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    GTEntity ent;
    ent.vx = 32;
    ent.vy = 32;
    ent.frame = 0;
    ent.slot = 0;
    ent.state = 0;
    entities.push_back(ent);

    QGraphicsPixmapItem *item = ui->graphicsView->scene()->addPixmap(framePixmaps.front());
    item->setX(ent.vx * 4);
    item->setY(ent.vy * 4);
    item->setScale(4);
    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setData(0, QVariant(entities.size() - 1));
    item->setOffset(frames[0].x, frames[0].y);

}


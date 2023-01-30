#include "behaviormanagerdialog.h"
#include "ui_behaviormanagerdialog.h"
#include <QFileDialog>
#include "whereami.h"

BehaviorManagerDialog::BehaviorManagerDialog(GTProject& project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BehaviorManagerDialog),
    project(project)
{
    ui->setupUi(this);
    ui->listWidget->blockSignals(true);
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clear();
    ui->tableWidget->setEnabled(false);

    ui->tableWidget->setColumnCount(2);

    QStringList headers;
    headers << "Parameter" << "Value";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->listWidget->clear();
    for(auto& behavior : project.behaviors) {
        ui->listWidget->addItem(behavior.name.c_str());
    }
    ui->tableWidget->blockSignals(false);
    ui->listWidget->blockSignals(false);
}

BehaviorManagerDialog::~BehaviorManagerDialog()
{
    delete ui;
}

void BehaviorManagerDialog::on_listWidget_itemSelectionChanged()
{
    if(ui->listWidget->selectedItems().size() == 1) {
        int selectedIndex = ui->listWidget->currentIndex().row();
        GTBehavior& selectedBehavior = project.behaviors[selectedIndex];
        int paramCount = selectedBehavior.params.size();
        ui->tableWidget->setRowCount(paramCount+1);

        ui->tableWidget->blockSignals(true);

        QTableWidgetItem* nameCell = new QTableWidgetItem("Name");
        nameCell->setFlags(nameCell->flags() & ~Qt::ItemIsEditable);
        QTableWidgetItem* valueCell = new QTableWidgetItem(QString::fromStdString(selectedBehavior.name));
        ui->tableWidget->setItem(0, 0, nameCell);
        ui->tableWidget->setItem(0, 1, valueCell);
        int row = 1;
        for (const auto& item : selectedBehavior.params) {
            QTableWidgetItem* nameCell = new QTableWidgetItem(QString::fromStdString(item.first));
            nameCell->setFlags(nameCell->flags() & ~Qt::ItemIsEditable);
            QTableWidgetItem* valueCell = new QTableWidgetItem(QString::number(item.second));
            ui->tableWidget->setItem(row, 0, nameCell);
            ui->tableWidget->setItem(row, 1, valueCell);
            ++row;
        }

        ui->tableWidget->setEnabled(true);
        ui->tableWidget->blockSignals(false);

        ui->DuplicateBehaviorButton->setEnabled(true);
        ui->RemoveBehaviorButton->setEnabled(true);
    } else {
        ui->DuplicateBehaviorButton->setEnabled(false);
        ui->RemoveBehaviorButton->setEnabled(false);
    }
}

void BehaviorManagerDialog::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    int selectedBehaviorIndex = ui->listWidget->currentIndex().row();
    GTBehavior& selectedBehavior = project.behaviors[selectedBehaviorIndex];

    if(item->row() == 0) {
        selectedBehavior.name = item->data(0).toString().toStdString();
        ui->listWidget->blockSignals(true);
        ui->listWidget->item(selectedBehaviorIndex)->setText(item->data(0).toString());
        ui->listWidget->blockSignals(false);
    } else {
        string changedPropName = ui->tableWidget->item(item->row(), 0)->data(0).toString().toStdString();
        selectedBehavior.params[changedPropName] = std::stoi(item->data(0).toString().toStdString());
    }
}

void BehaviorManagerDialog::on_AddBehaviorButton_clicked()
{
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';
    path exe(exePath);

    QString pathQStr = QFileDialog::getOpenFileName(this, tr("Add Behavior"), (exe.parent_path() / path("kit/behaviors")).string().c_str(), tr("6502 assembly file (*.asm)"));
    if(pathQStr.isEmpty()) {
        return;
    }
    path behaviorFileName = path(pathQStr.toStdString());
    string defaultName = path(behaviorFileName).filename().replace_extension("").string();

    path importedBehaviorPath = project.projectRoot / path("behaviors") / behaviorFileName.filename();
    filesystem::copy_file(behaviorFileName, importedBehaviorPath);

    path relToProject = filesystem::proximate(path(importedBehaviorPath), project.projectRoot);
    project.behaviors.push_back(GTBehavior(defaultName, relToProject));
    ui->listWidget->addItem(project.behaviors.back().name.c_str());
}

void BehaviorManagerDialog::on_DuplicateBehaviorButton_clicked()
{
    int selectedBehaviorIndex = ui->listWidget->currentIndex().row();
    GTBehavior& selectedBehavior = project.behaviors[selectedBehaviorIndex];
    project.behaviors.push_back(GTBehavior(selectedBehavior));
    ui->listWidget->addItem(project.behaviors.back().name.c_str());
}


void BehaviorManagerDialog::on_RemoveBehaviorButton_clicked()
{
    int selectedBehaviorIndex = ui->listWidget->currentIndex().row();
    GTBehavior& selectedBehavior = project.behaviors[selectedBehaviorIndex];
    ui->listWidget->blockSignals(true);
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    for(auto& scene : project.scenes) {
        for(auto& slot : scene.entitySlots) {
            if(slot.behavior_id == selectedBehaviorIndex) {
                slot.behavior_id = -1;
                slot.behavior = nullptr;
            } else if(slot.behavior_id > selectedBehaviorIndex) {
                slot.behavior_id--;
                slot.behavior = &(project.behaviors[slot.behavior_id]);
            }
        }
    }
    project.behaviors.erase(project.behaviors.begin()+selectedBehaviorIndex);
    ui->listWidget->blockSignals(false);
}

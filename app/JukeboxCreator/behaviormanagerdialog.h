#ifndef BEHAVIORMANAGERDIALOG_H
#define BEHAVIORMANAGERDIALOG_H

#include <QDialog>
#include "gtproject.h"
#include <QTableWidgetItem>
#include <gtbehavior.h>

namespace Ui {
class BehaviorManagerDialog;
}

class BehaviorManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BehaviorManagerDialog(GTProject& project, QWidget *parent = nullptr);
    ~BehaviorManagerDialog();

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_AddBehaviorButton_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_DuplicateBehaviorButton_clicked();

    void on_RemoveBehaviorButton_clicked();

private:
    Ui::BehaviorManagerDialog *ui;
    GTProject& project;
};

#endif // BEHAVIORMANAGERDIALOG_H

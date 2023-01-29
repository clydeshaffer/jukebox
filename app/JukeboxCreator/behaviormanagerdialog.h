#ifndef BEHAVIORMANAGERDIALOG_H
#define BEHAVIORMANAGERDIALOG_H

#include <QDialog>
#include "gtproject.h"

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

private:
    Ui::BehaviorManagerDialog *ui;
    GTProject& project;
};

#endif // BEHAVIORMANAGERDIALOG_H

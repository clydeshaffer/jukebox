#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>

#include "gtproject.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(GTProject& project, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void item_selection_changed();
    void item_properties_edited();
    void slot_properties_edited();

    void on_pushButton_clicked();

    void on_actionSave_triggered();

    void on_actionImport_Sprite_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QGraphicsScene *graphicsScene;
    GTProject& loadedProject;
    GTScene& currentScene;
    void AddEntityToView(GTEntity &ent, int index);
    void UpdateEntityView(GTEntity &ent);
};
#endif // MAINWINDOW_H

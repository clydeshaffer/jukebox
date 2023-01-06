#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgraphicsscene.h"
#include <QMainWindow>

#pragma pack(push, r1, 1)
struct GTFrame {
    signed char x, y;
    unsigned char width, height, gx, gy, color, bank;
};

struct GTEntity {
    unsigned char vx, vy, frame, slot, hp, state;
};

#pragma pack(pop, r1);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void item_selection_changed();
    void item_properties_edited();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QGraphicsScene *scene;
    QPixmap *sheetPixmap;
    std::vector<GTFrame> frames;
    std::vector<QPixmap> framePixmaps;
    std::vector<GTEntity> entities;
};
#endif // MAINWINDOW_H

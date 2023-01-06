/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QListView *listView;
    QGraphicsView *graphicsView;
    QWidget *widget;
    QFormLayout *formLayout_2;
    QSpinBox *ent_field_x;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *ent_field_y;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_4;
    QSpinBox *ent_field_frame;
    QSpinBox *ent_field_slot;
    QSpinBox *ent_field_hp;
    QLabel *label_6;
    QSpinBox *ent_field_state;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_7;
    QSpinBox *slot_field_type;
    QLabel *label_8;
    QComboBox *slot_field_sprite_combobox;
    QLabel *label_9;
    QComboBox *slot_field_updater_combobox;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1103, 632);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        listView = new QListView(centralwidget);
        listView->setObjectName("listView");
        listView->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(listView, 0, 0, 1, 1);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setSceneRect(QRectF(0, 0, 200, 200));
        graphicsView->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(graphicsView, 0, 1, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setMaximumSize(QSize(256, 16777215));
        formLayout_2 = new QFormLayout(widget);
        formLayout_2->setObjectName("formLayout_2");
        ent_field_x = new QSpinBox(widget);
        ent_field_x->setObjectName("ent_field_x");
        ent_field_x->setMaximum(256);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, ent_field_x);

        label = new QLabel(widget);
        label->setObjectName("label");

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        ent_field_y = new QSpinBox(widget);
        ent_field_y->setObjectName("ent_field_y");
        ent_field_y->setMaximum(256);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, ent_field_y);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        label_5 = new QLabel(widget);
        label_5->setObjectName("label_5");

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_5);

        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_4);

        ent_field_frame = new QSpinBox(widget);
        ent_field_frame->setObjectName("ent_field_frame");

        formLayout_2->setWidget(2, QFormLayout::FieldRole, ent_field_frame);

        ent_field_slot = new QSpinBox(widget);
        ent_field_slot->setObjectName("ent_field_slot");

        formLayout_2->setWidget(3, QFormLayout::FieldRole, ent_field_slot);

        ent_field_hp = new QSpinBox(widget);
        ent_field_hp->setObjectName("ent_field_hp");

        formLayout_2->setWidget(4, QFormLayout::FieldRole, ent_field_hp);

        label_6 = new QLabel(widget);
        label_6->setObjectName("label_6");

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_6);

        ent_field_state = new QSpinBox(widget);
        ent_field_state->setObjectName("ent_field_state");

        formLayout_2->setWidget(5, QFormLayout::FieldRole, ent_field_state);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName("groupBox");
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName("formLayout");
        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_7);

        slot_field_type = new QSpinBox(groupBox);
        slot_field_type->setObjectName("slot_field_type");

        formLayout->setWidget(0, QFormLayout::FieldRole, slot_field_type);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_8);

        slot_field_sprite_combobox = new QComboBox(groupBox);
        slot_field_sprite_combobox->setObjectName("slot_field_sprite_combobox");

        formLayout->setWidget(1, QFormLayout::FieldRole, slot_field_sprite_combobox);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_9);

        slot_field_updater_combobox = new QComboBox(groupBox);
        slot_field_updater_combobox->setObjectName("slot_field_updater_combobox");

        formLayout->setWidget(2, QFormLayout::FieldRole, slot_field_updater_combobox);


        formLayout_2->setWidget(6, QFormLayout::SpanningRole, groupBox);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        formLayout_2->setWidget(7, QFormLayout::LabelRole, pushButton);


        gridLayout->addWidget(widget, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1103, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Jukebox Creator", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Frame", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "HP", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Slot", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "State", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Slot Properties (Shared)", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Sprite", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Updater", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

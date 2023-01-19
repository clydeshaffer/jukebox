#include "mainwindow.h"
#include "gtproject.h"
#include "gtscene.h"
#include "gtsprite.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "JukeboxCreator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    path projectDir;
    GTProject myProject;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "CREATE NEW PROJECT?", "YES = create, NO = load", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        //create
        path projectParentDir = path(QFileDialog::getExistingDirectory(nullptr, "Choose location for project", "~").toStdString());
        QString projectName = QInputDialog::getText(nullptr, "Creating project", "Project name:");
        projectDir = projectParentDir / path(projectName.toStdString());
        myProject.scenes.push_back(GTScene());
        myProject.name = projectName.toStdString();
        myProject.projectRoot = projectDir;

        filesystem::create_directory(projectDir);
        filesystem::create_directory(projectDir / path("sprites"));
        myProject.Save();
    } else {
        //load
        path mainProjectJson = path(QFileDialog::getOpenFileName(nullptr, "Open project", "~", "Main project file (*.json)").toStdString());
        projectDir = mainProjectJson.parent_path();
        myProject.projectRoot = projectDir;
        myProject.DeserializeFromFile(mainProjectJson.string());
    }

    MainWindow w(myProject, nullptr);
    w.show();
    return a.exec();
}

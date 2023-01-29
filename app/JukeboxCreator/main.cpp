#include "mainwindow.h"
#include "gtproject.h"
#include "splashdialog.h"
#include "whereami.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>


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

    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    char* exePath = (char*)malloc(length + 1);
    wai_getExecutablePath(exePath, length, nullptr);
    exePath[length] = '\0';

    EditorSession lastEditorSession;
    path sessionPath = path(exePath).parent_path() / path("session.json");
    if(exists(sessionPath)) {
        lastEditorSession.DeserializeFromFile(sessionPath.string());
    } else {
        lastEditorSession.SerializeToFile(sessionPath.string());
    }
    GTProject myProject;


    SplashDialog splashScreen(lastEditorSession, myProject, nullptr);
    if(splashScreen.exec() == QDialog::Accepted) {

        lastEditorSession.UpdateRecentProjects(myProject.projectRoot);
        lastEditorSession.SerializeToFile(sessionPath.string());

        MainWindow w(myProject, nullptr);

        w.show();
        return a.exec();
    } else {
        a.quit();
        return a.exec();
    }

}

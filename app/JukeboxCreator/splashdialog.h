#ifndef SPLASHDIALOG_H
#define SPLASHDIALOG_H

#include <QDialog>
#include <vector>
#include <filesystem>
#include "gtproject.h"
#include "editorsession.h"

#include <QListWidgetItem>

namespace Ui {
class SplashDialog;
}

class SplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplashDialog(EditorSession &editorSession, GTProject &project, QWidget *parent = nullptr);
    ~SplashDialog();

    EditorSession &editorSession;
    GTProject &myProject;

    std::filesystem::path projectDir;
    std::vector<std::filesystem::path> recentProjects;
private slots:
    void on_button_create_clicked();

    void on_button_browse_clicked();

    void on_button_quit_clicked();

    void on_recentProjectsList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SplashDialog *ui;

};

#endif // SPLASHDIALOG_H

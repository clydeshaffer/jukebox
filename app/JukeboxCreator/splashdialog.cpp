#include "splashdialog.h"
#include "ui_splashdialog.h"

#include <QFileDialog>
#include <QInputDialog>

SplashDialog::SplashDialog(EditorSession &editorSession, GTProject &project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialog),
    editorSession(editorSession),
    myProject(project)
{
    ui->setupUi(this);

    for(auto& recentPath : editorSession.recentProjectPaths) {
        ui->recentProjectsList->addItem(QString(recentPath.filename().string().c_str()));
    }
}

SplashDialog::~SplashDialog()
{
    delete ui;
}

void SplashDialog::on_button_create_clicked()
{
    //create
    path projectParentDir = path(QFileDialog::getExistingDirectory(nullptr, "Choose location for project", "~").toStdString());
    QString projectName = QInputDialog::getText(nullptr, "Creating project", "Project name:");
    projectDir = projectParentDir / path(projectName.toStdString());
    myProject.scenes.push_back(GTScene());
    myProject.name = projectName.toStdString();
    myProject.projectRoot = projectDir;

    filesystem::create_directory(projectDir);
    myProject.EnsureSubdirs();
    myProject.Save();
    accept();
}




void SplashDialog::on_button_browse_clicked()
{
    //load
    path mainProjectJson = path(QFileDialog::getOpenFileName(nullptr, "Open project", "~", "Main project file (*.json)").toStdString());
    projectDir = mainProjectJson.parent_path();
    myProject.projectRoot = projectDir;
    myProject.DeserializeFromFile(mainProjectJson.string());
    myProject.EnsureSubdirs();
    accept();
}


void SplashDialog::on_button_quit_clicked()
{
    reject();
}


void SplashDialog::on_recentProjectsList_itemDoubleClicked(QListWidgetItem *item)
{
    int selectedIndex = ui->recentProjectsList->selectionModel()->selectedIndexes()[0].row();
    path mainProjectJson = editorSession.recentProjectPaths[selectedIndex] / path("project.json");
    projectDir = mainProjectJson.parent_path();
    myProject.projectRoot = projectDir;
    myProject.DeserializeFromFile(mainProjectJson.string());
    myProject.EnsureSubdirs();
    accept();
}


void SplashDialog::on_SplashDialog_rejected()
{
}


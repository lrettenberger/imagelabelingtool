#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDockWidget>

#include <QDir>
#include <QFont>

class ImageCanvasWidget;
class ColorSelectionWidget;
class ProjectInformationWidget;
class ProjectInfoWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void openDirectory(QString source, QString dest, bool mode = false);

private:
    ImageCanvasWidget *mImageCanvasWidget;
    ColorSelectionWidget *mColorSelectionWidget;
    ProjectInfoWidget *mProjectInfoWidget;
    void initLayout();

    QMenu *mFileMenu;
    QAction *mCreateProjectAct;
    QAction *mLoadProjectAction;
    QAction *mSaveProjectAction;

    QMenu *mViewMenu;
    QAction *mOpenImageExtern;
    QAction *mOpenImageDir;
    QAction *mOpenLabelDir;

    void createActions();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void closeEvent (QCloseEvent *event) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void dragEnterEvent(QDragEnterEvent*) override;
    void dropEvent(QDropEvent*) override;

public slots:
    void createProject();
    void loadProject();
    void saveProject();

    void openImage();
    void openImageDir();
    void openLabelDir();

    void imageDirectoryChanged();

signals:
    void savePaths();
    void saveLabelColors();
    void leftKeyPressed();
    void rightKeyPressed();
    void numberKeyPressed(int num);
};

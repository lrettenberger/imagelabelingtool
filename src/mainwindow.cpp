#include "mainwindow.h"

#include "imagecanvaswidget.h"
#include "imagescrollwidget.h"
#include "colorselectionwidget.h"
#include "projectinfowidget.h"

#include <QApplication>
#include <QWheelEvent>
#include <QEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initLayout();
    setAcceptDrops(true);
}

void MainWindow::initLayout()
{
    // init widgets
    QDockWidget *dockWidget = new QDockWidget();
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    mColorSelectionWidget = new ColorSelectionWidget();
    mColorSelectionWidget->setFixedWidth(150);
    mImageCanvasWidget = new ImageCanvasWidget(mColorSelectionWidget);
    mColorSelectionWidget->setImageCanvasWidget(mImageCanvasWidget);

    mProjectInfoWidget = new ProjectInfoWidget();
    mProjectInfoWidget->setColorSelectionWidget(mColorSelectionWidget);
    mColorSelectionWidget->setProjectInfoWidget(mProjectInfoWidget);

    // create center layout
    QWidget *mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    mainWidget->setLayout(vBoxLayout);
    //vBoxLayout->addWidget(mFolderWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    vBoxLayout->addLayout(mainLayout);

    QVBoxLayout *imageLayout = new QVBoxLayout();
    mainLayout->addLayout(imageLayout);
    //mainLayout->addWidget(mColorSelectionWidget);
    dockWidget->setWidget(mColorSelectionWidget);

    ImageScrollWidget *imageScroll = new ImageScrollWidget();
    imageScroll->setAutoFillBackground(true);
    imageScroll->setWidgetResizable(true);
    imageScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    imageScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // layout to center image
    QWidget *scrollSizeHelperWidget = new QWidget();
    imageScroll->setWidget(scrollSizeHelperWidget);

    QVBoxLayout *imageVLayout = new QVBoxLayout();
    QHBoxLayout *imageHLayout = new QHBoxLayout();
    imageVLayout->addStretch();
    imageVLayout->addLayout(imageHLayout);
    imageVLayout->addStretch();
    imageHLayout->addStretch();
    imageHLayout->addWidget(mImageCanvasWidget);
    imageHLayout->addStretch();

    scrollSizeHelperWidget->setLayout(imageVLayout);

    imageLayout->addWidget(imageScroll);
    imageLayout->addWidget(mProjectInfoWidget);

    setWindowTitle(tr("Image Labeling Tool"));
    resize(800, 600);

    // create actions
    createActions();

    // init menu
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mCreateProjectAct);
    mFileMenu->addAction(mLoadProjectAction);
    mFileMenu->addAction(mSaveProjectAction);

    //fileMenu->addSeparator();
    //fileMenu->addAction(exitAct);

    mViewMenu = menuBar()->addMenu(tr("&View"));
    mViewMenu->addAction(mOpenImageExtern);
    mViewMenu->addAction(mOpenImageDir);
    mViewMenu->addAction(mOpenLabelDir);
}

void MainWindow::createActions()
{
    mCreateProjectAct = new QAction(tr("&Create Project"), this);
    mCreateProjectAct->setShortcuts(QKeySequence::New);
    mCreateProjectAct->setStatusTip(tr("Create a new project, also drag & drop of directories is supported"));
    connect(mCreateProjectAct, &QAction::triggered, this, &MainWindow::createProject);

    mLoadProjectAction = new QAction(tr("&Open Project"), this);
    mLoadProjectAction->setShortcuts(QKeySequence::Open);
    mLoadProjectAction->setStatusTip(tr("Open an existing project"));
    connect(mLoadProjectAction, &QAction::triggered, this, &MainWindow::loadProject);

    mSaveProjectAction = new QAction(tr("&Save Project"), this);
    mSaveProjectAction->setShortcuts(QKeySequence::Save);
    mSaveProjectAction->setStatusTip(tr("Save current project"));
    connect(mSaveProjectAction, &QAction::triggered, this, &MainWindow::saveProject);


    mOpenImageExtern = new QAction(tr("&Open Image Extern"), this);
    mOpenImageExtern->setStatusTip(tr("Open image in external viewer"));
    connect(mOpenImageExtern, &QAction::triggered, this, &MainWindow::openImage);

    mOpenImageDir = new QAction(tr("&Open Image Dir"), this);
    connect(mOpenImageDir, &QAction::triggered, this, &MainWindow::openImageDir);

    mOpenLabelDir = new QAction(tr("&Create Label Dir"), this);
    connect(mOpenLabelDir, &QAction::triggered, this, &MainWindow::openLabelDir);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(Qt::ControlModifier == QApplication::keyboardModifiers())
        return;

    int currentPenWidth = mImageCanvasWidget->penWidth();
    int newPenWidth = currentPenWidth;
    int numDegrees = event->angleDelta().y() / 8;

    int mul = 1;
    if(numDegrees < 0)
        mul = -1;

    // stepping of the pen size
    if(currentPenWidth < 10)
        newPenWidth += (1*mul);

    if(currentPenWidth >= 10 && currentPenWidth < 20)
        newPenWidth += (2*mul);

    if(currentPenWidth == 20){
        if(mul < 0)
            newPenWidth += (2*mul);
        if(mul > 0)
            newPenWidth += (5*mul);
    }

    if(currentPenWidth > 20 && currentPenWidth < 50)
        newPenWidth += (5*mul);

    if(currentPenWidth >= 50)
        newPenWidth += (10*mul);

    // pen size below one not allowed
    if(newPenWidth < 1)
        newPenWidth = 1;

    mColorSelectionWidget->setPenWidth(newPenWidth);
    mImageCanvasWidget->setPenWidth(newPenWidth);
    mImageCanvasWidget->update();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mColorSelectionWidget->saveProject();
    event->accept();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int keyNumber = -1;

    if(mColorSelectionWidget->mPenSizeEdit->hasFocus())
        return;

    switch(event->key()){
    case Qt::Key_Space:
        mImageCanvasWidget->mDrawLabel = !mImageCanvasWidget->mDrawLabel;
        break;
    case Qt::Key_Z:
        if(QApplication::keyboardModifiers() == Qt::ControlModifier)
            mImageCanvasWidget->undo();
        break;
    case Qt::Key_Right:
        mColorSelectionWidget->nextImage();
        event->accept();
        break;
    case Qt::Key_Left:
        mColorSelectionWidget->prevImage();
        event->accept();
        break;
    case Qt::Key_0:
        keyNumber = 0;
        event->accept();
        break;
    case Qt::Key_1:
        keyNumber = 1;
        event->accept();
        break;
    case Qt::Key_2:
        keyNumber = 2;
        event->accept();
        break;
    case Qt::Key_3:
        keyNumber = 3;
        event->accept();
        break;
    case Qt::Key_4:
        keyNumber = 4;
        event->accept();
        break;
    case Qt::Key_5:
        keyNumber = 5;
        event->accept();
        break;
    case Qt::Key_6:
        keyNumber = 6;
        event->accept();
        break;
    case Qt::Key_7:
        keyNumber = 7;
        event->accept();
        break;
    case Qt::Key_8:
        keyNumber = 8;
        event->accept();
        break;
    case Qt::Key_9:
        keyNumber = 9;
        event->accept();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

    if(keyNumber > -1)
        mColorSelectionWidget->colorChanged(keyNumber);

    mImageCanvasWidget->update();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::openDirectory(QString source, QString dest, bool mode)
{
    QFileInfo fileInfo(source);
    if(!fileInfo.exists()){
        qWarning("Dir or file to open does not exists.");
        return;
    }

    if(fileInfo.isFile()){
        mColorSelectionWidget->loadProject(source);
        return;
    }

    if(fileInfo.isDir()){
        QDir dir(source);
        QStringList files = dir.entryList({"*.json"}, QDir::Files);

        if(files.isEmpty()){
            mColorSelectionWidget->setProject(source, dest, mode);
        } else {
            mColorSelectionWidget->loadProject(dir.filePath(files.at(0)));
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString source = event->mimeData()->urls().first().toLocalFile();

    QFileInfo fileInfo(source);
    if(!fileInfo.exists()){
        qWarning("Dir or file to open does not exists.");
        return;
    }

    if(fileInfo.isFile()){
        mColorSelectionWidget->loadProject(source);
        return;
    }

    if(fileInfo.isDir()){
        QDir dir(source);
        QStringList files = dir.entryList({"*.json"}, QDir::Files);

        if(files.isEmpty()){
            mColorSelectionWidget->newProject(source);
        } else {
            mColorSelectionWidget->loadProject(dir.filePath(files.at(0)));
        }
    }
}

void MainWindow::createProject()
{
    mColorSelectionWidget->newProject();
}

void MainWindow::loadProject()
{
    mColorSelectionWidget->loadProject();
}

void MainWindow::saveProject()
{
    mColorSelectionWidget->saveProject();
}

void MainWindow::openImage()
{
    mProjectInfoWidget->openImage();
}

void MainWindow::openImageDir()
{
    mProjectInfoWidget->openImageDir();
}

void MainWindow::openLabelDir()
{
    mProjectInfoWidget->openOutputDir();
}

void MainWindow::imageDirectoryChanged()
{

}

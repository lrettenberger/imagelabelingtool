#include "projectinfowidget.h"

#include "colorselectionwidget.h"
#include <QProcessEnvironment>

ProjectInfoWidget::ProjectInfoWidget(QWidget *parent) : QWidget(parent)
{
    QBoxLayout *mainLayout = new QHBoxLayout();
    setLayout(mainLayout);

    mImageNameLbl = new QLabel("Image Name");
    mainLayout->addWidget(mImageNameLbl);

    mImageNumberLbl = new QLabel("0/0");
    mainLayout->addWidget(mImageNumberLbl);

    mOpenImageBtn = new QPushButton("Open Image");
    mainLayout->addWidget(mOpenImageBtn);
    connect(mOpenImageBtn, SIGNAL(clicked()), this, SLOT(openImage()));

    mOpenDirBtn = new QPushButton("Image Dir");
    mainLayout->addWidget(mOpenDirBtn);
    connect(mOpenDirBtn, SIGNAL(clicked()), this, SLOT(openImageDir()));

    mOpenOutputBtn = new QPushButton("Output Dir");
    mainLayout->addWidget(mOpenOutputBtn);
    connect(mOpenOutputBtn, SIGNAL(clicked()), this, SLOT(openOutputDir()));
}

void ProjectInfoWidget::setColorSelectionWidget(ColorSelectionWidget *w)
{
    mCSWidget = w;
}

void ProjectInfoWidget::setImageName(QString name, int currentId, int maxId)
{
    mImageNameLbl->setText(name);
    mImageNumberLbl->setText(QString::number(currentId) + "/" + QString::number(maxId));
}

void ProjectInfoWidget::openExtern(const QFileInfo fileInfo, bool inExplorer)
{
    QString os = QSysInfo::productType();
    if(QSysInfo::productType().contains("windows")){
        QStringList param;
        if (!fileInfo.isDir() && inExplorer)
            param += QLatin1String("/select,");
        param += QDir::toNativeSeparators(fileInfo.canonicalFilePath());
        QString command = "explorer.exe " + param.join(" ");
        QProcess::startDetached(command);
    }
}

void ProjectInfoWidget::openImageDir()
{
    QString dir = mCSWidget->getImageDir();
    QString file = mCSWidget->getCurrentImageName();
    if(dir.isEmpty() || file.isEmpty())
        return;

    openExtern(dir+ "/" + file);
}

void ProjectInfoWidget::openOutputDir()
{
    QString dir = mCSWidget->getOutputDir();
    if(dir.isEmpty())
        return;

    // search label image
    openExtern(QFileInfo(dir));
}

void ProjectInfoWidget::openImage()
{
    QString dir = mCSWidget->getImageDir();
    QString file = mCSWidget->getCurrentImageName();
    if(dir.isEmpty() || file.isEmpty())
        return;

    openExtern(dir+ "/" + file, false);
}

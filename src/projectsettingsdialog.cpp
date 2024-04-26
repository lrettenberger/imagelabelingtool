#include "projectsettingsdialog.h"
#include "colorselectionwidget.h"

#include <QDir>

ProjectSettingsDialog::ProjectSettingsDialog(ColorSelectionWidget* r, QString source) : mCSWidget(r),
    mImageDir(source), mOutputDir(source), mLabelMode(0)
{    
    setWindowTitle("Create New Project");
    setAttribute(Qt::WA_DeleteOnClose);

    mMainLayout = new QVBoxLayout();
    setLayout(mMainLayout);

    // select image dir
    mImageDirGroup = new QGroupBox("Image Directory");
    mMainLayout->addWidget(mImageDirGroup);
    mImageLayout = new QHBoxLayout();
    mImageDirGroup->setLayout(mImageLayout);

    mImageDirectoryLabel = new QLabel(source);
    mImageDirectoryLabel->setMinimumSize(300,10);
    mImageLayout->addWidget(mImageDirectoryLabel);
    mImageDirectoryBtn = new QPushButton("...");
    mImageDirectoryBtn->setMaximumSize(50,100);
    mImageLayout->addWidget(mImageDirectoryBtn);
    connect(mImageDirectoryBtn, SIGNAL(clicked()), this, SLOT(selectImageDir()));

    // select output dir
    mOutputDirGroup = new QGroupBox("Output Directory");
    mMainLayout->addWidget(mOutputDirGroup);
    mOutputMainLayout = new QVBoxLayout();
    mOutputDirGroup->setLayout(mOutputMainLayout);

    mCheckBox = new QCheckBox("Same As Image Directory");
    mCheckBox->setChecked(true);
    mOutputMainLayout->addWidget(mCheckBox);
    connect(mCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectOutputCheckbox(int)));

    mOutputLayout = new QHBoxLayout();
    mOutputMainLayout->addLayout(mOutputLayout);
    mOutputDirectoryLabel = new QLabel(source);
    mOutputDirectoryLabel->setMinimumSize(300,10);
    mOutputLayout->addWidget(mOutputDirectoryLabel);
    mOutputDirectoryBtn = new QPushButton("...");
    mOutputDirectoryBtn->setMaximumSize(50,100);
    mOutputLayout->addWidget(mOutputDirectoryBtn);
    connect(mOutputDirectoryBtn, SIGNAL(clicked()), this, SLOT(selectOutputDir()));

    // label mode
    mLabelModeGroup = new QGroupBox("Label Mode");
    mMainLayout->addWidget(mLabelModeGroup);
    mLabelModeLayout = new QHBoxLayout();
    mLabelModeGroup->setLayout(mLabelModeLayout);
    mLabelModeCheck = new QCheckBox("Overlapping Classes (Default: Off)");
    mLabelModeLayout->addWidget(mLabelModeCheck);
    connect(mLabelModeCheck, SIGNAL(stateChanged(int)), this, SLOT(changeLabelMode(int)));

    // create project
    mBtnLayout = new QHBoxLayout();
    mMainLayout->addLayout(mBtnLayout);
    mBtnLayout->addStretch();
    mOkBtn = new QPushButton("Create Project");
    mBtnLayout->addWidget(mOkBtn);
    connect(mOkBtn, SIGNAL(clicked()), this, SLOT(createProject()));
}

void ProjectSettingsDialog::selectImageDir()
{
    QString folder = ""; //QDir::home().path();
    if(!mImageDir.isEmpty())
        folder = mImageDir;

    mImageDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), folder, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    mImageDirectoryLabel->setText(mImageDir);

    if(mCheckBox->isChecked()){
        mOutputDir = mImageDir;
        mOutputDirectoryLabel->setText(mOutputDir);
    }
}

void ProjectSettingsDialog::selectOutputDir()
{
    mCheckBox->setChecked(false);

    QString folder = ""; //QDir::home().path();
    if(!mImageDir.isEmpty())
        folder = mImageDir;

    mOutputDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), folder, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    mOutputDirectoryLabel->setText(mOutputDir);
}

void ProjectSettingsDialog::selectOutputCheckbox(int b)
{
    if(b == 1){
        mOutputDir = mImageDir;
        mOutputDirectoryLabel->setText(mOutputDir);
    }
}

void ProjectSettingsDialog::changeLabelMode(int b)
{
    if(b == 0)
        mLabelMode = 0;
    else
        mLabelMode = 1;
}

void ProjectSettingsDialog::createProject()
{
    mCSWidget->setProject(mImageDir, mOutputDir, mLabelMode);
    close();
}




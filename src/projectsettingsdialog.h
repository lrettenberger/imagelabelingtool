#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFileDialog>

class ColorSelectionWidget;

class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    ProjectSettingsDialog(ColorSelectionWidget *r, QString source = "");
    ColorSelectionWidget *mCSWidget;

private:
    QBoxLayout *mMainLayout;
    QBoxLayout *mImageLayout;
    QBoxLayout *mOutputMainLayout;
    QBoxLayout *mCheckLayout;
    QBoxLayout *mOutputLayout;
    QBoxLayout *mLabelModeLayout;
    QBoxLayout *mBtnLayout;

    QGroupBox *mImageDirGroup;
    QLabel *mImageDirectoryLabel;
    QPushButton *mImageDirectoryBtn;

    QGroupBox *mOutputDirGroup;
    QCheckBox *mCheckBox;
    QLabel *mOutputDirectoryLabel;
    QPushButton *mOutputDirectoryBtn;

    QGroupBox *mLabelModeGroup;
    QCheckBox *mLabelModeCheck;

    QPushButton *mOkBtn;

    QString mImageDir;
    QString mOutputDir;
    bool mLabelMode;

private slots:
    void selectImageDir();
    void selectOutputDir();
    void selectOutputCheckbox(int);
    void changeLabelMode(int);
    void createProject();
};

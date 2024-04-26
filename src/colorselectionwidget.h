#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QDial>
#include <QPushButton>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QFileDialog>
#include <QFileSystemWatcher>

#include "labelcolor.h"
#include "projectinfowidget.h"
#include "projectsettingsdialog.h"
#include "nowheelslider.h"

class ImageCanvasWidget;

class ColorSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorSelectionWidget(QWidget *parent = nullptr);

    ImageCanvasWidget *mImageCanvasWidget;
    void setImageCanvasWidget(ImageCanvasWidget* widget);

    ProjectInfoWidget *mProjectInfoWidget;
    void setProjectInfoWidget(ProjectInfoWidget* widget);

    QPushButton *mNextImage;
    QPushButton *mPreviousImage;

    QPushButton *mUndoButton;

    QCheckBox *mContrastCheck;
    QSlider *mMinContrastSlider;
    QSlider *mMaxContrastSlider;

    QLineEdit *mPenSizeEdit;
    NoWheelSlider *mPenSizeWidget;
    QDial *mDial;

    void addColor(LabelColor *color);
    LabelColor *getLabelColor(int index);
    QString getName(int index);
    QColor getColor(int labelValue);

    QList<LabelColor*> mColorList;

    QString getCurrentImageName();
    QString getImageDir() {return mImageDir;}
    QString getOutputDir() {return  mOutputDir;}

private:
    int mAlpha;
    QVBoxLayout *mMainLayout;
    QVBoxLayout *mLabelColorBtnsLayout;
    QVBoxLayout *mConfigBtnsLayout;

    QColor getRandomColor();
    void clearLayout(QLayout *lyt);

    // image and directory management
    QString mImageDir;
    QString mOutputDir;
    QFileSystemWatcher mFileSystemWatcher;
    bool mLabelingMode;

    QStringList mFiles;
    int mCurrentIndex = 0;

    void loadImages();

private slots:
    void addButtonClicked();
    void fillSingleColor(QColor,int);

public slots:
    void saveToJson();
    void loadFromJson();
    void labelColorDeleted(int index);
    void colorChanged(int);
    void setPenWidth(int);
    void sliderPenWidthUpdate(int);
    void editPenWidthUpdate(QString);

    void newProject(QString source = "");
    void loadProject(QString path = "");
    void saveProject(bool saveImage = true);
    void imageDirectoryChanged(QString dir);

    void setProject(QString, QString, bool);
    void nextImage();
    void prevImage();

signals:
    void sortImage(int);
};

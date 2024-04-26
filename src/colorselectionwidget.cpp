#include "colorselectionwidget.h"

#include "imagecanvaswidget.h"
#include "projectsettingsdialog.h"
#include "projectinfowidget.h"

ColorSelectionWidget::ColorSelectionWidget(QWidget *parent) : QWidget(parent), mImageCanvasWidget(nullptr)
{
    mMainLayout = new QVBoxLayout();
    setLayout(mMainLayout);

    QBoxLayout *prevNextLayout = new QHBoxLayout;
    mMainLayout->addLayout(prevNextLayout);
    mPreviousImage = new QPushButton("<<");
    mPreviousImage->setMinimumHeight(40);
    prevNextLayout->addWidget(mPreviousImage);
    connect(mPreviousImage, SIGNAL(clicked()), this, SLOT(prevImage()));
    mNextImage = new QPushButton(">>");
    mNextImage->setMinimumHeight(40);
    prevNextLayout->addWidget(mNextImage);
    connect(mNextImage, SIGNAL(clicked()), this, SLOT(nextImage()));

    mUndoButton = new QPushButton("Undo");
    mUndoButton->setMinimumSize(30, 30);
    mMainLayout->addWidget(mUndoButton);

    // Pen settings
    QHBoxLayout *penSizeLayout = new QHBoxLayout();
    mPenSizeEdit = new QLineEdit("40");
    penSizeLayout->addWidget(new QLabel("Pen Size:"));
    penSizeLayout->addWidget(mPenSizeEdit);
    connect(mPenSizeEdit, SIGNAL(textChanged(QString)), this, SLOT(editPenWidthUpdate(QString)) );
    mMainLayout->addLayout(penSizeLayout);

    mPenSizeWidget = new NoWheelSlider(Qt::Horizontal);
    mPenSizeWidget->setMinimum(1);
    mPenSizeWidget->setMaximum(200);
    mPenSizeWidget->setValue(40);
    mMainLayout->addWidget(mPenSizeWidget);
    connect(mPenSizeWidget, SIGNAL(sliderMoved(int)), this, SLOT(sliderPenWidthUpdate(int)) );

    mDial = new QDial();
    mDial->setMinimum(1);
    mDial->setMaximum(200);
    mDial->setValue(40);
    mMainLayout->addWidget(mDial);
    connect(mDial, SIGNAL(valueChanged(int)), this, SLOT(sliderPenWidthUpdate(int)));

    //button layouts
    mLabelColorBtnsLayout = new QVBoxLayout();
    mConfigBtnsLayout = new QVBoxLayout();
    mMainLayout->addLayout(mLabelColorBtnsLayout );
    mMainLayout->addStretch(1);
    mMainLayout->addLayout(mConfigBtnsLayout);

    connect(&mFileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(imageDirectoryChanged(QString)));

    // last line of color gui
    /* stick to 9 colors for now
    mAddButton = new QPushButton("+");
    connect(mAddButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    mConfigBtnsLayout->addWidget(mAddButton);
    //*/

    mAlpha = 70;
    // init default colors
    addColor(new LabelColor(QColor(0,0,0,0),            0, 0, "Background"));
    addColor(new LabelColor(QColor(255,0,0,mAlpha),     1, 1, "C 1"));
    addColor(new LabelColor(QColor(0,255,0,mAlpha),     2, 2, "C 2"));
    addColor(new LabelColor(QColor(0,0,255,mAlpha),     3, 3, "C 3"));
    addColor(new LabelColor(QColor(255,255,0,mAlpha),   4, 4, "C 4"));
    addColor(new LabelColor(QColor(255,0,255,mAlpha),   5, 5, "C 5"));
    addColor(new LabelColor(QColor(0,255,255,mAlpha),   6, 6, "C 6"));
    addColor(new LabelColor(QColor(255,255,255,mAlpha), 7, 7, "C 7"));
    addColor(new LabelColor(QColor(244,164,96,mAlpha),  8, 8, "C 8"));
    addColor(new LabelColor(QColor(148,0,211,mAlpha),   9, 9, "C 9"));
}

void ColorSelectionWidget::setImageCanvasWidget(ImageCanvasWidget *widget)
{
    mImageCanvasWidget = widget;
    connect(mUndoButton, SIGNAL(clicked()), mImageCanvasWidget, SLOT(undo()));
}

void ColorSelectionWidget::setProjectInfoWidget(ProjectInfoWidget *widget)
{
    mProjectInfoWidget = widget;
}

void ColorSelectionWidget::addColor(LabelColor *color)
{
    mColorList.append(color);
    mLabelColorBtnsLayout->addWidget(color);

    connect(color, SIGNAL(colorClicked(int)), this, SLOT(colorChanged(int)));
    connect(color, SIGNAL(colorLabelClicked(QColor,int)), this, SLOT(fillSingleColor(QColor,int)));
    connect(color, SIGNAL(colorLabelDelete(int)), this, SLOT(labelColorDeleted(int)));
    connect(color, SIGNAL(sortImageBtnClicked(int)), this, SIGNAL(sortImage(int)));
}

LabelColor* ColorSelectionWidget::getLabelColor(int index)
{
    return mColorList.at(index);
}

QColor ColorSelectionWidget::getColor(int labelValue)
{
    for (int i = 0; i < mColorList.size(); i++ ) {
        if (mColorList.at(i)->mLabelValue == labelValue ) {
            return mColorList.at(i)->mColor;
        }
    }

    // label Value not found -> create new button with this label value and return the color
    // add transperant first
    if (mColorList.isEmpty()) {
        addButtonClicked();
    }
    addButtonClicked();
    mColorList.at(mColorList.size() - 1)->mLabelValue = labelValue;
    return mColorList.at(mColorList.size() - 1)->mColor;
}

QString ColorSelectionWidget::getCurrentImageName() {
    if(mFiles.isEmpty() || mCurrentIndex < 0 || mCurrentIndex >= mFiles.length()){
        return "";
    }
    return mFiles.at(mCurrentIndex);}

QColor ColorSelectionWidget::getRandomColor()
{
    int r = std::rand() % 255;
    int g = std::rand() % 255;
    int b = std::rand() % 255;
    return QColor(r,g,b,mAlpha);
}

void ColorSelectionWidget::clearLayout(QLayout *layout)
{
    if (!layout)
        return;
    while (auto item = layout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        if (item->layout()) {
            clearLayout(item->layout());
            delete item;
        }
    }
}

void ColorSelectionWidget::loadImages()
{
    mFiles.clear();
    QStringList files;

    QDir rootDir(mImageDir);
    files = rootDir.entryList(QStringList() << "*.tif" << "*.tiff" << "*.png" << "*.jpg" << "*.jpeg");
    for(QString f : files){
        if(f.contains("_label") || f.contains("_color"))
            continue;
        mFiles.append(f);
    }

    mImageCanvasWidget->loadImage(mImageDir, mOutputDir, mFiles.at(mCurrentIndex), mLabelingMode);
    mProjectInfoWidget->setImageName(mFiles.at(mCurrentIndex), mCurrentIndex+1, mFiles.size());
}

void ColorSelectionWidget::addButtonClicked()
{
    addColor(new LabelColor(getRandomColor(), mColorList.size(),mColorList.size(), " "));
    update();
}

void ColorSelectionWidget::fillSingleColor(QColor c, int num)
{
    if(mImageCanvasWidget == nullptr)
        return;
    mImageCanvasWidget->fillImageWithSingleColor(c, num);
}

void ColorSelectionWidget::newProject(QString source)
{
    ProjectSettingsDialog *dlg = new ProjectSettingsDialog(this, source);
    dlg->exec();
}

void ColorSelectionWidget::loadProject(QString file1Name)
{
    if(file1Name.isEmpty())
        file1Name = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("json (*.json)"));

    // load json
    QFile qFile(file1Name);
    if (!qFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open file.");
        return;
    }

    QByteArray savedData = qFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(savedData));
    QJsonObject jsonObj = loadDoc.object();

    mImageDir = jsonObj["imageDir"].toString();
    mOutputDir = jsonObj["outputDir"].toString();
    mCurrentIndex = jsonObj["currImageIndex"].toInt(0);
    mLabelingMode = jsonObj["labelingMode"].toBool(0);

    QStringList watchedPaths = mFileSystemWatcher.files();
    if(!watchedPaths.contains(mImageDir))
        mFileSystemWatcher.addPath(mImageDir);

    QJsonValue currImageIndex = jsonObj.value(QString("currImageIndex"));

    // TODO load colors?

    loadImages();
}

void ColorSelectionWidget::saveProject(bool saveImage)
{
    if(mImageDir.isEmpty() || mOutputDir.isEmpty())
        return;

    // save current images
    if(saveImage && !mFiles.isEmpty() && mFiles.length() > mCurrentIndex)
        mImageCanvasWidget->saveImages(mOutputDir, mFiles.at(mCurrentIndex));

    QJsonObject jsonObj;
    jsonObj["imageDir"] = mImageDir;
    jsonObj["outputDir"] = mOutputDir;
    jsonObj["currImageIndex"] = mCurrentIndex;
    jsonObj["labelingMode"] = mLabelingMode;

    // TODO save colors?

    QFile qFile(mOutputDir + "/labeling.json");
    if (qFile.open(QIODevice::WriteOnly)) {
        QJsonDocument jsonDoc(jsonObj);
        qFile.write(jsonDoc.toJson());
    }else{
        qWarning("Couldn't open file.");
    }
}

void ColorSelectionWidget::imageDirectoryChanged(QString dir)
{
    QDir rootDir(dir);
    QStringList files = rootDir.entryList(QStringList() << "*.txt");
    if(files.contains("update.txt"))
    {
        QFile file(rootDir.path() + "/update.txt");
        file.remove();

        setProject(mImageDir, mOutputDir, mLabelingMode);
    }
}

void ColorSelectionWidget::setProject(QString image, QString output, bool mode)
{
    // save current project
    saveProject();

    // create new project
    mImageDir = image;
    mOutputDir = output;
    mLabelingMode = mode;

    if(mImageDir.isEmpty() && mOutputDir.isEmpty())
        return;

    QStringList watchedPaths = mFileSystemWatcher.files();
    if(!watchedPaths.contains(mImageDir))
        mFileSystemWatcher.addPath(mImageDir);

    mCurrentIndex = 0;
    saveProject(false);
    loadImages();
}

void ColorSelectionWidget::labelColorDeleted(int index)
{
    mColorList.removeAt(index);
    for (int i = index; i < mColorList.size(); i++ ) {
        mColorList.at(i)->mIndex = i;
    }

    QLayoutItem *child = mLabelColorBtnsLayout->takeAt(index);
    clearLayout(child->layout());
}

void ColorSelectionWidget::colorChanged(int index)
{
    if(index >= mColorList.length() || index < 0)
        return;
    LabelColor *color = getLabelColor(index);
    mImageCanvasWidget->setPenColor( color->mColor );
    mImageCanvasWidget->setLabelValue( color->mLabelValue );
    mImageCanvasWidget->update();
}

void ColorSelectionWidget::setPenWidth(int s)
{
    mPenSizeWidget->setValue(s);
    mPenSizeEdit->setText(QString::number(s));
    mDial->setValue(s);
}

void ColorSelectionWidget::sliderPenWidthUpdate(int s)
{
    mPenSizeEdit->setText(QString::number(s));
    mDial->setValue(s);
    mImageCanvasWidget->setPenWidth(s);
}

void ColorSelectionWidget::editPenWidthUpdate(QString v)
{
    int s = v.toInt();
    if(s==0)
        return;

    mPenSizeWidget->setValue(s);
    mImageCanvasWidget->setPenWidth(s);
}

void ColorSelectionWidget::nextImage()
{
    // save current labeling
    saveProject();

    if((mCurrentIndex+1) < mFiles.length()){
        mCurrentIndex++;
        mImageCanvasWidget->loadImage(mImageDir, mOutputDir, mFiles.at(mCurrentIndex), mLabelingMode);
        mProjectInfoWidget->setImageName(mFiles.at(mCurrentIndex), mCurrentIndex+1, mFiles.size());
    }
}

void ColorSelectionWidget::prevImage()
{
    // save current labeling
    saveProject();

    if(mCurrentIndex > 0){
        mCurrentIndex--;
        mImageCanvasWidget->loadImage(mImageDir, mOutputDir, mFiles.at(mCurrentIndex), mLabelingMode);
        mProjectInfoWidget->setImageName(mFiles.at(mCurrentIndex), mCurrentIndex+1, mFiles.size());
    }
}

void ColorSelectionWidget::saveToJson()
{
    QJsonArray jsonObjList;

    for (int i =0; i < mColorList.size(); i++ ) {
        QJsonObject jsonObjItem;
        jsonObjItem["name"] = mColorList.at(i)->mName;
        jsonObjItem["labelValue"] = mColorList.at(i)->mLabelValue;
        jsonObjItem["colorR"] = mColorList.at(i)->mColor.red();
        jsonObjItem["colorG"] = mColorList.at(i)->mColor.green();
        jsonObjItem["colorB"] = mColorList.at(i)->mColor.blue();
        jsonObjItem["colorA"] = mColorList.at(i)->mColor.alpha();

        jsonObjList.append(jsonObjItem);
    }

    QFile qFile(QStringLiteral("colorLabels.json"));

    if (!qFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QJsonDocument jsonDoc(jsonObjList);
    qFile.write(jsonDoc.toJson());
}

void ColorSelectionWidget::loadFromJson()
{
    // read json and add ColorLabels
    QFile qFile("colorLabels.json");
    if (!qFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    // clear layout
    clearLayout(mLabelColorBtnsLayout);
    //clear m_color list
    mColorList.clear();

    QByteArray savedData = qFile.readAll();
    QJsonDocument loadDoc( QJsonDocument::fromJson(savedData));
    QJsonArray jsonArray = loadDoc.array();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        int r = obj["colorR"].toInt();
        int g = obj["colorG"].toInt();
        int b = obj["colorB"].toInt();
        int a = obj["colorA"].toInt();
        int labelValue = obj["labelValue"].toInt();
        QString name = obj["name"].toString();
        addColor(new LabelColor(QColor(r,g,b,a), labelValue, labelValue, name ));
    }
}

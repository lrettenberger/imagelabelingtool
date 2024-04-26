#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QFileInfo>

class ColorSelectionWidget;

class ProjectInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectInfoWidget(QWidget *parent = nullptr);
    void setColorSelectionWidget(ColorSelectionWidget *w);
    void setImageName(QString name, int currentId, int maxId);


private:
    ColorSelectionWidget *mCSWidget;

    QPushButton *mOpenDirBtn;
    QPushButton *mOpenOutputBtn;
    QPushButton *mOpenImageBtn;
    QLabel *mImageNameLbl;
    QLabel *mImageNumberLbl;

    void openExtern(const QFileInfo file, bool inExplorer = true);

signals:

public slots:
    void openImageDir();
    void openOutputDir();
    void openImage();

};


#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QStyleOption>
#include <QLineEdit>
#include <QMouseEvent>
#include <QWidget>
#include <QMenu>
#include <QSizePolicy>
#include <QSpinBox>
#include <QColorDialog>
#include <QApplication>

#include "colorselectiondialog.h"
#include "colorbutton.h"

class LabelColor : public QWidget
{
    Q_OBJECT

public:
    LabelColor();
    LabelColor(QColor color, int value, int index, QString name);

    void initLayout();

    int mIndex;
    QColor mColor;
    int mLabelValue;
    QString mName = "";

    QPushButton *mSelectColorButton;
    ColorButton *mLabelButton;
    ColorSelectionDialog *mConfigDialog;
    QHBoxLayout *mMainLayout;

private slots:
    void nameChanged(QString name);
    void labelValueChanged(int value);
    void colorChanged();
    void newColorSelected(QColor);
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void buttonClicked();
    void on_labelLeftClicked() { emit colorLabelClicked(mColor, mIndex); }
    void on_labelRightClicked(QPoint pos);
    void on_settingsAct();
    void on_deleteAct();

signals:
    void colorClicked(int);
    void colorLabelClicked(QColor , int);
    void colorLabelDelete(int);
    void stateChanged(bool);
    void sortImageBtnClicked(int index);
    void rightClicked(QPoint);

};


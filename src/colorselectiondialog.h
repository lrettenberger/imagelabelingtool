#pragma once

#include <QColor>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>

class ColorSelectionDialog: public QDialog
{
public:
    ColorSelectionDialog(QString name, QColor color, int labelvalue);

    QLineEdit *mNameEdit;
    QSpinBox *mValueSpinBox;
    QPushButton *mColorPickerBtn;

private:
    QString mName;
    QColor mColor;
    int mLabelValue;
    QDialogButtonBox *mOkButton;
    QVBoxLayout *mMainLayout;
    QHBoxLayout *mHlayoutName;
    QHBoxLayout *mHlayoutValue;
    QHBoxLayout *mHlayoutColor;
};

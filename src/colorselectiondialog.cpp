#include "colorselectiondialog.h"

ColorSelectionDialog::ColorSelectionDialog(QString name, QColor color, int labelvalue):
    mName(name), mColor(color), mLabelValue(labelvalue)
{
    mMainLayout = new QVBoxLayout(this);
    this->setLayout(mMainLayout);

    //setting name
    mHlayoutName = new QHBoxLayout();
    QLabel *labelName = new QLabel("name: ");
    mNameEdit = new QLineEdit();
    mNameEdit->setText(mName);
    mHlayoutName->addWidget(labelName);
    mHlayoutName->addWidget(mNameEdit);
    mMainLayout->addLayout(mHlayoutName);

    //setting label value
    mHlayoutValue = new QHBoxLayout();
    QLabel *labelValue = new QLabel("label value: ");
    mValueSpinBox = new QSpinBox();
    mValueSpinBox->setMaximum(255);
    mValueSpinBox->setMinimum(0);
    mValueSpinBox->setValue( mLabelValue);
    mHlayoutValue->addWidget(labelValue);
    mHlayoutValue->addWidget(mValueSpinBox,1);
    mMainLayout->addLayout(mHlayoutValue);

    // setting color
    mHlayoutColor = new QHBoxLayout();
    QLabel *labelColor = new QLabel("color: ");
    mColorPickerBtn = new QPushButton();
    int r = mColor.red();
    int g = mColor.green();
    int b = mColor.blue();
    int a = mColor.alpha();
    QString style = QString("background:rgb(%1,%2,%3,%4);").arg(r).arg(g).arg(b).arg(a);
    mColorPickerBtn->setStyleSheet(style);
    //m_colorPickerBtn->setMinimumWidth(150);
    mHlayoutColor->addWidget(labelColor);
    mHlayoutColor->addWidget(mColorPickerBtn,1);
    mMainLayout->addLayout(mHlayoutColor);

    mOkButton = new QDialogButtonBox(QDialogButtonBox::Ok);
    mOkButton->setCenterButtons(true);
    connect(mOkButton, SIGNAL(accepted()), this, SLOT(accept()));
    mMainLayout->addWidget(mOkButton);
}

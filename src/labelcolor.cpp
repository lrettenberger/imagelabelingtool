 #include "labelcolor.h"

LabelColor::LabelColor()
{

}

LabelColor::LabelColor(QColor color, int value, int index,QString name):
    mIndex(index), mColor(color), mLabelValue(value), mName(name)
{
    mLabelButton = new ColorButton(this);
    mLabelButton->setFlat(false);
    int r, g, b, a;
    color.getRgb(&r,&g,&b, &a);
    if(a != 0)
        mLabelButton->setStyleSheet(QString("background:rgb(%1,%2,%3,%4);").arg(r).arg(g).arg(b).arg(a));

    mLabelButton->setText(mName);
    mLabelButton->setMinimumSize(30,30);
    //connect(mLabelButton, SIGNAL(clicked()), this, SLOT(on_labelLeftClicked()));
    //connect(mLabelButton, SIGNAL(rightClicked(QPoint)), this, SLOT(on_labelRightClicked(QPoint)));

    mSelectColorButton = new QPushButton(QString::number(value));
    mSelectColorButton->setFixedSize(30,30);
    //connect(mSelectColorButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));


    connect(mLabelButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    initLayout();
}

void LabelColor::initLayout()
{
    mMainLayout = new QHBoxLayout();
    mMainLayout->setMargin(0);
    setLayout(mMainLayout);
    mMainLayout->addWidget(mLabelButton);
    //mMainLayout->addWidget(mSelectColorButton);
}

void LabelColor::nameChanged(QString name)
{
    mLabelButton->setText(name);
    mName = name;
    emit stateChanged(true);
}

void LabelColor::labelValueChanged(int value)
{
    mLabelValue = value;
    mSelectColorButton->setText(QString::number(value));
    emit stateChanged(true);
}

void LabelColor::colorChanged()
{
    QColor c(mColor.red(),mColor.green(), mColor.blue(), 100);
    QColorDialog *dialog = new QColorDialog(c, this);
    connect(dialog, SIGNAL(currentColorChanged(const QColor)), this, SLOT(newColorSelected(QColor)));
    dialog->exec();
}

void LabelColor::newColorSelected(QColor c)
{
    mColor = c;
    mColor.setAlpha(100);
    int r, g, b, a;
    mColor.getRgb(&r,&g,&b, &a);
    QString style = QString("background:rgb(%1,%2,%3,%4);").arg(r).arg(g).arg(b).arg(a);
    mLabelButton->setStyleSheet(style);
    mConfigDialog->mColorPickerBtn->setStyleSheet(style);

    emit stateChanged(true);
}

void LabelColor::on_labelRightClicked(QPoint pos)
{
    QMenu myMenu(this);
    QAction *settingsAction  = new QAction("Settings",this);
    connect(settingsAction,SIGNAL ( triggered()), this,SLOT( on_settingsAct()));
    myMenu.addAction(settingsAction);
    QAction *deleteAction  = new QAction("delete",this);
    connect(deleteAction,SIGNAL( triggered()), this, SLOT(on_deleteAct()));
    myMenu.addAction(deleteAction);
    myMenu.exec(pos);
}

void LabelColor::on_settingsAct()
{
    mConfigDialog = new ColorSelectionDialog(mName,mColor,mLabelValue);

    connect(mConfigDialog->mNameEdit, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    connect(mConfigDialog->mValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(labelValueChanged(int)) );
    connect(mConfigDialog->mColorPickerBtn, SIGNAL(clicked()), this, SLOT(colorChanged()));
    mConfigDialog->exec();
}

void LabelColor::on_deleteAct()
{
    emit colorLabelDelete(mIndex);
}

void LabelColor::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton) {
        emit rightClicked(e->globalPos());
     } else {
        QWidget::mouseReleaseEvent(e);
    }
}

void LabelColor::buttonClicked()
{
    if(QApplication::keyboardModifiers() & Qt::ControlModifier)
        on_labelLeftClicked();
    else
        emit colorClicked(mIndex);
}

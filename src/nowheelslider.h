#ifndef NOWHEELSLIDER_H
#define NOWHEELSLIDER_H

#include <QSlider>

class NoWheelSlider : public QSlider
{
    Q_OBJECT
public:
    explicit NoWheelSlider(QWidget *parent = nullptr): QSlider(parent) {}
    explicit NoWheelSlider(Qt::Orientation orientation, QWidget *parent = nullptr): QSlider(orientation, parent) {}

protected:
    // wheel event gets handeled by main window
    void wheelEvent(QWheelEvent *event) override
    {
        return QWidget::wheelEvent( event);
    }
};

#endif // NOWHEELSLIDER_H

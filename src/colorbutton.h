#pragma once

#include <QPushButton>
#include <QMouseEvent>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = nullptr) : QPushButton(parent) {}

private slots:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if(event->button()==Qt::RightButton) {
            emit rightClicked(event->globalPos());
         } else {
            QPushButton::mouseReleaseEvent(event);
        }
        if(event->button()==Qt::LeftButton){
            emit leftClicked(event->globalPos());
        }else{
            QPushButton::mouseReleaseEvent(event);
        }
    }

signals:
    void rightClicked(QPoint);
    void leftClicked(QPoint);
};

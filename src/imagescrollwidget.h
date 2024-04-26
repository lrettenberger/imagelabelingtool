#pragma once

#include <QWidget>
#include <QScrollArea>

class ImageScrollWidget: public QScrollArea
{
public:
    ImageScrollWidget();

private:
    bool mMousePressed;
    QPoint mLastPoint;

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
};

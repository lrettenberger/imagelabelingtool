#include "imagescrollwidget.h"

#include <QScrollBar>
#include <QMouseEvent>

ImageScrollWidget::ImageScrollWidget():
    mMousePressed(false)
{

}

void ImageScrollWidget::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
}

void ImageScrollWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        mLastPoint = event->pos();
        mMousePressed = true;

        scrollContentsBy(20, 0);
    }
}

void ImageScrollWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed && event->buttons() == Qt::RightButton) {
        QPoint currentPoint = event->pos();
        QPoint delta = currentPoint-mLastPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        mLastPoint = currentPoint;
    }
}

void ImageScrollWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mMousePressed = false;
    if (event->buttons() == Qt::RightButton) {
        QPoint currentPoint = event->pos();
        QPoint delta = currentPoint-mLastPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        mLastPoint = currentPoint;
    }
}

void ImageScrollWidget::keyPressEvent(QKeyEvent *event)
{

}

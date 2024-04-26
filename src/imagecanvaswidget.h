#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QRgb>

#include "colorselectionwidget.h"

class ImageCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCanvasWidget(ColorSelectionWidget* w, QWidget *parent = nullptr);

    void setLabelValue(const int labelValue);
    void setPenWidth(const int width);
    void setPenColor(const QColor &color) {mPenColor = color; drawPreviewCircle(mapFromGlobal(QCursor::pos()));}

    QColor penColor() const { return mPenColor; }
    int penWidth() const { return mPenWidth; }

    void loadImage(const QString imageDir, const QString outputDir, const QString fileName, const bool labelMode);
    void saveImages(QString outputPath, QString imageName);

    void fillImageWithSingleColor(const QColor color, const int value);
    void clearImage();

    bool mDrawLabel;

private:
    ColorSelectionWidget *mColorSelectionWidget;
    bool mLabelMode;

    bool mImageIsLoaded;
    bool mImageIsModified;
    bool mMousePressed;
    bool mModeSingleLabelImage;

    // area and scale that is drawn on
    double mDrawScale;
    QRect mDrawRect;

    // images
    QImage mImage;                      // original image
    QList<QImage> mNumericCanvasList;   // numeric canvas (mask)
    QList<QImage> mColorCanvasList;     // color canvas (color overlay)
    QImage mPenPreview;                 // paint preview cavas

    // pen configuration
    int mPenWidth;
    int mLabelValue;
    int mPrevLabelValue;
    QColor mPenColor;
    QPoint mLastPoint;
    QPoint mLastPreviewPosition;

    // painting
    void drawLineTo(const QPoint &endPoint);
    void drawCircle();
    void drawPreviewCircle(QPoint position);

    // undo
    QList<QList<QImage>> mLabelHistory;
    QList<QList<QImage>> mColorHistory;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:


public slots:
    void undo();
};

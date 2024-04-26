#include "imagecanvaswidget.h"

#include <QApplication>
#include <QWheelEvent>
#include <QtMath>

ImageCanvasWidget::ImageCanvasWidget(ColorSelectionWidget *w, QWidget *parent) : QWidget(parent), mDrawLabel(true), mColorSelectionWidget(w),
    mImageIsLoaded(false), mImageIsModified(false), mMousePressed(false), mDrawScale(1),
    mPenWidth(40), mLabelValue(0), mPrevLabelValue(0), mPenColor(QColor(0,0,0,0)),
    mLastPreviewPosition(QPoint(0,0))
{
    setAttribute(Qt::WA_StaticContents);
    this->setMouseTracking(true);
    setMinimumSize(100,100);
}

void ImageCanvasWidget::setLabelValue(const int labelValue)
{
    mPrevLabelValue = mLabelValue;
    mLabelValue = labelValue;
}

void ImageCanvasWidget::setPenWidth(const int width)
{
    mPenWidth = width;
    QPoint point = mapFromGlobal(QCursor::pos());
    QRect rect(QPoint(0,0), size());
    if(rect.contains(point))
        drawPreviewCircle(point);
    else
        drawPreviewCircle(rect.center());

}

void ImageCanvasWidget::loadImage(const QString imageDir, const QString outputDir, const QString fileName, const bool labelMode)
{
    bool keepLabel = false;
    if(QApplication::keyboardModifiers() == Qt::ControlModifier)
        keepLabel = true;

    QList<QImage> oldNumericCanvasList = mNumericCanvasList;

    mLabelMode = labelMode;

    QImage loadedImage;
    if (!loadedImage.load(imageDir + "/" + fileName))
        return;

    mLabelHistory.clear();
    mColorHistory.clear();

    QSize newSize = loadedImage.size();
    setMinimumSize(newSize);
    mImage = loadedImage;

    mColorCanvasList.clear();
    mColorCanvasList.append(QImage(newSize, QImage::Format_ARGB32));
    mColorCanvasList[0].fill(Qt::transparent);
    if(labelMode){
        for(int i = 1; i < 9; i++){
            mColorCanvasList.append(QImage(newSize, QImage::Format_ARGB32));
            mColorCanvasList[i].fill(Qt::transparent);
        }
    }
    mColorHistory.append(mColorCanvasList);

    mNumericCanvasList.clear();
    mNumericCanvasList.append(QImage(newSize, QImage::Format_Grayscale8));
    mNumericCanvasList[0].fill(Qt::black);
    if(labelMode){
        for(int i = 1; i < 9; i++){
            mNumericCanvasList.append(QImage(newSize, QImage::Format_Grayscale8));
            mNumericCanvasList[i].fill(Qt::black);
        }
    }
    mLabelHistory.append(mNumericCanvasList);

    mPenPreview = QImage(newSize, QImage::Format_ARGB32);
    mPenPreview.fill(Qt::transparent);

    mDrawRect = QRect(QPoint(0,0), mDrawScale * mImage.size());
    setMinimumSize(mDrawRect.size());

    mImageIsLoaded = true;
    mImageIsModified = false;

    // load existing labels
    QString imageName = fileName.mid(0, fileName.lastIndexOf(".") );
    if(!mLabelMode){
        if(keepLabel && !mNumericCanvasList.isEmpty() && !oldNumericCanvasList.isEmpty() && (mNumericCanvasList[0].size() == oldNumericCanvasList[0].size())){
            QString file = outputDir + "/" + imageName + "_label.tif";
            QImage loadedImage;
            if (loadedImage.load(file)){
                mLabelHistory[0][0] = loadedImage;
            }
            mNumericCanvasList = oldNumericCanvasList;
        }else{
            QString file = outputDir + "/" + imageName + "_label.tif";
            QImage loadedImage;
            if (!loadedImage.load(file)){
                update();
                return;
            }
            mNumericCanvasList[0] = loadedImage;
        }

        // create colored mask
        QImage mask(mNumericCanvasList[0].size(), QImage::Format_ARGB32);
        mask.fill(Qt::transparent);

        for (int y = 0; y < mNumericCanvasList[0].size().height(); y++ ) {
            for (int x = 0; x < mNumericCanvasList[0].size().width(); x++ ) {
                QColor color = mNumericCanvasList[0].pixelColor(x,y);

                int labelValue = color.value();

                if (labelValue == 0) {
                    mask.setPixelColor(x,y, QColor(0,0,0,0));
                } else {
                    mask.setPixelColor(x,y, mColorSelectionWidget->getColor(labelValue));
                }
            }
        }
        mColorCanvasList[0] = mask;

        // fill history mask
        mask.fill(Qt::transparent);
        for (int y = 0; y < mLabelHistory[0][0].size().height(); y++ ) {
            for (int x = 0; x < mLabelHistory[0][0].size().width(); x++ ) {
                QColor color = mLabelHistory[0][0].pixelColor(x,y);

                int labelValue = color.value();

                if (labelValue == 0) {
                    mask.setPixelColor(x,y, QColor(0,0,0,0));
                } else {
                    mask.setPixelColor(x,y, mColorSelectionWidget->getColor(labelValue));
                }
            }
        }
        mColorHistory[0][0] = mask;

    }else{
        for(int i = 0; i < 9; i++){
            if(keepLabel && (mNumericCanvasList[0].size() == oldNumericCanvasList[0].size())){
                QString file = outputDir + "/" + imageName + "_label_" + QString::number(i) + ".tif";
                QImage loadedImage;
                if (!loadedImage.load(file))
                    mLabelHistory[0][i] = loadedImage;
                mNumericCanvasList = oldNumericCanvasList;
            }else{
                QString file = outputDir + "/" + imageName + "_label_" + QString::number(i) + ".tif";
                QImage loadedImage;
                if (!loadedImage.load(file))
                    continue;
                mNumericCanvasList[i] = loadedImage;
            }

            // create colored mask
            QImage mask(mNumericCanvasList[i].size(), QImage::Format_ARGB32);
            mask.fill(Qt::transparent);

            for (int y = 0; y < mNumericCanvasList[i].size().height(); y++ ) {
                for (int x = 0; x < mNumericCanvasList[i].size().width(); x++ ) {
                    QColor color = mNumericCanvasList[i].pixelColor(x,y);

                    int labelValue = color.value();

                    if (labelValue == 0) {
                        mask.setPixelColor(x,y, QColor(0,0,0,0));
                    } else {
                        mask.setPixelColor(x,y, mColorSelectionWidget->getColor(labelValue));
                    }
                }
            }
            mColorCanvasList[i] = mask;

            // fill history mask
            mask.fill(Qt::transparent);
            for (int y = 0; y < mLabelHistory[0][i].size().height(); y++ ) {
                for (int x = 0; x < mLabelHistory[0][i].size().width(); x++ ) {
                    QColor color = mLabelHistory[0][i].pixelColor(x,y);

                    int labelValue = color.value();

                    if (labelValue == 0) {
                        mask.setPixelColor(x,y, QColor(0,0,0,0));
                    } else {
                        mask.setPixelColor(x,y, mColorSelectionWidget->getColor(labelValue));
                    }
                }
            }
            mColorHistory[0][i] = mask;
        }
    }

    update();
}

void ImageCanvasWidget::saveImages(QString outputPath, QString imageFileName)
{
    if(!mImageIsLoaded)
        return;

    QString imageName = imageFileName.mid(0, imageFileName.lastIndexOf(".") );
    QString imageExtension = ".tif";

    if(!mLabelMode){
        mNumericCanvasList[0].save(outputPath + "/" + imageName + "_label" + imageExtension);

        QImage saveImage(mImage.size(), QImage::Format_ARGB32);
        QPainter patchPainter(&saveImage);
        patchPainter.setPen(Qt::NoPen);
        patchPainter.setBrush(Qt::NoBrush);
        patchPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        patchPainter.drawImage(0,0, mImage);
        patchPainter.drawImage(0,0, mColorCanvasList[0]);

        saveImage.save(outputPath + "/" + imageName + "_color" + imageExtension);
    }else{
        for(int i = 1; i < 9; i++){

            bool imageHasContent = false;
            for (int y = 0; y < mNumericCanvasList[i].size().height(); y++ ) {
                for (int x = 0; x < mNumericCanvasList[i].size().width(); x++ ) {
                    QColor color = mNumericCanvasList[i].pixelColor(x,y);
                    int labelValue = color.value();
                    if (labelValue != 0) {
                        imageHasContent = true;
                        break;
                    }
                }
                if(imageHasContent)
                    break;
            }
            if(!imageHasContent)
                continue;

            mNumericCanvasList[i].save(outputPath + "/" + imageName + "_label_" + QString::number(i) + imageExtension);

            QImage saveImage(mImage.size(), QImage::Format_ARGB32);
            QPainter patchPainter(&saveImage);
            patchPainter.setPen(Qt::NoPen);
            patchPainter.setBrush(Qt::NoBrush);
            patchPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            patchPainter.drawImage(0,0, mImage);
            patchPainter.drawImage(0,0, mColorCanvasList[i]);

            saveImage.save(outputPath + "/" + imageName + "_color_" + QString::number(i) + imageExtension);
        }
    }
}

void ImageCanvasWidget::drawLineTo(const QPoint &endPoint)
{
    if(!mImageIsLoaded)
        return;

    // create drawing rect
    float halfPenWidth = ((mPenWidth+1)/2);
    int minx = ceil((1.0/mDrawScale)*qMin(mLastPoint.x(), endPoint.x()) - halfPenWidth);
    int miny = ceil((1.0/mDrawScale)*qMin(mLastPoint.y(), endPoint.y()) - halfPenWidth);
    int maxx = ceil((1.0/mDrawScale)*qMax(mLastPoint.x(), endPoint.x()) + halfPenWidth);
    int maxy = ceil((1.0/mDrawScale)*qMax(mLastPoint.y(), endPoint.y()) + halfPenWidth);

    int offset = 0;
    if((mPenWidth%2) == 1)
        offset = -1;

    int startx = ceil((1.0/mDrawScale)*mLastPoint.x()) - minx + offset;
    int starty = ceil((1.0/mDrawScale)*mLastPoint.y()) - miny + offset;
    int endx   = ceil((1.0/mDrawScale)*endPoint.x()) - minx + offset;
    int endy   = ceil((1.0/mDrawScale)*endPoint.y()) - miny + offset;

    QRect orgRect(minx, miny, maxx-minx, maxy-miny);
    QImage imagePatch = mImage.copy(orgRect);

    int colorIndex = 0;
    if(mLabelMode)
        colorIndex = mLabelValue;
    if(mLabelMode && mLabelValue == 0)
        colorIndex = mPrevLabelValue;

    QImage drawOrgPatch = mColorCanvasList[colorIndex].copy(orgRect);
    QImage drawPatch = drawOrgPatch;

    QImage labelOrgPatch = mNumericCanvasList[colorIndex].copy(orgRect);
    QImage labelPatch = labelOrgPatch;

    QPainter::CompositionMode mode = QPainter::CompositionMode_Source;

    QPen pen;
    if(mPenWidth < 5){
        pen.setCapStyle(Qt::SquareCap);
    } else {
        pen.setCapStyle(Qt::RoundCap);
    }
    pen.setColor(mPenColor);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(mPenWidth);

    QPainter patchPainter(&drawPatch);
    patchPainter.setPen(pen);
    patchPainter.setBrush(Qt::NoBrush);
    patchPainter.setCompositionMode(mode);
    patchPainter.drawLine(QPoint(startx,starty), QPoint(endx,endy));

    QPen penLabel;
    if(mPenWidth < 5){
        penLabel.setCapStyle(Qt::SquareCap);
    } else {
        penLabel.setCapStyle(Qt::RoundCap);
    }
    penLabel.setColor(QColor(mLabelValue, mLabelValue, mLabelValue));
    penLabel.setStyle(Qt::SolidLine);
    penLabel.setWidth(mPenWidth);

    QPainter painterLabel(&labelPatch);
    painterLabel.setPen(penLabel);
    painterLabel.setBrush(Qt::NoBrush);
    painterLabel.setCompositionMode(mode);
    painterLabel.drawLine(QPoint(startx,starty), QPoint(endx,endy));

    if(QApplication::keyboardModifiers() != Qt::ControlModifier){
        for (int y = 0; y < drawOrgPatch.size().height(); y++ ) {
            for (int x = 0; x < drawOrgPatch.size().width(); x++ ) {
                if(drawOrgPatch.pixelColor(x,y) != QColor(0,0,0,0)){
                    drawPatch.setPixelColor(x,y,drawOrgPatch.pixelColor(x,y));
                    labelPatch.setPixelColor(x,y,labelOrgPatch.pixelColor(x,y));
                }
            }
        }
    }

    QPainter painter(&mColorCanvasList[colorIndex]);
    painter.setCompositionMode(mode);
    painter.drawImage(orgRect, drawPatch);

    QPainter labelPainter(&mNumericCanvasList[colorIndex]);
    labelPainter.setCompositionMode(mode);
    labelPainter.drawImage(orgRect, labelPatch);

    mImageIsModified = true;
    update();
    mLastPoint = endPoint;
}

void ImageCanvasWidget::drawCircle()
{
    if(!mImageIsLoaded)
        return;

    int colorIndex = 0;
    if(mLabelMode)
        colorIndex = mLabelValue;
    if(mLabelMode && mLabelValue == 0)
        colorIndex = mPrevLabelValue;

    int minx = ceil((1.0/mDrawScale)*mLastPoint.x() - ((mPenWidth+1)/2));
    int miny = ceil((1.0/mDrawScale)*mLastPoint.y() - ((mPenWidth+1)/2));
    QRect paintingRect(QPoint(minx, miny), QSize(mPenWidth, mPenWidth));

    QImage drawOrgPatch = mColorCanvasList[colorIndex].copy(paintingRect);
    QImage drawPatch = drawOrgPatch;

    QImage labelOrgPatch = mNumericCanvasList[colorIndex].copy(paintingRect);
    QImage labelPatch = labelOrgPatch;

    QImage imagePatch = mImage.copy(paintingRect);

    QPainter::CompositionMode mode = QPainter::CompositionMode_Source;

    QPen pen;
    if(mPenWidth < 5){
        pen.setCapStyle(Qt::SquareCap);
    } else {
        pen.setCapStyle(Qt::RoundCap);
    }
    pen.setColor(mPenColor);
    pen.setStyle(Qt::SolidLine);

    QPainter patchPainter(&drawPatch);
    patchPainter.setPen(pen);
    patchPainter.setBrush(pen.color());
    patchPainter.setCompositionMode(mode);
    if(mPenWidth == 1)
        patchPainter.drawPoint(0, 0);
    if(mPenWidth < 5)
        patchPainter.drawRect(QRect(0, 0, mPenWidth-1, mPenWidth-1));
    else
        patchPainter.drawEllipse(QRect(0, 0, mPenWidth-1, mPenWidth-1));

    QPen penLabel;
    if(mPenWidth < 5){
        penLabel.setCapStyle(Qt::SquareCap);
    } else {
        penLabel.setCapStyle(Qt::RoundCap);
    }
    penLabel.setColor(QColor(mLabelValue, mLabelValue, mLabelValue));
    penLabel.setStyle(Qt::SolidLine);

    QPainter painterLabel(&labelPatch);
    painterLabel.setPen(penLabel);
    painterLabel.setBrush(penLabel.color());
    painterLabel.setCompositionMode(mode);
    if(mPenWidth == 1)
        painterLabel.drawPoint(0, 0);
    if(mPenWidth < 5)
        painterLabel.drawRect(QRect(0, 0, mPenWidth-1, mPenWidth-1));
    else
        painterLabel.drawEllipse(QRect(0, 0, mPenWidth-1, mPenWidth-1));

    /*
    for (int y = 0; y < mPenWidth; y++ ) {
        for (int x = 0; x < mPenWidth; x++ ) {
            if(QApplication::keyboardModifiers() != Qt::ControlModifier && drawOrgPatch.pixelColor(x,y) != QColor(0,0,0,0)){
                drawPatch.setPixelColor(x,y,drawOrgPatch.pixelColor(x,y));
                labelPatch.setPixelColor(x,y,labelOrgPatch.pixelColor(x,y));
            }
            if(drawOrgPatch.pixelColor(x,y) == Qt::black){
                drawPatch.setPixelColor(x,y,QColor(0,0,0,0));
                labelPatch.setPixelColor(x,y,Qt::black);
            }
        }
    }
    //*/

    if(QApplication::keyboardModifiers() != Qt::ControlModifier){
        for (int y = 0; y < mPenWidth; y++ ) {
            for (int x = 0; x < mPenWidth; x++ ) {
                if(drawOrgPatch.pixelColor(x,y) != QColor(0,0,0,0)){
                    drawPatch.setPixelColor(x,y,drawOrgPatch.pixelColor(x,y));
                    labelPatch.setPixelColor(x,y,labelOrgPatch.pixelColor(x,y));
                }
            }
        }
    }

    QPainter painter(&mColorCanvasList[colorIndex]);
    painter.setCompositionMode(mode);
    painter.drawImage(paintingRect, drawPatch);

    QPainter labelPainter(&mNumericCanvasList[colorIndex]);
    labelPainter.setCompositionMode(mode);
    labelPainter.drawImage(paintingRect, labelPatch);

    mImageIsModified = true;
    update();
}

void ImageCanvasWidget::drawPreviewCircle(QPoint position)
{
    if(!mImageIsLoaded)
        return;

    int minx = ceil((1.0/mDrawScale)*position.x() - ((mPenWidth+1)/2));
    int miny = ceil((1.0/mDrawScale)*position.y() - ((mPenWidth+1)/2));
    QRect paintingRect(QPoint(minx, miny), QSize(mPenWidth-1, mPenWidth-1));

    mPenPreview.fill(Qt::transparent);
    QPen pen;
    if(mPenWidth < 5){
        pen.setCapStyle(Qt::SquareCap);
    } else {
        pen.setCapStyle(Qt::RoundCap);
    }

    pen.setColor(mPenColor);
    if(mPenWidth >= 7 || mLabelValue == 0)
        pen.setColor(QColor(255,255,255));
    pen.setStyle(Qt::SolidLine);

    QBrush brush;
    brush.setColor(mPenColor);
    brush.setStyle(Qt::SolidPattern);

    QPainter painter(&mPenPreview);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setBrush(mPenColor);
    painter.setPen(pen);
    if(mPenWidth == 1)
        painter.drawPoint(minx, miny);
    if(mPenWidth < 5)
        painter.drawRect(paintingRect);
    else
        painter.drawEllipse(paintingRect);
    update();
}

void ImageCanvasWidget::wheelEvent(QWheelEvent *event)
{
    if(Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        mDrawScale = mDrawScale * (1+static_cast<double>(event->angleDelta().ry())/3600);
        mDrawRect = QRect(QPoint(0,0), mDrawScale * mImage.size());
        setMinimumSize(mDrawRect.size());
        update();
    }else{
        QWidget::wheelEvent(event);
    }
}

void ImageCanvasWidget::undo()
{
    if(!mLabelHistory.isEmpty()){
        mNumericCanvasList = mLabelHistory.last();
        mLabelHistory.pop_back();
    }
    if(!mColorHistory.isEmpty()){
        mColorCanvasList = mColorHistory.last();
        mColorHistory.pop_back();
    }
    update();
}

void ImageCanvasWidget::fillImageWithSingleColor(const QColor color, const int index)
{
    int colorIndex = 0;
    if(mLabelMode)
        colorIndex = mLabelValue;
    if(mLabelMode && mLabelValue == 0)
        colorIndex = mPrevLabelValue;

    mLabelHistory.push_back(mNumericCanvasList);
    if(mLabelHistory.length() > 10)
        mLabelHistory.pop_front();

    mColorHistory.push_back(mColorCanvasList);
    if(mColorHistory.length() > 10)
        mColorHistory.pop_front();

    if(Qt::ControlModifier != QApplication::keyboardModifiers()){
        for (int y = 0; y < mImage.size().height(); y++ ) {
            for (int x = 0; x < mImage.size().width(); x++ ) {

                QColor curColor = mImage.pixelColor(x,y);
                if(curColor == Qt::black)
                    continue;

                curColor = mNumericCanvasList[colorIndex].pixelColor(x,y);
                if(curColor != Qt::black)
                    continue;

                mColorCanvasList[colorIndex].setPixelColor(x,y, color);
                mNumericCanvasList[colorIndex].setPixelColor(x,y, QColor(index,index,index));
            }
        }
    }else{
        mColorCanvasList[colorIndex].fill(color);
        mNumericCanvasList[colorIndex].fill(QColor(index,index,index));
    }
    update();
}

void ImageCanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        mLabelHistory.push_back(mNumericCanvasList);
        if(mLabelHistory.length() > 10)
            mLabelHistory.pop_front();

        mColorHistory.push_back(mColorCanvasList);
        if(mColorHistory.length() > 10)
            mColorHistory.pop_front();

        mLastPoint = event->pos();
        this->mMousePressed = true;
    }else{
        QWidget::mousePressEvent(event);
    }
}

void ImageCanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        drawLineTo(event->pos());
    }else{
        QWidget::mouseMoveEvent(event);
    }

    drawPreviewCircle(event->pos());
}

void ImageCanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (mLastPoint == event->pos()) {
            drawCircle();
        } else {
            drawLineTo(event->pos());
        }
        this->mMousePressed = false;
    }else{
        QWidget::mouseReleaseEvent(event);
    }
}

void ImageCanvasWidget::paintEvent(QPaintEvent *event)
{
    if(!mImageIsLoaded)
        return;

    int colorIndex = 0;
    if(mLabelMode)
        colorIndex = mLabelValue;
    if(mLabelMode && mLabelValue == 0)
        colorIndex = mPrevLabelValue;

    QPainter painterWidget(this);
    painterWidget.drawImage(mDrawRect, mImage);
    if(mDrawLabel)
        painterWidget.drawImage(mDrawRect, mColorCanvasList[colorIndex]);
    painterWidget.drawImage(mDrawRect, mPenPreview);
}

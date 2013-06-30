#include <QtGui>

 #include "Imagearea.h"

 ImageArea::ImageArea(QWidget *parent)
     : QWidget(parent)
 {
     setAttribute(Qt::WA_StaticContents);
     modified = false;
     scribbling = false;
     myPenWidth = 1;
     myPenColor = Qt::blue;
 }

 bool ImageArea::openImage(const QString &fileName)
 {
     QImage loadedImage;
     if (!loadedImage.load(fileName))
         return false;

	 /*QSize newSize = loadedImage.size().expandedTo(size());
	 resizeImage(&loadedImage, newSize);*/
     image = loadedImage;
     modified = false;
     update();
     return true;
 }

 bool ImageArea::saveImage(const QString &fileName, const char *fileFormat)
 {
     QImage visibleImage = image;
     resizeImage(&visibleImage, size());

     if (visibleImage.save(fileName, fileFormat)) {
         modified = false;
         return true;
     } else {
         return false;
     }
 }

 void ImageArea::setPenColor(const QColor &newColor)
 {
     myPenColor = newColor;
 }


 void ImageArea::clearImage()
 {
     image.fill(qRgb(255, 255, 255));
     modified = true;
     update();
 }

 void ImageArea::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton) {
         lastPoint = event->pos();
         scribbling = true;
     }
 }

 void ImageArea::mouseMoveEvent(QMouseEvent *event)
 {
     if ((event->buttons() & Qt::LeftButton) && scribbling)
         drawLineTo(event->pos());
 }

 void ImageArea::mouseReleaseEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton && scribbling) {
         drawLineTo(event->pos());
         scribbling = false;
     }
 }

 void ImageArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
	 QRect eventRect = event->rect();
     QRect dirtyRect;
	 dirtyRect.setTop(0);
	 dirtyRect.setLeft(0);
	 dirtyRect.setBottom(image.height());
	 dirtyRect.setRight(image.width());
	 QRect targetRect;
	 targetRect.setLeft(0);
	 targetRect.setTop(eventRect.bottom() - image.height());
	 targetRect.setRight(image.width());
	 targetRect.setBottom(eventRect.bottom());
     painter.drawImage(targetRect, image, dirtyRect);
 }

 void ImageArea::resizeEvent(QResizeEvent *event)
 {
     if (width() > image.width() || height() > image.height()) {
         int newWidth = qMax(width() + 128, image.width());
         int newHeight = qMax(height() + 128, image.height());
         resizeImage(&image, QSize(newWidth, newHeight));
         update();
     }
     QWidget::resizeEvent(event);
 }

 void ImageArea::drawLineTo(const QPoint &endPoint)
 {
     QPainter painter(&image);
     painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
     painter.drawLine(lastPoint, endPoint);
     modified = true;

     int rad = (myPenWidth / 2) + 2;
     update(QRect(lastPoint, endPoint).normalized()
                                      .adjusted(-rad, -rad, +rad, +rad));
     lastPoint = endPoint;
 }

 void ImageArea::resizeImage(QImage *image, const QSize &newSize)
 {
     if (image->size() == newSize)
         return;

     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
     QPainter painter(&newImage);
     painter.drawImage(QPoint(0, 0), *image);
     *image = newImage;
 }

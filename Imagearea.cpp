#include <QtGui>

#include "Imagearea.h"
#include <sstream>
#include <string>

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

	// axis
	painter.drawLine(QPoint(axis_width - 1, 0), QPoint(axis_width - 1, eventRect.height() - axis_width - 1));
	painter.drawLine(QPoint(axis_width - 1, eventRect.height() - axis_width - 1), QPoint(eventRect.width(), eventRect.height() - axis_width - 1));
	for(int i = 0; i < eventRect.width(); ++i)
	{
		if(!(i % 50))
		{
			std::stringstream ss;
			std::string str;
			ss << i;
			ss >> str;
			painter.drawText(QPoint(axis_width - 1 + i, eventRect.height() - axis_width + 12), str.c_str());
		}	
	}

	for(int i = 0; i < eventRect.height(); ++i)
	{
		if(!(i % 50))
		{
			std::stringstream ss;
			std::string str;
			ss << i;
			ss >> str;
			painter.drawText(QPoint(axis_width - 1 - 12 - str.size() * 3, eventRect.height() - axis_width - i), str.c_str());
		}	
	}


	QRect sourceRect;
	sourceRect.setTop(0);
	sourceRect.setLeft(0);
	sourceRect.setBottom(image.height());
	sourceRect.setRight(image.width());
	QRect targetRect;
	targetRect.setLeft(axis_width);
	targetRect.setTop(eventRect.bottom() - image.height() - axis_width);
	targetRect.setRight(image.width() + axis_width);
	targetRect.setBottom(eventRect.bottom() - axis_width);
	painter.drawImage(targetRect, image, sourceRect);
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

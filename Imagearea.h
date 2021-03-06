#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ImageArea : public QWidget
{
	Q_OBJECT
	
public:
	ImageArea(QWidget * parent = 0);
	
	bool openImage(const QString &fileName);
	bool saveImage(const QString &fileName, const char *fileFormat);
	void setPenColor(const QColor &newColor);
	
	bool isModified() const {return modified;}
	QColor penColor() const {return myPenColor;}
	
	static const unsigned int window_width = 1024;
	static const unsigned int window_height = 768;

	static const unsigned int axis_width = 50;

public slots:
	void clearImage();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	
private:
	void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
};
#endif
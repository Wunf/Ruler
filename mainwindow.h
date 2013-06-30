#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class ImageArea;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void open();
	void save();
	void penColor();
	void about();
	
private:
	void createActions();
	void createMenus();
	bool maybeSave();
	bool saveFile(const QByteArray &fileFormat);
	
	ImageArea * imageArea;
	
	QMenu *saveAsMenu;
	QMenu *fileMenu;
	QMenu *optionMenu;
	QMenu *helpMenu;
	
	QAction *openAct;
	QList<QAction *> saveAsActs;
	QAction *exitAct;
	QAction *penColorAct;
	QAction *printAct;
	QAction *clearScreenAct;
	QAction *aboutAct;
	QAction *aboutQtAct;

	unsigned int my_width;
	unsigned int my_height;
};

#endif
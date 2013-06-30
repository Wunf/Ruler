#include <QtGui>

#include "mainwindow.h"
#include "imagearea.h"

MainWindow::MainWindow() : my_width(window_width), my_height(window_height)
{
	imageArea = new ImageArea;
	setCentralWidget(imageArea);
	
	createActions();
	createMenus();
	
	setWindowTitle("Ruler");
	resize(my_width, my_height);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(maybeSave())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainWindow::open()
{
	if(maybeSave())
	{
		QString filename = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
		if(!filename.isEmpty())
			imageArea->openImage(filename);
	}
}

void MainWindow::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void MainWindow::penColor()
{
	QColor newColor = QColorDialog::getColor(imageArea->penColor());
	if(newColor.isValid())
		imageArea->setPenColor(newColor);
}

void MainWindow::about()
{
	QMessageBox::about(this, "About Ruler", "Say Something...");
}

void MainWindow::createActions()
{
	openAct = new QAction("&Open...", this);
	openAct->setShortcuts(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	
	foreach(QByteArray format, QImageWriter::supportedImageFormats())
	{
		QString text = tr("%1...").arg(QString(format).toUpper());
		
		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
		saveAsActs.append(action);
	}
	
	exitAct = new QAction("E&xit", this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	
	penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
	
	clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
             imageArea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
	saveAsMenu = new QMenu("&Save As", this);
	foreach(QAction *action, saveAsActs)
		saveAsMenu->addAction(action);
		
	fileMenu = new QMenu("&File", this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	
	optionMenu = new QMenu("&Option", this);
	optionMenu->addAction(penColorAct);
	optionMenu->addSeparator();
	optionMenu->addAction(clearScreenAct);
	
	helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
	
	menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
	if(imageArea->isModified())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, "Ruler", "The image has been modified.\nDo you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if(ret == QMessageBox::Save)
		{
			return saveFile("png");
		}
		else if(ret == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
	
	QString fileName = QFileDialog::getSaveFileName(this, "Save As", initialPath, tr("%1 Files (*.%2);;All Files (*)").arg(QString(fileFormat.toUpper())).arg(QString(fileFormat)));
	if (fileName.isEmpty()) 
	{
        return false;
    } 
	else 
	{
        return imageArea->saveImage(fileName, fileFormat);
    }
}
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mTimer = new QTimer(this);
	connect(
			ui->widget,SIGNAL(updateWave(int)),
			ui->widget, SLOT(waveNextRender(int))
			);
	connect(
			ui->widget_2,SIGNAL(updateWave(int)),
			ui->widget_2, SLOT(waveNextRender(int))
			);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(cursorForward()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	if(e->key() == Qt::Key_PageUp) {
		mTimer->start(33);
	} else if(e->key() == Qt::Key_PageDown) {
		mTimer->stop();
	}
}

void MainWindow::cursorForward() {
	this->ui->widget->cursorForward(33);
	this->ui->widget_2->cursorForward(60);
}

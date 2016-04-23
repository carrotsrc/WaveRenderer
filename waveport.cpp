#include "waveport.hpp"
#include <QPainter>
#include <iostream>
#include <fstream>
WavePort::WavePort(QWidget *parent) : QWidget(parent)
{
	QPoint p(600,140);

	std::ifstream f("/home/cfg/VibeAttack.raw", std::ios::binary);

	if(f.fail()) {
		std::cout << "Failed to load" << std::endl;
		return;
	}

	f.seekg (0, f.end);
	int length = f.tellg() / 4;
	f.seekg (0, f.beg);

	mRawData = new float[length];
	f.read((char*)mRawData, length*4);
	f.close();
	mWaveRenderer = new WaveRenderer(mRawData, length, p, 44100);
	mPixmap = mWaveRenderer->nextRender(0);

	mTimer = new QTimer(this);

	connect(
		mTimer,SIGNAL(timeout()),
		this, SLOT(portMove())
	);
}

void WavePort::paintEvent(QPaintEvent*) {

	QPainter painter(this);

	mMutex.lock();
	painter.setBrush(mPixmap);
	painter.drawRect(this->rect());
	mMutex.unlock();

}

void WavePort::cursorForward(int delta) {
	emit updateWave(delta);
}

void WavePort::cursorRewind(int delta) {
	emit updateWave(delta);
}

void WavePort::waveNextRender(int delta) {
	mMutex.lock();
	mPixmap = mWaveRenderer->nextRender(delta);
	mMutex.unlock();
	emit update();
}

void WavePort::mouseMoveEvent( QMouseEvent * event )
{
	if(!mDragging) {
		mDragging = true;
		mDragStart = QPoint(event->x(), event->y());
		mTimer->start(50);
	}
	mMutex.lock();
		mSpeed = (mDragStart.x() - event->x())/5;
	mMutex.unlock();
	//std::cout << (mSpeed) << std::endl;
}

void WavePort::mouseReleaseEvent(QMouseEvent *e) {
	mDragging = false;
	mTimer->stop();
}

void WavePort::portMove() {
	mMutex.lock();
	mPixmap = mWaveRenderer->nextRender(mSpeed);
	mMutex.unlock();
	emit update();
}

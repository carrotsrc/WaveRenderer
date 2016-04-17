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
	mWaveRenderer = new WaveRenderer(mRawData, length, p);
	mPixmap = mWaveRenderer->nextRender(0);
}

void WavePort::paintEvent(QPaintEvent*) {

	QPainter painter(this);
	//QPixmap pix( size() );
	//pix.fill( Qt::black );
	//painter.drawPixmap( 0, 0, pix );


	mMutex.lock();
	painter.setBrush(mPixmap);
	painter.drawRect(this->rect());
	mMutex.unlock();

}

void WavePort::cursorForward() {
	emit updateWave();
}

void WavePort::cursorRewind() {
	emit updateWave();
}

void WavePort::waveNextRender() {
	mMutex.lock();
	mPixmap = mWaveRenderer->nextRender(8);
	mMutex.unlock();
	emit update();
}

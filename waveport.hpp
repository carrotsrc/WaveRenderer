#ifndef WAVEPORT_HPP
#define WAVEPORT_HPP

#include <QWidget>
#include <QPixmap>
#include <QKeyEvent>
#include <QMutex>
#include "waverenderer.hpp"

class WavePort : public QWidget
{
	Q_OBJECT
public:
	explicit WavePort(QWidget *parent = 0);
	void paintEvent(QPaintEvent *);

signals:
	void updateWave();

public slots:
	void cursorForward();
	void cursorRewind();
	void waveNextRender();

private:
	WaveRenderer *mWaveRenderer;
	QPixmap mPixmap;
	float *mRawData;
	QMutex mMutex;
};

#endif // WAVEPORT_HPP

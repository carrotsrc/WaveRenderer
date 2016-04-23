#ifndef WAVEPORT_HPP
#define WAVEPORT_HPP

#include <QWidget>
#include <QPixmap>
#include <QKeyEvent>
#include <QMutex>
#include <QTimer>
#include <QMouseEvent>
#include <atomic>

#include "waverenderer.hpp"

class WavePort : public QWidget
{
	Q_OBJECT
public:
	explicit WavePort(QWidget *parent = 0);
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);
signals:
	void updateWave(int);

public slots:
	void cursorForward(int delta);
	void cursorRewind(int delta);
	void waveNextRender(int delta);

	void portMove();

private:
	WaveRenderer *mWaveRenderer;
	QPixmap mPixmap;
	QTimer *mTimer;
	float *mRawData;
	QMutex mMutex;
	QPoint mDragStart;
	bool mDragging;
	std::atomic_int mSpeed;
};

#endif // WAVEPORT_HPP

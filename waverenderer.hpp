#ifndef WAVEVIEWPORT_HPP
#define WAVEVIEWPORT_HPP
#include <memory>
#include <QWidget>
#include <QPixmap>


class WaveRenderer : public QWidget
{
	Q_OBJECT
public:
	explicit WaveRenderer(float* raw, int len, QPoint dim, int sampleRate, QWidget *parent = 0);

	QPixmap nextRender(int shift);
	void portForward(int blocks);
	void portRewind(int blocks);
signals:

public slots:

private:
	float* mRaw, mScale;
	int mRawLen, mCursor, mPortStart;
	unsigned int mBlockSize, mBufferLevel, mSampleRate, mBlockTime;
	std::uint32_t *mRingPort, mStart, mEnd, mRead, mWrite;
	QPoint mDimension;
	QPixmap mFill;

	void fillBlocks(int numBlocks);
};

#endif // WAVEVIEWPORT_HPP

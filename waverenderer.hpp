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

signals:

public slots:

private:
	float* mRaw, mScale;
	int mRawLen, mSampleStart, mLowerBound, mUpperBound;
	unsigned int mBlockSize, mBufferLevel, mSampleRate, mCentreBlock;
	std::uint32_t *mRingPort, mStart, mEnd, mWrite;
	QPoint mDimension;
	QPixmap mFill;
	int mLastDir;

	int fillBlocks(int numBlocks);
};

#endif // WAVEVIEWPORT_HPP

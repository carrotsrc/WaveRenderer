#include <iostream>

#include <QPainter>
#include "waverenderer.hpp"

WaveRenderer::WaveRenderer(float *raw, int len, QPoint dim, int sampleRate, QWidget *parent): QWidget(parent)
	, mRaw(raw), mRawLen(len), mSampleStart(0), mUpperBound(0), mSamplePlay(0)
	, mBufferLevel(0), mSampleRate(sampleRate)
	, mRingPort(nullptr), mStart(0), mEnd(0), mHead(0)
	, mDimension(dim), mFill(mDimension.x(), mDimension.y())
{

	// Each block is 2 ms
	mBlockSize = static_cast<unsigned int>( (static_cast<float>(mSampleRate) / 1000.0f) * 2.0f );
	mCentreBlock = mDimension.x()/2;
	mScale = mDimension.y() / 2.0f;
	mRingPort = new std::uint32_t[mDimension.x()];
	mFill.fill(QColor("#2F2F2F"));
	mDim = (mBlockSize * mCentreBlock);

	mLowerBound = (mSamplePlay - mDim)*2;
	mUpperBound = (mSamplePlay + mDim)*2;
	mSampleStart = mLowerBound;
	std::cout << mLowerBound << "/" << mUpperBound << std::endl;
	fillBlocks(mDimension.x());
}

QPixmap WaveRenderer::nextRender(int ms) {
	QPixmap graph(mDimension.x(), mDimension.y());

	QPainter painter(&graph);
	QPen pen(QColor("#ffffff"));

	auto mid = mDimension.y()/2;

	painter.drawPixmap(0,0,mFill);

	pen.setWidth(1);

	painter.setRenderHints(QPainter::HighQualityAntialiasing, true);
	painter.setPen(pen);

	auto dir = 1;
	if(ms != 0) {
		auto shift = ms / 2;
		dir = shift > 0 ? 1 : -1;

		mLowerBound += shift*mBlockSize;
		mUpperBound += shift*mBlockSize;
		mSamplePlay += shift*mBlockSize;


		mSampleStart = shift > 0 ? mUpperBound : mLowerBound;

		fillBlocks(shift);
	}

	auto bufferCursor = (mHead+0)%mDimension.x();

	for(auto block = 0; block < mDimension.x(); block++) {
		bufferCursor = bufferCursor % mDimension.x();
		auto val = mRingPort[bufferCursor++];

		auto yp = val & 0x0000ffff;
		auto yn = val >> 16;

		painter.drawLine(block, mid - yp, block,mid+yn);
	}

	painter.setPen(QPen(QColor("#d283ac")));
	painter.drawLine(mCentreBlock, 0, mCentreBlock,mDimension.y());
	return graph;
}


int WaveRenderer::fillBlocks(int numBlocks) {

	auto tblocks = mDimension.x();
	auto sampleCursor = mSampleStart;
	auto dir = numBlocks >= 0 ? 1 : -1;
	auto mid = tblocks/2;

	if(numBlocks < 0) numBlocks *= -1;

	for(auto block = 0; block < numBlocks; block++) {

		auto x = 0u;
		auto meanPos = 0.0f, meanNeg = 0.0f,
				xPos = 0.0f, xNeg = 0.0f;

		for(x = 0; x < mBlockSize; x++) {

			if(sampleCursor < 0 || sampleCursor >= mRawLen) {
				sampleCursor += (2*dir) * (mBlockSize-x);
				break;
			}

			auto v = mRaw[ sampleCursor ];

			if(v < 0) {
				meanNeg += v*-1.0;
				xNeg++;
			} else {
				meanPos += v;
				xPos++;
			}

			sampleCursor += (2*dir);
		}

		auto pi = static_cast<int>(xPos ? (meanPos/xPos) * mScale : 0);
		auto ni = static_cast<int>(xNeg ? (meanNeg/xNeg) * mScale : 0);

		auto final = (ni << 16) ^ pi;

		mRingPort[mHead] = final;

		if(mHead == 0) mHead = tblocks;
		mHead += dir;
		mHead %= tblocks;
	}

	return sampleCursor;

}

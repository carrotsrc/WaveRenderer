#include <iostream>

#include <QPainter>
#include "waverenderer.hpp"

WaveRenderer::WaveRenderer(float *raw, int len, QPoint dim, int sampleRate, QWidget *parent): QWidget(parent)
	, mRaw(raw), mRawLen(len), mSampleStart(0), mLowerBound(0), mUpperBound(0)
	, mBufferLevel(0), mSampleRate(sampleRate)
	, mRingPort(nullptr), mStart(0), mEnd(0), mWrite(0)
	, mDimension(dim), mFill(mDimension.x(), mDimension.y())
{

	// Each block is 2 ms
	mBlockSize = static_cast<unsigned int>( (static_cast<float>(mSampleRate) / 1000.0f) * 2.0f );
	mCentreBlock = mDimension.x()/2;
	mScale = mDimension.y() / 2.0f;
	std::cout << "mBlockSize: " << mBlockSize << std::endl;
	mRingPort = new std::uint32_t[mDimension.x()];
	mFill.fill(QColor("#2F2F2F"));

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

	if(mBufferLevel == 0) {
		for(auto x = 0; x < mDimension.x()/2; x++) {
			mRingPort[mWrite++] = 0;
			mBufferLevel++;
		}
		mUpperBound = fillBlocks(mDimension.x()/2);
	}

	if(ms != 0) {
		auto shift = ms / 2;

		mLowerBound += shift*mBlockSize;
		mUpperBound += shift*mBlockSize;

		mSampleStart = shift > 0 ? mUpperBound : mLowerBound;


		fillBlocks(shift);
	}

	auto cursor = (mWrite+1)%mDimension.x();
	for(auto block = 0; block < mDimension.x(); block++) {
		cursor = cursor % mDimension.x();
		auto val = mRingPort[cursor++];


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
	auto cursor = mSampleStart;
	auto dir = numBlocks >= 0 ? 1 : -1;


	for(auto block = 0; block < numBlocks; block++) {

		auto x = 0u;
		auto meanPos = 0.0f, meanNeg = 0.0f,
				xPos = 0.0f, xNeg = 0.0f;

		for(x = 0; x < mBlockSize; x++) {

			if(cursor < 0 || cursor >= mRawLen) break;

			auto v = mRaw[ cursor ];

			if(v < 0) {
				meanNeg += v*-1.0;
				xNeg++;
			} else {
				meanPos += v;
				xPos++;
			}

			cursor += 2;
		}

		auto pi = static_cast<int>(xPos ? (meanPos/xPos) * mScale : 0);
		auto ni = static_cast<int>(xNeg ? (meanNeg/xNeg) * mScale : 0);

		auto final = (ni << 16) ^ pi;

		mRingPort[mWrite] = final;

		mWrite++;

		mWrite %= tblocks;
	}

	return cursor;

}

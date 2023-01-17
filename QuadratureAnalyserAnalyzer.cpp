/* Copyright 2011 Dirk-Willem van Gulik, All Rights Reserved.
 *                dirkx(at)webweaving(dot)org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * $Id: QuadratureAnalyserAnalyzer.cpp 1037 2011-09-12 09:49:58Z dirkx $
 */
#include "QuadratureAnalyserAnalyzer.h"
#include "QuadratureAnalyserAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

static change_t qe_decode(unsigned char olda, unsigned char oldb, unsigned char newa, unsigned char newb)
{
	change_t oldnew[16] = {
		// AB -> AB  which is (old state) --> (new state)
		STANDSTILL, // 00 -> 00
		CLOCKWISE,	// 00 -> 01 -1
		COUNTERCW,	// 00 -> 10 +1
		GLITCH,		// 00 -> 11
		COUNTERCW,	// 01 -> 00 +1
		STANDSTILL, // 01 -> 01
		GLITCH,		// 01 -> 10
		CLOCKWISE,	// 01 -> 11 -1
		CLOCKWISE,	// 10 -> 00 -1
		GLITCH,		// 10 -> 01
		STANDSTILL, // 10 -> 10
		COUNTERCW,	// 10 -> 11 +1
		GLITCH,		// 11 -> 00
		COUNTERCW,	// 11 -> 01 +1
		CLOCKWISE,	// 11 -> 10 -1
		STANDSTILL, // 11 -> 11
	};
	unsigned char state = 0;
	state += olda ? 1 : 0;
	state += oldb ? 2 : 0;
	state += newa ? 4 : 0;
	state += newb ? 8 : 0;
	return oldnew[state];
}

QuadratureAnalyserAnalyzer::QuadratureAnalyserAnalyzer()
	: Analyzer(),
	  mSettings(new QuadratureAnalyserAnalyzerSettings())
{
	SetAnalyzerSettings(mSettings.get());
}

QuadratureAnalyserAnalyzer::~QuadratureAnalyserAnalyzer()
{
	KillThread();
}

void QuadratureAnalyserAnalyzer::WorkerThread()
{
	mResults.reset(new QuadratureAnalyserAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());

	if (mSettings->mInputChannelA != UNDEFINED_CHANNEL)
		mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannelA);
	if (mSettings->mInputChannelB != UNDEFINED_CHANNEL)
		mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannelB);
	if (mSettings->mInputChannelZ != UNDEFINED_CHANNEL)
		mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannelZ);

	AnalyzerChannelData *mChannelA = GetAnalyzerChannelData(mSettings->mInputChannelA);
	AnalyzerChannelData *mChannelB = GetAnalyzerChannelData(mSettings->mInputChannelB);
	AnalyzerChannelData *mChannelZ = GetAnalyzerChannelData(mSettings->mInputChannelZ);

	U8 mLastA = mChannelA->GetBitState() == BIT_HIGH ? 1 : 0;
	U8 mLastB = mChannelB->GetBitState() == BIT_HIGH ? 1 : 0;

	change_t lastDir = GLITCH;

	U64 lastStart = mChannelA->GetSampleNumber();
	U64 lastEnd = mChannelA->GetSampleNumber();
	U64 curEnd = mChannelA->GetSampleNumber();

	U32 glitchCount = 0;
	U32 tickCount = 0;
	int64_t posCount = 0;

	for (;;)
	{
		AnalyzerResults::MarkerType m;
		int result = 0;
		U64 a, b, c;
#if 0
		mChannelA->Advance(1);
		mChannelB->Advance(1);

		U8 mNewA = mChannelA->GetBitState() == BIT_HIGH ? 1 : 0;
		U8 mNewB = mChannelB->GetBitState() == BIT_HIGH ? 1 : 0;
#else
		a = mChannelA->GetSampleNumber();
		b = mChannelB->GetSampleNumber();
		if (a <= b)
		{
			mChannelA->AdvanceToNextEdge();
		};
		if (b <= a)
		{
			mChannelB->AdvanceToNextEdge();
		};
		a = mChannelA->GetSampleNumber();
		b = mChannelB->GetSampleNumber();
		U8 mNewA = mLastA;
		U8 mNewB = mLastB;
		if (a <= b)
		{
			mNewA = mChannelA->GetBitState() == BIT_HIGH ? 1 : 0;
			c = a;
		};
		if (b <= a)
		{
			mNewB = mChannelB->GetBitState() == BIT_HIGH ? 1 : 0;
			c = b;
		};
#endif

		change_t dir = qe_decode(mLastA, mLastB, mNewA, mNewB);
		mLastA = mNewA;
		mLastB = mNewB;
		if (dir == STANDSTILL)
			continue;

		switch (dir)
		{
		case CLOCKWISE:
		case COUNTERCW:
			m = (dir == CLOCKWISE) ? AnalyzerResults::DownArrow : AnalyzerResults::UpArrow;
			mResults->AddMarker(c, m, mSettings->mInputChannelA);
			mResults->AddMarker(c, m, mSettings->mInputChannelB);
			result++;
			tickCount++;
			posCount += (m == CLOCKWISE) ? -1 : 1;

			lastEnd = curEnd;
			curEnd = c;
			break;
		default:
			glitchCount++;
			break;
		};
		if (dir != lastDir)
		{
			// skip any initial glitches. We're not sure what they are anyway.
			//
			if (glitchCount != 0 || lastDir != GLITCH)
			{
				Frame frame;

				frame.mData1 = (posCount << 32) | lastDir;
				frame.mData2 = ((U64)tickCount << 32) | (glitchCount << 0);
				frame.mFlags = 0;

				frame.mStartingSampleInclusive = lastStart;
				frame.mEndingSampleInclusive = lastEnd;
				mResults->AddFrame(frame);
			}

			lastStart = curEnd;
			lastDir = dir;
			result++;
			tickCount = 0;
			glitchCount = 0;
		};

		if (result)
		{
			mResults->CommitResults();
			ReportProgress(c);
		};
	}
}

bool QuadratureAnalyserAnalyzer::NeedsRerun()
{
	return false;
}

U32 QuadratureAnalyserAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
	mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index,
														   device_sample_rate, simulation_channels);
}

U32 QuadratureAnalyserAnalyzer::GetMinimumSampleRateHz()
{
	return SCANRATE;
}

const char *QuadratureAnalyserAnalyzer::GetAnalyzerName() const
{
	return "Quadrature Decoder";
}

const char *GetAnalyzerName()
{
	// return MYVERSION;
	return "Quadrature Decoder";
}

Analyzer *CreateAnalyzer()
{
	return new QuadratureAnalyserAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
	delete analyzer;
}

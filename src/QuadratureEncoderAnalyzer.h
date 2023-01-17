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
 * $Id: QuadratureEncoderAnalyzer.h 1037 2011-09-12 09:49:58Z dirkx $
 */

#ifndef QUADRATUREENCODER_ANALYZER_H
#define QUADRATUREENCODER_ANALYZER_H

#include <Analyzer.h>
#include "QuadratureEncoderAnalyzerResults.h"
#include "QuadratureEncoderSimulationDataGenerator.h"

#define MYVERSION "QuadratureEncoder/1.02"

#define SCANRATE (100)
enum change_t { STANDSTILL = 0, CLOCKWISE, COUNTERCW, GLITCH };

class QuadratureEncoderAnalyzerSettings;
class ANALYZER_EXPORT QuadratureEncoderAnalyzer : public Analyzer
{
public:
	QuadratureEncoderAnalyzer();
	virtual ~QuadratureEncoderAnalyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::unique_ptr< QuadratureEncoderAnalyzerSettings > mSettings;
	std::unique_ptr< QuadratureEncoderAnalyzerResults > mResults;

	QuadratureEncoderSimulationDataGenerator mSimulationDataGenerator;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //QUADRATUREENCODER_ANALYZER_H

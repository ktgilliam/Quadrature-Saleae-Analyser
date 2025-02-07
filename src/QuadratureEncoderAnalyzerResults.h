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
 * $Id: QuadratureEncoderAnalyzerResults.h 1037 2011-09-12 09:49:58Z dirkx $
 */

#ifndef QUADRATUREENCODER_ANALYZER_RESULTS
#define QUADRATUREENCODER_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class QuadratureEncoderAnalyzer;
class QuadratureEncoderAnalyzerSettings;

class QuadratureEncoderAnalyzerResults : public AnalyzerResults
{
public:
	QuadratureEncoderAnalyzerResults( QuadratureEncoderAnalyzer* analyzer, QuadratureEncoderAnalyzerSettings* settings );
	virtual ~QuadratureEncoderAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	QuadratureEncoderAnalyzerSettings* mSettings;
	QuadratureEncoderAnalyzer* mAnalyzer;
};

#endif //QUADRATUREENCODER_ANALYZER_RESULTS

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
 * $Id: QuadratureEncoderAnalyzerSettings.cpp 1037 2011-09-12 09:49:58Z dirkx $
 */

#include <climits>

#include "QuadratureEncoderAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


QuadratureEncoderAnalyzerSettings::QuadratureEncoderAnalyzerSettings()
:	mInputChannelA( UNDEFINED_CHANNEL ),
	mInputChannelB( UNDEFINED_CHANNEL ),
	mInputChannelZ( UNDEFINED_CHANNEL ),
	ticksPerRotation( 0 )
{
	mInputChannelAInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelAInterface->SetTitleAndTooltip( "Quad A", "Standard Quadrature Decoder - input A (or left/cw/first)" );
	mInputChannelAInterface->SetChannel( mInputChannelA );

	mInputChannelBInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelBInterface->SetTitleAndTooltip( "Quad B", "Standard Quadrature Decoder - input Bi (or right/ccw/last)" );
	mInputChannelBInterface->SetChannel( mInputChannelB );

	mInputChannelZInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelZInterface->SetTitleAndTooltip( "Index Z", "Standard Quadrature Decoder - input Z (optional index)" );
	mInputChannelZInterface->SetChannel( mInputChannelZ );
	mInputChannelZInterface->SetSelectionOfNoneIsAllowed(true);

	mTicksPerRotationInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mTicksPerRotationInterface->SetTitleAndTooltip( "Counts Per Rotation (CPR)",  
		"Specify the number of changes per full revolution (or some other measure). Set to '0' to ignore - and not do speed/change calculations.");
	mTicksPerRotationInterface->SetMax( INT_MAX );
	mTicksPerRotationInterface->SetMin( 0 );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation);

	AddInterface( mInputChannelAInterface.get() );
	AddInterface( mInputChannelBInterface.get() );
	AddInterface( mInputChannelZInterface.get() );
	AddInterface( mTicksPerRotationInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannelA, "Quad A", false );
	AddChannel( mInputChannelB, "Quad B", false );
	AddChannel( mInputChannelZ, "Index Z", false );
}

QuadratureEncoderAnalyzerSettings::~QuadratureEncoderAnalyzerSettings()
{
}

bool QuadratureEncoderAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannelA = mInputChannelAInterface->GetChannel();
	mInputChannelB = mInputChannelBInterface->GetChannel();
	mInputChannelZ = mInputChannelZInterface->GetChannel();
	ticksPerRotation = mTicksPerRotationInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannelA, "Quad A", true);
	AddChannel( mInputChannelB, "Quad B", true);
	AddChannel( mInputChannelZ, "Index Z", true);

	return true;
}

void QuadratureEncoderAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelAInterface->SetChannel( mInputChannelA );
	mInputChannelBInterface->SetChannel( mInputChannelB );
	mInputChannelZInterface->SetChannel( mInputChannelZ );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation );
}

void QuadratureEncoderAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannelA;
	text_archive >> mInputChannelB;
	text_archive >> mInputChannelZ;
	text_archive >> ticksPerRotation;

	ClearChannels();
        AddChannel( mInputChannelA, "Quad A", true);
        AddChannel( mInputChannelB, "Quad B", true);
        AddChannel( mInputChannelZ, "Index Z", true);

	UpdateInterfacesFromSettings();
}

const char* QuadratureEncoderAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannelA;
	text_archive << mInputChannelB;
	text_archive << mInputChannelZ;
	text_archive << ticksPerRotation;

	return SetReturnString( text_archive.GetString() );
}

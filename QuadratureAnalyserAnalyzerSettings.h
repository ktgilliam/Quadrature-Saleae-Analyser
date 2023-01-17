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
 * $Id: QuadratureAnalyserAnalyzerSettings.h 1037 2011-09-12 09:49:58Z dirkx $
 */

#ifndef QUADRATUREANALYSER_ANALYZER_SETTINGS
#define QUADRATUREANALYSER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class QuadratureAnalyserAnalyzerSettings : public AnalyzerSettings
{
public:
	QuadratureAnalyserAnalyzerSettings();
	virtual ~QuadratureAnalyserAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings(const char *settings);
	virtual const char *SaveSettings();

	Channel mInputChannelA;
	Channel mInputChannelB;
	Channel mInputChannelZ;
	U32 ticksPerRotation;
	bool direction; // CW=true, CCW=false

protected:
	std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelAInterface;
	std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelBInterface;
	std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelZInterface;
	std::auto_ptr<AnalyzerSettingInterfaceInteger> mTicksPerRotationInterface;
	std::auto_ptr<AnalyzerSettingInterfaceBoolData> mDirectionInterface;
};

#endif // QUADRATUREANALYSER_ANALYZER_SETTINGS

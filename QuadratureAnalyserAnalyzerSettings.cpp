/* Modifications Copyright 2012 Henry von Tresckow (hvontres(at)gmail(dot)com

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
 * $Id: QuadratureAnalyserAnalyzerSettings.cpp 1037 2011-09-12 09:49:58Z dirkx $
 */

#include "QuadratureAnalyserAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <stdlib.h>
#include <stdio.h>



QuadratureAnalyserAnalyzerSettings::QuadratureAnalyserAnalyzerSettings()
:	mInputChannelA( UNDEFINED_CHANNEL ),
	mInputChannelB( UNDEFINED_CHANNEL ),
	ticksPerRotation( 0 )
{
	mInputChannelAInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelAInterface->SetTitleAndTooltip( "Quadrature A", "Standard Quadrature Decoder - input A (or left/cw/first)" );
	mInputChannelAInterface->SetChannel( mInputChannelA );

	mInputChannelBInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelBInterface->SetTitleAndTooltip( "Quadrature B", "Standard Quadrature Decoder - input Bi (or right/ccw/last)" );
	mInputChannelBInterface->SetChannel( mInputChannelB );

	mTicksPerRotationInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mTicksPerRotationInterface->SetTitleAndTooltip( "Impules/rotation",  
		"Specify the number of changes per full revolution (or some other measure). Set to '0' to ignore - and not do speed/change calculations.");
	mTicksPerRotationInterface->SetMax( 1e12 );
	mTicksPerRotationInterface->SetMin( 0 );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation);
	
	mTicksPerFrameInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mTicksPerFrameInterface->SetTitleAndTooltip( "Max Impules/Frame",  
		"Specify the maximum number of changes per Frame. Set to '0' to ignore - only use direction changes.");
	mTicksPerFrameInterface->SetMax( 1e12 );
	mTicksPerFrameInterface->SetMin( 0 );
	mTicksPerFrameInterface->SetInteger( ticksPerFrame);

	AddInterface( mInputChannelAInterface.get() );
	AddInterface( mInputChannelBInterface.get() );
	AddInterface( mTicksPerRotationInterface.get() );
	AddInterface( mTicksPerFrameInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannelA, "Quadrature A", false );
	AddChannel( mInputChannelB, "Quadrature B", false );
}

QuadratureAnalyserAnalyzerSettings::~QuadratureAnalyserAnalyzerSettings()
{
}

bool QuadratureAnalyserAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannelA = mInputChannelAInterface->GetChannel();
	mInputChannelB = mInputChannelBInterface->GetChannel();
	ticksPerRotation = mTicksPerRotationInterface->GetInteger();
	ticksPerFrame = mTicksPerFrameInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannelA, "Quadrature A", true);
	AddChannel( mInputChannelB, "Quadrature B", true);

	return true;
}

void QuadratureAnalyserAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelAInterface->SetChannel( mInputChannelA );
	mInputChannelBInterface->SetChannel( mInputChannelB );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation );
	mTicksPerFrameInterface->SetInteger( ticksPerFrame );
}

void QuadratureAnalyserAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannelA;
	text_archive >> mInputChannelB;
	text_archive >> ticksPerRotation;
	text_archive >> ticksPerFrame;

	ClearChannels();
        AddChannel( mInputChannelA, "Quadrature A", true);
        AddChannel( mInputChannelB, "Quadrature B", true);

	UpdateInterfacesFromSettings();
}

const char* QuadratureAnalyserAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannelA;
	text_archive << mInputChannelB;
	text_archive << ticksPerRotation;
	text_archive << ticksPerFrame;

	return SetReturnString( text_archive.GetString() );
}

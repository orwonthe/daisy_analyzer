#include "DaisyAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

DaisyAnalyzerSettings::DaisyAnalyzerSettings()
: mServoInChannel( UNDEFINED_CHANNEL ),
  mServoOutChannel( UNDEFINED_CHANNEL ),
  mConsoleInChannel( UNDEFINED_CHANNEL ),
  mConsoleOutChannel( UNDEFINED_CHANNEL ),
  mShiftClockChannel(UNDEFINED_CHANNEL ),
  mLoadClockChannel(UNDEFINED_CHANNEL ),
  mShiftOrder( AnalyzerEnums::MsbFirst ),
  mBitsPerTransfer( 24 ),
  mClockInactiveState( BIT_LOW ),
  mDataValidEdge( AnalyzerEnums::LeadingEdge ),
  mEnableActiveState( BIT_LOW )
{
	mServoInChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
  mServoInChannelInterface->SetTitleAndTooltip( "ServoIn", "Servo Data In" );
  mServoInChannelInterface->SetChannel( mServoInChannel );
  mServoInChannelInterface->SetSelectionOfNoneIsAllowed( true );

	mServoOutChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
  mServoOutChannelInterface->SetTitleAndTooltip( "ServoOut", "Servo Data Out" );
  mServoOutChannelInterface->SetChannel( mServoOutChannel );
  mServoOutChannelInterface->SetSelectionOfNoneIsAllowed( true );

	mConsoleInChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
  mConsoleInChannelInterface->SetTitleAndTooltip( "ConsoleIn", "Console Data In" );
  mConsoleInChannelInterface->SetChannel( mConsoleInChannel );
  mConsoleInChannelInterface->SetSelectionOfNoneIsAllowed( true );

	mConsoleOutChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
  mConsoleOutChannelInterface->SetTitleAndTooltip( "ConsoleOut", "Console Data Out" );
  mConsoleOutChannelInterface->SetChannel( mConsoleOutChannel );
  mConsoleOutChannelInterface->SetSelectionOfNoneIsAllowed( true );

	mShiftClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mShiftClockChannelInterface->SetTitleAndTooltip("Shift", "Shift Clock" );
	mShiftClockChannelInterface->SetChannel(mShiftClockChannel );

	mLoadClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mLoadClockChannelInterface->SetTitleAndTooltip("Load", "Load Clock" );
	mLoadClockChannelInterface->SetChannel(mLoadClockChannel );
	mLoadClockChannelInterface->SetSelectionOfNoneIsAllowed(true );

//	mShiftOrderInterface.reset( new AnalyzerSettingInterfaceNumberList() );
//	mShiftOrderInterface->SetTitleAndTooltip( "Significant Bit", "" );
//	mShiftOrderInterface->AddNumber( AnalyzerEnums::MsbFirst, "Most Significant Bit First (Standard)", "Select if the most significant bit or least significant bit is transmitted first" );
//	mShiftOrderInterface->AddNumber( AnalyzerEnums::LsbFirst, "Least Significant Bit First", "" );
//	mShiftOrderInterface->SetNumber( mShiftOrder );
//
//	mBitsPerTransferInterface.reset( new AnalyzerSettingInterfaceNumberList() );
//	mBitsPerTransferInterface->SetTitleAndTooltip( "Bits per Transfer", "" );
//	for( U32 i=1; i<=64; i++ )
//	{
//		std::stringstream ss;
//
//		if( i == 8 )
//			ss << "8 Bits per Transfer (Standard)";
//		else
//			ss << i << " Bits per Transfer";
//
//		mBitsPerTransferInterface->AddNumber( i, ss.str().c_str(), "" );
//	}
//	mBitsPerTransferInterface->SetNumber( mBitsPerTransfer );
//
//	mClockInactiveStateInterface.reset( new AnalyzerSettingInterfaceNumberList() );
//	mClockInactiveStateInterface->SetTitleAndTooltip( "Clock State", "" );
//	mClockInactiveStateInterface->AddNumber( BIT_LOW, "Clock is Low when inactive (CPOL = 0)", "CPOL = 0 (Clock Polarity)" );
//	mClockInactiveStateInterface->AddNumber( BIT_HIGH, "Clock is High when inactive (CPOL = 1)", "CPOL = 1 (Clock Polarity)" );
//	mClockInactiveStateInterface->SetNumber( mClockInactiveState );
//
//	mDataValidEdgeInterface.reset( new AnalyzerSettingInterfaceNumberList() );
//	mDataValidEdgeInterface->SetTitleAndTooltip( "Clock Phase", "" );
//	mDataValidEdgeInterface->AddNumber( AnalyzerEnums::LeadingEdge, "Data is Valid on Clock Leading Edge (CPHA = 0)", "CPHA = 0 (Clock Phase)" );
//	mDataValidEdgeInterface->AddNumber( AnalyzerEnums::TrailingEdge, "Data is Valid on Clock Trailing Edge (CPHA = 1)", "CPHA = 1 (Clock Phase)" );
//	mDataValidEdgeInterface->SetNumber( mDataValidEdge );
//
//	mEnableActiveStateInterface.reset( new AnalyzerSettingInterfaceNumberList() );
//	mEnableActiveStateInterface->SetTitleAndTooltip( "Enable Line", "" );
//	mEnableActiveStateInterface->AddNumber( BIT_LOW, "Enable line is Active Low (Standard)", "" );
//	mEnableActiveStateInterface->AddNumber( BIT_HIGH, "Enable line is Active High", "" );
//	mEnableActiveStateInterface->SetNumber( mEnableActiveState );


	AddInterface( mServoInChannelInterface.get() );
	AddInterface( mServoOutChannelInterface.get() );
	AddInterface( mConsoleInChannelInterface.get() );
	AddInterface( mConsoleOutChannelInterface.get() );
	AddInterface(mShiftClockChannelInterface.get() );
	AddInterface(mLoadClockChannelInterface.get() );
//	AddInterface( mShiftOrderInterface.get() );
//	AddInterface( mBitsPerTransferInterface.get() );
//	AddInterface( mClockInactiveStateInterface.get() );
//	AddInterface( mDataValidEdgeInterface.get() );
//	AddInterface( mEnableActiveStateInterface.get() );


	//AddExportOption( 0, "Export as text/csv file", "text (*.txt);;csv (*.csv)" );
	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mServoInChannel, "ServoIn", false );
	AddChannel( mServoOutChannel, "ServoOut", false );
	AddChannel( mConsoleInChannel, "ConsoleIn", false );
	AddChannel( mConsoleOutChannel, "ConsoleOut", false );
	AddChannel(mShiftClockChannel, "SHIFT", false );
	AddChannel(mLoadClockChannel, "LOAD", false );
}

DaisyAnalyzerSettings::~DaisyAnalyzerSettings()
{
}

bool DaisyAnalyzerSettings::SetSettingsFromInterfaces()
{
	Channel servoIn = mServoInChannelInterface->GetChannel();
	Channel servoOut = mServoOutChannelInterface->GetChannel();
	Channel consoleIn = mConsoleInChannelInterface->GetChannel();
	Channel consoleOut = mConsoleOutChannelInterface->GetChannel();
	Channel shiftClock = mShiftClockChannelInterface->GetChannel();
	Channel loadClock = mLoadClockChannelInterface->GetChannel();

	std::vector<Channel> channels;
	channels.push_back(servoIn );
	channels.push_back(servoOut );
	channels.push_back(consoleIn );
	channels.push_back(consoleOut );
	channels.push_back(shiftClock );
	channels.push_back(loadClock );

	if( AnalyzerHelpers::DoChannelsOverlap( &channels[0], channels.size() ) == true )
	{
		SetErrorText( "Please select different channels for each input." );
		return false;
	}

	if(
	    (servoIn == UNDEFINED_CHANNEL )
	    && (servoOut == UNDEFINED_CHANNEL )
	    && (consoleIn == UNDEFINED_CHANNEL )
	    && (consoleOut == UNDEFINED_CHANNEL )
	    )
	{
		SetErrorText(
		    "Please select at least one input for either ConsoleIn, ConsoleOut, ServoIn or ServoOut." );
		return false;
	}

	mServoInChannel = mServoInChannelInterface->GetChannel();
	mServoOutChannel = mServoOutChannelInterface->GetChannel();
	mConsoleInChannel = mConsoleInChannelInterface->GetChannel();
	mConsoleOutChannel = mConsoleOutChannelInterface->GetChannel();
  mShiftClockChannel = mShiftClockChannelInterface->GetChannel();
  mLoadClockChannel = mLoadClockChannelInterface->GetChannel();

//	mShiftOrder =			(AnalyzerEnums::ShiftOrder) U32( mShiftOrderInterface->GetNumber() );
//	mBitsPerTransfer =		U32( mBitsPerTransferInterface->GetNumber() );
//	mClockInactiveState =	(BitState) U32( mClockInactiveStateInterface->GetNumber() );
//	mDataValidEdge =		(AnalyzerEnums::Edge)  U32( mDataValidEdgeInterface->GetNumber() );
//	mEnableActiveState =	(BitState) U32( mEnableActiveStateInterface->GetNumber() );

	ClearChannels();
	AddChannel( mServoInChannel, "ServoIn", mServoInChannel != UNDEFINED_CHANNEL );
	AddChannel( mServoInChannel, "ServoOut", mServoOutChannel != UNDEFINED_CHANNEL );
	AddChannel( mConsoleInChannel, "ConsoleIn", mConsoleInChannel != UNDEFINED_CHANNEL );
	AddChannel( mConsoleOutChannel, "ConsoleOut", mConsoleOutChannel != UNDEFINED_CHANNEL );
	AddChannel(mShiftClockChannel, "SHIFT", mShiftClockChannel != UNDEFINED_CHANNEL );
	AddChannel(mLoadClockChannel, "LOAD", mLoadClockChannel != UNDEFINED_CHANNEL );

	return true;
}

void DaisyAnalyzerSettings::LoadSettings(const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	const char* name_string;	//the first thing in the archive is the name of the protocol analyzer that the data belongs to.
	text_archive >> &name_string;
	if( strcmp( name_string, "DaisyAnalyzer" ) != 0 )
		AnalyzerHelpers::Assert( "DaisyAnalyzer: Provided with a settings string that doesn't belong to us;" );

	text_archive >>  mServoInChannel;
	text_archive >>  mServoOutChannel;
	text_archive >>  mConsoleInChannel;
	text_archive >>  mConsoleOutChannel;
	text_archive >> mShiftClockChannel;
	text_archive >> mLoadClockChannel;
//	text_archive >>  *(U32*)&mShiftOrder;
//	text_archive >>  mBitsPerTransfer;
//	text_archive >>  *(U32*)&mClockInactiveState;
//	text_archive >>  *(U32*)&mDataValidEdge;
//	text_archive >>  *(U32*)&mEnableActiveState;

	//bool success = text_archive >> mUsePackets;  //new paramater added -- do this for backwards compatibility
	//if( success == false )
	//	mUsePackets = false; //if the archive fails, set the default value

	ClearChannels();
	AddChannel( mServoInChannel, "ServoIn", mServoInChannel != UNDEFINED_CHANNEL );
	AddChannel( mServoOutChannel, "ServoOut", mServoOutChannel != UNDEFINED_CHANNEL );
	AddChannel( mConsoleInChannel, "ConsoleIn", mConsoleInChannel != UNDEFINED_CHANNEL );
	AddChannel( mConsoleOutChannel, "ConsoleOut", mConsoleOutChannel != UNDEFINED_CHANNEL );
	AddChannel(mShiftClockChannel, "SHIFT", mShiftClockChannel != UNDEFINED_CHANNEL );
	AddChannel(mLoadClockChannel, "LOAD", mLoadClockChannel != UNDEFINED_CHANNEL );

	UpdateInterfacesFromSettings();
}

const char* DaisyAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << "DaisyAnalyzer";
	text_archive <<  mServoInChannel;
	text_archive <<  mServoOutChannel;
	text_archive <<  mConsoleInChannel;
	text_archive <<  mConsoleOutChannel;
	text_archive << mShiftClockChannel;
	text_archive << mLoadClockChannel;
	text_archive <<  mShiftOrder;
	text_archive <<  mBitsPerTransfer;
	text_archive <<  mClockInactiveState;
	text_archive <<  mDataValidEdge;
	text_archive <<  mEnableActiveState;

	return SetReturnString( text_archive.GetString() );
}

void DaisyAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mServoInChannelInterface->SetChannel( mServoInChannel );
	mServoOutChannelInterface->SetChannel( mServoOutChannel );
	mConsoleInChannelInterface->SetChannel( mConsoleInChannel );
	mConsoleOutChannelInterface->SetChannel( mConsoleOutChannel );
	mShiftClockChannelInterface->SetChannel(mShiftClockChannel );
	mLoadClockChannelInterface->SetChannel(mLoadClockChannel );
//	mShiftOrderInterface->SetNumber( mShiftOrder );
//	mBitsPerTransferInterface->SetNumber( mBitsPerTransfer );
//	mClockInactiveStateInterface->SetNumber( mClockInactiveState );
//	mDataValidEdgeInterface->SetNumber( mDataValidEdge );
//	mEnableActiveStateInterface->SetNumber( mEnableActiveState );
}

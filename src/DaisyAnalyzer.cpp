
#include "DaisyAnalyzer.h"
#include "DaisyAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

 
//enum SpiBubbleType { SpiData, SpiError };

DaisyAnalyzer::DaisyAnalyzer()
: Analyzer2(),
  mSettings( new DaisyAnalyzerSettings() ),
  mSimulationInitilized( false ),
  mServoIn( NULL ),
  mServoOut( NULL ),
  mConsoleIn( NULL ),
  mConsoleOut( NULL ),
  mShift(NULL ),
  mLoad(NULL )
{	
	SetAnalyzerSettings( mSettings.get() );
}

DaisyAnalyzer::~DaisyAnalyzer()
{
	KillThread();
}

void DaisyAnalyzer::SetupResults()
{
	mResults.reset( new DaisyAnalyzerResults(this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );

	if( mSettings->mServoInChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mServoInChannel );
	if( mSettings->mServoOutChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mServoOutChannel );
	if( mSettings->mConsoleInChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mConsoleInChannel );
	if( mSettings->mConsoleOutChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mConsoleOutChannel );
}

void DaisyAnalyzer::WorkerThread()
{
	Setup();

	AdvanceToActiveEnableEdgeWithCorrectClockPolarity();

	for( ; ; )
	{
		GetWord();
		CheckIfThreadShouldExit();
	}
}

void DaisyAnalyzer::AdvanceToActiveEnableEdgeWithCorrectClockPolarity()
{
	mResults->CommitPacketAndStartNewPacket();
	mResults->CommitResults();
	
	AdvanceToActiveEnableEdge();

	for( ; ; )
	{
		if( IsInitialClockPolarityCorrect() == true )  //if false, this function moves to the next active enable edge.
			break;
	}
}

void DaisyAnalyzer::Setup()
{
	bool allow_last_trailing_clock_edge_to_fall_outside_enable = false;
	if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
		allow_last_trailing_clock_edge_to_fall_outside_enable = true;

	if( mSettings->mClockInactiveState == BIT_LOW )
	{
		if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
			mArrowMarker = AnalyzerResults::UpArrow;
		else
			mArrowMarker = AnalyzerResults::DownArrow;

	}else
	{
		if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
			mArrowMarker = AnalyzerResults::DownArrow;
		else
			mArrowMarker = AnalyzerResults::UpArrow;
	}


	if( mSettings->mServoInChannel != UNDEFINED_CHANNEL )
		mServoIn = GetAnalyzerChannelData( mSettings->mServoInChannel );
	else
		mServoIn = NULL;

	if( mSettings->mServoOutChannel != UNDEFINED_CHANNEL )
		mServoOut = GetAnalyzerChannelData( mSettings->mServoOutChannel );
	else
		mServoOut = NULL;

	if( mSettings->mConsoleInChannel != UNDEFINED_CHANNEL )
		mConsoleIn = GetAnalyzerChannelData( mSettings->mConsoleInChannel );
	else
		mConsoleIn = NULL;

	if( mSettings->mConsoleOutChannel != UNDEFINED_CHANNEL )
		mConsoleOut = GetAnalyzerChannelData( mSettings->mConsoleOutChannel );
	else
		mConsoleOut = NULL;


  mShift = GetAnalyzerChannelData(mSettings->mShiftClockChannel );

	if(mSettings->mLoadClockChannel != UNDEFINED_CHANNEL )
    mLoad = GetAnalyzerChannelData(mSettings->mLoadClockChannel );
	else
    mLoad = NULL;

}

void DaisyAnalyzer::AdvanceToActiveEnableEdge()
{
	if(mLoad != NULL )
	{
		if(mLoad->GetBitState() != mSettings->mEnableActiveState )
		{
			mLoad->AdvanceToNextEdge();
		}else
		{
			mLoad->AdvanceToNextEdge();
			mLoad->AdvanceToNextEdge();
		}
		mCurrentSample = mLoad->GetSampleNumber();
		mShift->AdvanceToAbsPosition(mCurrentSample );
	}else
	{
		mCurrentSample = mShift->GetSampleNumber();
	}
}

bool DaisyAnalyzer::IsInitialClockPolarityCorrect()
{
	if(mShift->GetBitState() == mSettings->mClockInactiveState )
		return true;

	mResults->AddMarker( mCurrentSample, AnalyzerResults::ErrorSquare, mSettings->mShiftClockChannel );

	if(mLoad != NULL )
	{
		Frame error_frame;
		error_frame.mStartingSampleInclusive = mCurrentSample;

		mLoad->AdvanceToNextEdge();
		mCurrentSample = mLoad->GetSampleNumber();

		error_frame.mEndingSampleInclusive = mCurrentSample;
		error_frame.mFlags = SPI_ERROR_FLAG | DISPLAY_AS_ERROR_FLAG;
		mResults->AddFrame( error_frame );
		mResults->CommitResults();
		ReportProgress( error_frame.mEndingSampleInclusive );

		//move to the next active-going enable edge
		mLoad->AdvanceToNextEdge();
		mCurrentSample = mLoad->GetSampleNumber();
		mShift->AdvanceToAbsPosition(mCurrentSample );

		return false;
	}else
	{
		mShift->AdvanceToNextEdge();  //at least start with the clock in the idle state.
		mCurrentSample = mShift->GetSampleNumber();
		return true;
	}
}

bool DaisyAnalyzer::WouldAdvancingTheClockToggleEnable()
{
	if(mLoad == NULL )
		return false;

	U64 next_edge = mShift->GetSampleOfNextEdge();
	bool enable_will_toggle = mLoad->WouldAdvancingToAbsPositionCauseTransition(next_edge );

	if( enable_will_toggle == false )
		return false;
	else
		return true;
}

void DaisyAnalyzer::GetWord()
{
	//we're assuming we come into this function with the clock in the idle state;

	U32 bits_per_transfer = mSettings->mBitsPerTransfer;

	DataBuilder servo_in_result;
	U64 servo_in_word = 0;
  servo_in_result.Reset( &servo_in_word, mSettings->mShiftOrder, bits_per_transfer );

	DataBuilder servo_out_result;
	U64 servo_out_word = 0;
  servo_out_result.Reset( &servo_out_word, mSettings->mShiftOrder, bits_per_transfer );

	DataBuilder console_in_result;
	U64 console_in_word = 0;
	console_in_result.Reset(&console_in_word, mSettings->mShiftOrder, bits_per_transfer );

	DataBuilder console_out_result;
	U64 console_out_word = 0;
	console_out_result.Reset(&console_out_word, mSettings->mShiftOrder, bits_per_transfer );

	U64 first_sample = 0;
	bool need_reset = false;

	mArrowLocations.clear();
	ReportProgress(mShift->GetSampleNumber() );

	for( U32 i=0; i<bits_per_transfer; i++ )
	{
		if( i == 0 )
			CheckIfThreadShouldExit();

		//on every single edge, we need to check that enable doesn't toggle.
		//note that we can't just advance the enable line to the next edge, becuase there may not be another edge

		if( WouldAdvancingTheClockToggleEnable() == true )
		{
			AdvanceToActiveEnableEdgeWithCorrectClockPolarity();  //ok, we pretty much need to reset everything and return.
			return;
		}
		
		mShift->AdvanceToNextEdge();
		if( i == 0 )
			first_sample = mShift->GetSampleNumber();

		if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
		{
			mCurrentSample = mShift->GetSampleNumber();
			if( mServoIn != NULL )
			{
				mServoIn->AdvanceToAbsPosition( mCurrentSample );
				servo_in_result.AddBit( mServoIn->GetBitState() );
			}
			if( mServoOut != NULL )
			{
				mServoOut->AdvanceToAbsPosition( mCurrentSample );
				servo_out_result.AddBit( mServoOut->GetBitState() );
			}
			if( mConsoleIn != NULL )
			{
				mConsoleIn->AdvanceToAbsPosition( mCurrentSample );
				console_in_result.AddBit(mConsoleIn->GetBitState() );
			}
			if( mConsoleOut != NULL )
			{
				mConsoleOut->AdvanceToAbsPosition( mCurrentSample );
				console_out_result.AddBit(mConsoleOut->GetBitState() );
			}
			mArrowLocations.push_back( mCurrentSample );
		}


		// ok, the trailing edge is messy -- but only on the very last bit.
		// If the trialing edge isn't doesn't represent valid data, we want to allow the enable line to rise before the clock trialing edge -- and still report the frame
		if( ( i == ( bits_per_transfer - 1 ) ) && ( mSettings->mDataValidEdge != AnalyzerEnums::TrailingEdge ) )
		{
			//if this is the last bit, and the trailing edge doesn't represent valid data
			if( WouldAdvancingTheClockToggleEnable() == true )
			{
				//moving to the trailing edge would cause the clock to revert to inactive.  jump out, record the frame, and them move to the next active enable edge
				need_reset = true;
				break;
			}
			
			//enable isn't going to go inactive, go ahead and advance the clock as usual.  Then we're done, jump out and record the frame.
			mShift->AdvanceToNextEdge();
			break;
		}
		
		//this isn't the very last bit, etc, so proceed as normal
		if( WouldAdvancingTheClockToggleEnable() == true )
		{
			AdvanceToActiveEnableEdgeWithCorrectClockPolarity();  //ok, we pretty much need to reset everything and return.
			return;
		}

		mShift->AdvanceToNextEdge();

		if( mSettings->mDataValidEdge == AnalyzerEnums::TrailingEdge )
		{
			mCurrentSample = mShift->GetSampleNumber();
			if( mServoIn != NULL )
			{
				mServoIn->AdvanceToAbsPosition( mCurrentSample );
				servo_in_result.AddBit( mServoIn->GetBitState() );
			}
			if( mServoOut != NULL )
			{
				mServoOut->AdvanceToAbsPosition( mCurrentSample );
				servo_out_result.AddBit( mServoOut->GetBitState() );
			}
			if( mConsoleIn != NULL )
			{
				mConsoleIn->AdvanceToAbsPosition( mCurrentSample );
				console_in_result.AddBit(mConsoleIn->GetBitState() );
			}
			if( mConsoleOut != NULL )
			{
				mConsoleOut->AdvanceToAbsPosition( mCurrentSample );
				console_out_result.AddBit(mConsoleOut->GetBitState() );
			}
			mArrowLocations.push_back( mCurrentSample );
		}
		
	}

	//save the resuls:
	U32 count = mArrowLocations.size();
	for( U32 i=0; i<count; i++ )
		mResults->AddMarker( mArrowLocations[i], mArrowMarker, mSettings->mShiftClockChannel );

	Frame result_frame;
	result_frame.mStartingSampleInclusive = first_sample;
	result_frame.mEndingSampleInclusive = mShift->GetSampleNumber();
	result_frame.mData1 = (servo_in_word << 32) | servo_out_word;
	result_frame.mData2 = (console_in_word << 32) | console_out_word;;
	result_frame.mFlags = 0;
	mResults->AddFrame( result_frame );
	
	mResults->CommitResults();

	if( need_reset == true )
		AdvanceToActiveEnableEdgeWithCorrectClockPolarity();
}

bool DaisyAnalyzer::NeedsRerun()
{
	return false;
}

U32 DaisyAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}


U32 DaisyAnalyzer::GetMinimumSampleRateHz()
{
	return 10000; //we don't have any idea, depends on the SPI rate, etc.; return the lowest rate.
}

const char* DaisyAnalyzer::GetAnalyzerName() const
{
	return "Daisy";
}

const char* GetAnalyzerName()
{
	return "Daisy";
}

Analyzer* CreateAnalyzer()
{
	return new DaisyAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}

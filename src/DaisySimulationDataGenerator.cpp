#include "DaisySimulationDataGenerator.h"
#include "DaisyAnalyzerSettings.h"

DaisySimulationDataGenerator::DaisySimulationDataGenerator()
{
}

DaisySimulationDataGenerator::~DaisySimulationDataGenerator()
{
}

void DaisySimulationDataGenerator::Initialize(U32 simulation_sample_rate, DaisyAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mClockGenerator.Init( simulation_sample_rate / 10, simulation_sample_rate );

	if( settings->mConsoleChannel != UNDEFINED_CHANNEL )
		mConsole = mSpiSimulationChannels.Add( settings->mConsoleChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mConsole = NULL;
	
	if( settings->mServoChannel != UNDEFINED_CHANNEL )
		mServo = mSpiSimulationChannels.Add( settings->mServoChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mServo = NULL;

	mClock = mSpiSimulationChannels.Add(settings->mShiftClockChannel, mSimulationSampleRateHz, mSettings->mClockInactiveState );

	if(settings->mLoadClockChannel != UNDEFINED_CHANNEL )
		mEnable = mSpiSimulationChannels.Add(settings->mLoadClockChannel, mSimulationSampleRateHz, Invert(mSettings->mEnableActiveState ) );
	else
		mEnable = NULL;

	mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle

	mValue = 0;
}

U32 DaisySimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mClock->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateSpiTransaction();

		mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle
	}

	*simulation_channels = mSpiSimulationChannels.GetArray();
	return mSpiSimulationChannels.GetCount();
}

void DaisySimulationDataGenerator::CreateSpiTransaction()
{
	if( mEnable != NULL )
		mEnable->Transition();

	mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
	
	if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
	{
		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		if( mEnable != NULL )
			mEnable->Transition();
	
		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;
	}else
	{
		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		if( mEnable != NULL )
			mEnable->Transition();
	
		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;
	}
}

void DaisySimulationDataGenerator::OutputWord_CPHA0(U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );

	U32 count = mSettings->mBitsPerTransfer;
	for( U32 i=0; i<count; i++ )
	{
		if( mServo != NULL )
			mServo->TransitionIfNeeded( mosi_bits.GetNextBit() );

		if( mConsole != NULL )
			mConsole->TransitionIfNeeded( miso_bits.GetNextBit() );

		mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data invalid
	}

	if( mServo != NULL )
		mServo->TransitionIfNeeded( BIT_LOW );

	if( mConsole != NULL )
		mConsole->TransitionIfNeeded( BIT_LOW );

	mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}

void DaisySimulationDataGenerator::OutputWord_CPHA1(U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );

	U32 count = mSettings->mBitsPerTransfer;
	for( U32 i=0; i<count; i++ )
	{
		mClock->Transition();  //data invalid
		if( mServo != NULL )
			mServo->TransitionIfNeeded( mosi_bits.GetNextBit() );
		if( mConsole != NULL )
			mConsole->TransitionIfNeeded( miso_bits.GetNextBit() );

		mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
	}

	if( mServo != NULL )
		mServo->TransitionIfNeeded( BIT_LOW );
	if( mConsole != NULL )
		mConsole->TransitionIfNeeded( BIT_LOW );

	mSpiSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}

#include "DaisySimulationDataGenerator.h"
#include "DaisyAnalyzerSettings.h"

DaisySimulationDataGenerator::DaisySimulationDataGenerator()
{
  mValue = 1;
}

DaisySimulationDataGenerator::~DaisySimulationDataGenerator()
{
}

void DaisySimulationDataGenerator::Initialize(U32 simulation_sample_rate, DaisyAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mClockGenerator.Init( simulation_sample_rate / 10, simulation_sample_rate );

	if( settings->mConsoleInChannel != UNDEFINED_CHANNEL )
		mConsoleIn = mDaisySimulationChannels.Add(settings->mConsoleInChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mConsoleIn = NULL;
	
	if( settings->mConsoleOutChannel != UNDEFINED_CHANNEL )
		mConsoleOut = mDaisySimulationChannels.Add(settings->mConsoleOutChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mConsoleOut = NULL;

	if( settings->mServoInChannel != UNDEFINED_CHANNEL )
		mServoIn = mDaisySimulationChannels.Add(settings->mServoInChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mServoIn = NULL;

	if( settings->mServoOutChannel != UNDEFINED_CHANNEL )
		mServoOut = mDaisySimulationChannels.Add(settings->mServoOutChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mServoOut = NULL;

  mShift = mDaisySimulationChannels.Add(settings->mShiftClockChannel, mSimulationSampleRateHz, mSettings->mClockInactiveState );

	if(settings->mLoadClockChannel != UNDEFINED_CHANNEL )
    mLoad = mDaisySimulationChannels.Add(settings->mLoadClockChannel, mSimulationSampleRateHz, Invert(mSettings->mEnableActiveState ) );
	else
    mLoad = NULL;

	mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(10.0 ) ); //insert 10 bit-periods of idle

	mValue = 0;
}

U32 DaisySimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while(mShift->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
    CreateDaisyTransaction();

		mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(10.0 ) ); //insert 10 bit-periods of idle
	}

	*simulation_channels = mDaisySimulationChannels.GetArray();
	return mDaisySimulationChannels.GetCount();
}

void DaisySimulationDataGenerator::CreateDaisyTransaction()
{
	if(mLoad != NULL )
		mLoad->Transition();

	mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(2.0 ) );

	bool leading = mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge;

  if(mLoad != NULL )
    mLoad->Transition();

  if(mLoad != NULL )
    mLoad->Transition();

  OutputWord_Data(mValue, mValue+1, mValue+2, mValue+3, leading);
  mValue += 4;
  OutputWord_Data(mValue, mValue+1, mValue+2, mValue+3, leading);
  mValue += 4;
  OutputWord_Data(mValue, mValue+1, mValue+2, mValue+3, leading);
  mValue += 4;
  OutputWord_Data(mValue, mValue+1, mValue+2, mValue+3, leading);
  mValue += 4;

}

//void DaisySimulationDataGenerator::OutputWord_CPHA0(U64 mosi_data, U64 miso_data )
//{
//	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
//	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
//
//	U32 count = mSettings->mBitsPerTransfer;
//	for( U32 i=0; i<count; i++ )
//	{
//		if( mServo != NULL )
//			mServo->TransitionIfNeeded( mosi_bits.GetNextBit() );
//
//		if( mConsole != NULL )
//			mConsole->TransitionIfNeeded( miso_bits.GetNextBit() );
//
//		mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5 ) );
//		mClock->Transition();  //data valid
//
//		mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5 ) );
//		mClock->Transition();  //data invalid
//	}
//
//	if( mServo != NULL )
//		mServo->TransitionIfNeeded( BIT_LOW );
//
//	if( mConsole != NULL )
//		mConsole->TransitionIfNeeded( BIT_LOW );
//
//	mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(2.0 ) );
//}

void DaisySimulationDataGenerator::OutputWord_Data(
    U64 servo_in_data,
    U64 servo_out_data,
    U64 console_in_data,
    U64 console_out_data,
    bool leading)
{
	BitExtractor servo_in_bits( servo_in_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor servo_out_bits( servo_out_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor console_in_bits( console_in_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor console_out_bits( console_out_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );

	U32 count = mSettings->mBitsPerTransfer;
	for( U32 i=0; i<count; i++ )
	{
		if(!leading)
		  mShift->Transition();  //data invalid
		if( mServoIn != NULL )
			mServoIn->TransitionIfNeeded( servo_in_bits.GetNextBit() );
		if( mServoOut != NULL )
			mServoOut->TransitionIfNeeded( servo_out_bits.GetNextBit() );
		if( mConsoleIn != NULL )
			mConsoleIn->TransitionIfNeeded( console_in_bits.GetNextBit() );
		if( mConsoleOut != NULL )
			mConsoleOut->TransitionIfNeeded( console_out_bits.GetNextBit() );

		mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5 ) );
    if(leading)
      mShift->Transition();  //data invalid
		mShift->Transition();  //data valid

		mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5 ) );
	}

	if( mServoIn != NULL )
		mServoIn->TransitionIfNeeded( BIT_LOW );
	if( mServoOut != NULL )
		mServoOut->TransitionIfNeeded( BIT_LOW );
	if( mConsoleIn != NULL )
		mConsoleIn->TransitionIfNeeded( BIT_LOW );
	if( mConsoleOut != NULL )
		mConsoleOut->TransitionIfNeeded( BIT_LOW );

	mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(2.0 ) );
}

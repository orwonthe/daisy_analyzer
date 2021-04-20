
#include "DaisyAnalyzer.h"
#include "DaisyAnalyzerSettings.h"
#include "AnalyzerChannelDataHolder.h"
#include <AnalyzerChannelData.h>

 
//enum SpiBubbleType { SpiData, SpiError };

DaisyAnalyzer::DaisyAnalyzer()
: Analyzer2(),
  mSettings( new DaisyAnalyzerSettings(mChannelizerManager.get()) ),
  mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

DaisyAnalyzer::~DaisyAnalyzer()
{
	KillThread();
  deleteSignalGrabbers();
}

void DaisyAnalyzer::SetupResults()
{
	mResults.reset( new DaisyAnalyzerResults(this, mSettings.get() , mChannelizerManager.get()) );
	SetAnalyzerResults( mResults.get() );
  mChannelizerManager->markDataChannelsAsBubbleWorthy(mResults.get());
}

void DaisyAnalyzer::WorkerThread()
{
  setup();

  startNewPacket();
  advanceToLoadEventEnd();

	for( ; ; )
	{
    getWord();
		CheckIfThreadShouldExit();
	}
}


void DaisyAnalyzer::setup()
{
  resetClockers();
  resetSignalGrabbers();
}

void DaisyAnalyzer::resetSignalGrabbers() {
  deleteSignalGrabbers();
  std::vector<DataChannelizer *> dataChannelizers;
  mChannelizerManager->grabDefinedDataChannels(dataChannelizers);
  for(DataChannelizer* channelizer: dataChannelizers) {
    mSignalGrabbers.push_back(channelizer->createSignalGrabber(this));
  }
}

void DaisyAnalyzer::resetClockers() {
  mLoadClocker.reset(mChannelizerManager->createLoadClocker(this));
  mShiftClocker.reset(mChannelizerManager->createShiftClocker(this));
}

void DaisyAnalyzer::deleteSignalGrabbers() {
  for (SignalGrabber* grabber: mSignalGrabbers) {
    delete grabber;
  }
  mSignalGrabbers.clear();
}

void DaisyAnalyzer::getWord()
{
	U32 bits_per_transfer = 24;
	U64 first_sample = 0;
	bool need_reset = false;

	mArrowLocations.clear();
	ReportProgress(mCurrentSample );

	CheckIfThreadShouldExit();
	for(U32 bit_index=0; bit_index < bits_per_transfer; bit_index++ )
	{
	  advanceToShiftEventEnd();
	  if(bit_index == 0) {
	    first_sample = mCurrentSample;
	  }
	  captureCurrentDataSignals();
	}

//	//save the resuls:
//	U32 count = mArrowLocations.size();
//	for( U32 i=0; i<count; i++ )
//		mResults->AddMarker( mArrowLocations[i], mArrowMarker, mSettings->mShiftClockChannel );

	Frame result_frame;
	result_frame.mStartingSampleInclusive = first_sample;
	result_frame.mEndingSampleInclusive = mCurrentSample;
	result_frame.mData1 =0;
	result_frame.mData2 = 0;
	result_frame.mFlags = 0;
  loadFrameWithSignalData(result_frame);
	mResults->AddFrame( result_frame );
	
	mResults->CommitResults();
}

void DaisyAnalyzer::startNewPacket() {
  mResults->CommitPacketAndStartNewPacket();
  mResults->CommitResults();
}

void DaisyAnalyzer::advanceToLoadEventEnd()
{
  mCurrentSample = mLoadClocker->advanceToMarkerEnd();
  syncAllChannels();
}

void DaisyAnalyzer::advanceToShiftEventEnd() {
  mCurrentSample = mShiftClocker->advanceToMarkerEnd();
  syncAllChannels();
}

void DaisyAnalyzer::syncAllChannels() {
  mLoadClocker->advanceToAbsolutePosition(mCurrentSample);
  mShiftClocker->advanceToAbsolutePosition(mCurrentSample);
  for(SignalGrabber* grabber: mSignalGrabbers) {
    grabber->advanceToAbsolutePosition(mCurrentSample);
  }
}

void DaisyAnalyzer::captureCurrentDataSignals() {
  for(SignalGrabber* grabber: mSignalGrabbers) {
    grabber->grab();
  }
}
void DaisyAnalyzer::loadFrameWithSignalData(Frame &frame) {
  for(SignalGrabber* grabber: mSignalGrabbers) {
    grabber->stash(frame);
  }
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

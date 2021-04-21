#include "DaisySimulationDataGenerator.h"
#include "DaisyAnalyzerSettings.h"

DaisySimulationDataGenerator::DaisySimulationDataGenerator(DaisyChannelizerManager *channelizerManager)
    : mChannelizerManager(channelizerManager), mShiftSimulator(nullptr), mLoadSimulator(nullptr), mSettings(nullptr),
      mSimulationSampleRateHz(10000) {
}

DaisySimulationDataGenerator::~DaisySimulationDataGenerator() { deleteBitGenerators(); }

void DaisySimulationDataGenerator::Initialize(U32 simulation_sample_rate, DaisyAnalyzerSettings *settings) {
  deleteBitGenerators();
  mSimulationSampleRateHz = simulation_sample_rate;
  mSettings = settings;

  mClockGenerator.Init(simulation_sample_rate / 10.0, simulation_sample_rate);
  mLoadSimulator = mChannelizerManager->addLoadSimulationChannelDescriptor(mDaisySimulationChannels,
                                                                           simulation_sample_rate, BIT_HIGH);
  mShiftSimulator = mChannelizerManager->addShiftSimulationChannelDescriptor(mDaisySimulationChannels,
                                                                             simulation_sample_rate, BIT_HIGH);

  U32 startValue = 0x00010203;
  U32 deltaValue = 0x00010305;

  for (DataChannelizer *channelizer: mChannelizerManager->definedDataChannels()) {
    SimulationChannelDescriptor *descriptor = channelizer->addSimulationChannelDescriptor(mDaisySimulationChannels,
                                                                                          simulation_sample_rate,
                                                                                          BIT_LOW);
    mDataSimulators.push_back(descriptor);
    mBitGenerators.push_back(new BitGenerator(startValue, deltaValue, 24));
    startValue += 0x00101010;
    deltaValue += 0x00020202;
  }
  advanceHalfPeriod(10.0);
}

U32 DaisySimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate,
                                                         SimulationChannelDescriptor **simulation_channels) {
  U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested,
                                                                                        sample_rate,
                                                                                        mSimulationSampleRateHz);

  while (mShiftSimulator->GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
    createDaisyTransaction();
    advanceHalfPeriod(10.0);
  }

  *simulation_channels = mDaisySimulationChannels.GetArray();
  return mDaisySimulationChannels.GetCount();
}

void DaisySimulationDataGenerator::createDaisyTransaction() {
  loadCycle();
  for (int bit_index = 0; bit_index < 24; bit_index++) {
    shiftCycle();
  }
}

void DaisySimulationDataGenerator::loadCycle() {
  mLoadSimulator->Transition();
  advanceHalfPeriod();
  mLoadSimulator->Transition();
  advanceHalfPeriod();
}

void DaisySimulationDataGenerator::shiftCycle() {
  mShiftSimulator->Transition();
  for (int dataChannelIndex = 0; dataChannelIndex < mDataSimulators.size(); dataChannelIndex++) {
    mDataSimulators[dataChannelIndex]->TransitionIfNeeded(mBitGenerators[dataChannelIndex]->nextBit());
  }
  advanceHalfPeriod();
  mShiftSimulator->Transition();
  advanceHalfPeriod();
}

void DaisySimulationDataGenerator::advanceHalfPeriod(double multiple) {
  mDaisySimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(multiple)); //insert 10 bit-periods of idle
}

void DaisySimulationDataGenerator::deleteBitGenerators() {
  for (BitGenerator *generator: mBitGenerators) {
    delete generator;
  }
  mBitGenerators.clear();
}


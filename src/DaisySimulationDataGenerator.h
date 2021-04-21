#ifndef SPI_SIMULATION_DATA_GENERATOR
#define SPI_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>
#include "DaisyChannelizerManager.h"
#include "BitGenerator.h"

class DaisyAnalyzerSettings;

class DaisySimulationDataGenerator
{
public:
	explicit DaisySimulationDataGenerator(DaisyChannelizerManager* channelizerManager);
	~DaisySimulationDataGenerator();

	void Initialize(U32 simulation_sample_rate, DaisyAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

protected:
  void advanceHalfPeriod(double multiple = 1.0);
  void loadCycle();
  void shiftCycle();
  void deleteBitGenerators();
	DaisyAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	ClockGenerator mClockGenerator;

	void createDaisyTransaction();

  DaisyChannelizerManager* mChannelizerManager;
	SimulationChannelDescriptorGroup mDaisySimulationChannels;
  SimulationChannelDescriptor* mShiftSimulator;
	SimulationChannelDescriptor* mLoadSimulator;
	std::vector<SimulationChannelDescriptor*> mDataSimulators;
	std::vector<BitGenerator*> mBitGenerators;
};
#endif //SPI_SIMULATION_DATA_GENERATOR

#ifndef SPI_SIMULATION_DATA_GENERATOR
#define SPI_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class DaisyAnalyzerSettings;

class DaisySimulationDataGenerator
{
public:
	DaisySimulationDataGenerator();
	~DaisySimulationDataGenerator();

	void Initialize(U32 simulation_sample_rate, DaisyAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

protected:
	DaisyAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	U64 mValue;

protected: //SPI specific
	ClockGenerator mClockGenerator;

	void CreateDaisyTransaction();
	void OutputWord_Data(
	    U64 servo_in_data,
	    U64 servo_out_data,
	    U64 console_in_data,
	    U64 console_out_data,
	    bool leading);


	SimulationChannelDescriptorGroup mDaisySimulationChannels;
	SimulationChannelDescriptor* mServoIn;
  SimulationChannelDescriptor* mServoOut;
  SimulationChannelDescriptor* mConsoleIn;
  SimulationChannelDescriptor* mConsoleOut;
  SimulationChannelDescriptor* mShift;
	SimulationChannelDescriptor* mLoad;
};
#endif //SPI_SIMULATION_DATA_GENERATOR

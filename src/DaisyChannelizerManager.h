//
// Created by willy on 4/20/21.
//

#ifndef DAISY_ANALYZER_DAISY_CHANNELIZER_MANAGER_H
#define DAISY_ANALYZER_DAISY_CHANNELIZER_MANAGER_H

#include "MasterChannelizerManager.h"

class DaisyChannelizerManager : public MasterChannelizerManager {
public:
  DaisyChannelizerManager();
  Clocker* createLoadClocker(Analyzer2* analyzer);
  Clocker* createShiftClocker(Analyzer2* analyzer);
  SimulationChannelDescriptor* addShiftSimulationChannelDescriptor( SimulationChannelDescriptorGroup& group, U32 sample_rate, BitState intial_bit_state );
  SimulationChannelDescriptor* addLoadSimulationChannelDescriptor( SimulationChannelDescriptorGroup& group, U32 sample_rate, BitState intial_bit_state );

protected:
  ClockingChannelizer mLoadClockingChannelizer;
  ClockingChannelizer mShiftClockingChannelizer;
};


#endif //DAISY_ANALYZER_DAISY_CHANNELIZER_MANAGER_H

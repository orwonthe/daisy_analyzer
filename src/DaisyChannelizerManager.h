//
// Created by willy on 4/20/21.
//

#ifndef DAISY_ANALYZER_DAISYCHANNELIZERMANAGER_H
#define DAISY_ANALYZER_DAISYCHANNELIZERMANAGER_H

#include "MasterChannelizerManager.h"

class DaisyChannelizerManager : public MasterChannelizerManager {
public:
  DaisyChannelizerManager();
  Clocker* createLoadClocker(Analyzer2* analyzer);
  Clocker* createShiftClocker(Analyzer2* analyzer);
protected:
  ClockingChannelizer mLoadClockingChannelizer;
  ClockingChannelizer mShiftClockingChannelizer;
};


#endif //DAISY_ANALYZER_DAISYCHANNELIZERMANAGER_H

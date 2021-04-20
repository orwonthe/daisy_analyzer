//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_CLOCKINGCHANNELIZERMANAGER_H
#define DAISY_ANALYZER_CLOCKINGCHANNELIZERMANAGER_H

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerHelpers.h>
#include "ClockingChannelizer.h"
#include "ChannelizerManager.h"

class ClockingChannelizerManager : public ChannelizerManager {
public:
  ClockingChannelizerManager();

  virtual ~ClockingChannelizerManager();

  void addClockingChannelizer(ClockingChannelizer *clockingChannelizer, bool transferOwnership = true);

protected:
  std::vector<ClockingChannelizer *> mClockingChannels;
};


#endif //DAISY_ANALYZER_CLOCKINGCHANNELIZERMANAGER_H


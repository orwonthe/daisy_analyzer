//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_CLOCKINGCHANNELIZER_H
#define DAISY_ANALYZER_CLOCKINGCHANNELIZER_H

#include "Channelizer.h"
#include "Clocker.h"

class ClockingChannelizer : public Channelizer {
public:
  ClockingChannelizer(const char *title, const char *label, const char *tooltip, bool optional, bool isRisingEdge);
  virtual ~ClockingChannelizer();

  Clocker* createClocker(Analyzer2* analyzer);
protected:
  bool mIsRising;
};


#endif //DAISY_ANALYZER_CLOCKINGCHANNELIZER_H

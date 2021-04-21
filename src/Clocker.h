//
// Created by willy on 4/20/21.
//

#ifndef DAISY_ANALYZER_CLOCKER_H
#define DAISY_ANALYZER_CLOCKER_H

#include <AnalyzerChannelData.h>
#include "AnalyzerChannelDataHolder.h"


class Clocker : public AnalyzerChannelDataHolder {
public:
  explicit Clocker(AnalyzerChannelData *channelData);
  virtual ~Clocker();
  virtual U64 reachInactiveState() = 0;
  virtual U64 reachActiveState() = 0;
  virtual U64 advanceToMarkerBegin() = 0;
  virtual U64 advanceToMarkerEnd() = 0;
protected:
  virtual U64 reachDesiredState(BitState desiredState);
  virtual U64 advanceToRisingEdge();
  virtual U64 advanceToFallingEdge();
};

class FallingClocker : public Clocker {
public:
  explicit FallingClocker(AnalyzerChannelData *channelData);
  virtual ~FallingClocker();
  virtual U64 reachInactiveState();
  virtual U64 reachActiveState();
  virtual U64 advanceToMarkerBegin();
  virtual U64 advanceToMarkerEnd();
};

class RisingClocker : public Clocker {
public:
  explicit RisingClocker(AnalyzerChannelData *channelData);
  virtual ~RisingClocker();
  virtual U64 reachInactiveState();
  virtual U64 reachActiveState();
  virtual U64 advanceToMarkerBegin();
  virtual U64 advanceToMarkerEnd();
};


#endif //DAISY_ANALYZER_CLOCKER_H

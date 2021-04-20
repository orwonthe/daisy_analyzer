//
// Created by willy on 4/19/21.
//

#include "ClockingChannelizer.h"

ClockingChannelizer::ClockingChannelizer(const char *title, const char *label, const char *tooltip, bool optional,
                                         bool isRising)
    : Channelizer(title, label, tooltip, optional), mIsRising(isRising) {}

ClockingChannelizer::~ClockingChannelizer() = default;

Clocker *ClockingChannelizer::createClocker(Analyzer2 *analyzer) {
  AnalyzerChannelData *analyzerChannelData = getAnalyzerChannelData(analyzer);
  return mIsRising
         ? new RisingClocker(analyzerChannelData)
         : new FallingClocker(analyzerChannelData);
}

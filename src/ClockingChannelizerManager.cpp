//
// Created by willy on 4/19/21.
//


#include "ClockingChannelizerManager.h"

ClockingChannelizerManager::ClockingChannelizerManager() = default;

ClockingChannelizerManager::~ClockingChannelizerManager() {
  mClockingChannels.clear();
}

void
ClockingChannelizerManager::addClockingChannelizer(ClockingChannelizer *clockingChannelizer, bool transferOwnership) {
  mClockingChannels.push_back(clockingChannelizer);
  addChannelizer(clockingChannelizer, transferOwnership);
}

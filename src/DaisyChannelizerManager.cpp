//
// Created by willy on 4/20/21.
//

#include "DaisyChannelizerManager.h"

DaisyChannelizerManager::DaisyChannelizerManager() :
    mLoadClockingChannelizer("Load", "Load", "Load Clock", false, false),
    mShiftClockingChannelizer("Shift", "Shift", "Shift Clock", false, false)
{
  const U32  bitsPerTransfer = 24;

  // Create and add clocking channelizers.
  addClockingChannelizer(&mLoadClockingChannelizer, false);
  addClockingChannelizer(&mShiftClockingChannelizer, false);

  // Create and add data channelizers.
  addDataChannelizer(new DataChannelizer("ServoIn", "ServoIn", "Servo Data In", true, false, bitsPerTransfer, 0));
  addDataChannelizer(new DataChannelizer("ServoOut", "ServoOut", "Servo Data Out", true, false, bitsPerTransfer, 32));
  addDataChannelizer(new DataChannelizer("ConsoleIn", "ConsoleIn", "Console Data In", true, true, bitsPerTransfer, 0));
  addDataChannelizer(new DataChannelizer("ConsoleOut", "ConsoleOut", "Console Data Out", true, true, bitsPerTransfer, 32));
}

Clocker *DaisyChannelizerManager::createLoadClocker(Analyzer2 *analyzer) {
  return mLoadClockingChannelizer.createClocker(analyzer);
}

Clocker *DaisyChannelizerManager::createShiftClocker(Analyzer2 *analyzer) {
  return mShiftClockingChannelizer.createClocker(analyzer);
}

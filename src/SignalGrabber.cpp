//
// Created by willy on 4/20/21.
//

#include "SignalGrabber.h"

SignalGrabber::SignalGrabber(AnalyzerChannelData *channelData, bool useData2, U64 mask, int shift) :
    AnalyzerChannelDataHolder(channelData),
    mUseData2(useData2), mMask(mask), mShift(shift), mSignal(0) {
}

SignalGrabber::~SignalGrabber() = default;

void SignalGrabber::zero() {
  mSignal = 0;
}

void SignalGrabber::grab() {
  mSignal <<= 1;
  mSignal &= mMask;  // Masking as we go makes debug a little clearer.
  if (mChannelData->GetBitState() == BIT_HIGH) {
    mSignal |= 1;
  }
}

void SignalGrabber::stash(Frame &frame) const {
  // Using the mask here is redundant but a bit more robust.
  U64 data_value = (mSignal & mMask) << mShift;
  if (mUseData2) {
    frame.mData2 |= data_value;
  } else {
    frame.mData1 |= data_value;
  }
}

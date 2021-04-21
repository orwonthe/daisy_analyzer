//
// Created by willy on 4/21/21.
//

#include "BitGenerator.h"

BitGenerator::BitGenerator(U32 startValue, U32 delta, U32 bitsWide)
: mStartValue(startValue), mDelta(delta), mCurrentValue(startValue) {
  mBitSelector = mBitSelectorStart =  1 << (bitsWide - 1);
  mMask = (1 << bitsWide) - 1;
  mCurrentValue &= mMask;
}

BitGenerator::~BitGenerator() = default;

void BitGenerator::init() {
  mCurrentValue = mStartValue;
  mBitSelector = mBitSelectorStart;
}

BitState BitGenerator::nextBit() {
  mBitSelector >>= 1;
  if(mBitSelector == 0) {
    mBitSelector = mBitSelectorStart;
    mCurrentValue += mDelta;
    mCurrentValue &= mMask;
  }
  return (mCurrentValue & mBitSelector) ? BIT_HIGH : BIT_LOW;
}

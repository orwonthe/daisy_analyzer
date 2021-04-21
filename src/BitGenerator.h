//
// Created by willy on 4/21/21.
//

#ifndef DAISY_ANALYZER_BITGENERATOR_H
#define DAISY_ANALYZER_BITGENERATOR_H

#include <AnalyzerHelpers.h>

class BitGenerator {
public:
  BitGenerator(U32 startValue, U32 delta, U32 bitsWide);
  virtual ~BitGenerator();
  void init();
  BitState nextBit();
protected:
  U32 mStartValue;
  U32 mDelta;
  U32 mBitSelector;
  U32 mBitSelectorStart;
  U32 mCurrentValue;
  U32 mMask;
};


#endif //DAISY_ANALYZER_BITGENERATOR_H

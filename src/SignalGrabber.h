//
// Created by willy on 4/20/21.
//

#ifndef DAISY_ANALYZER_SIGNALGRABBER_H
#define DAISY_ANALYZER_SIGNALGRABBER_H

#include "DataChannelizerManager.h"
#include <AnalyzerChannelData.h>
#include <AnalyzerResults.h>
#include "AnalyzerChannelDataHolder.h"

class SignalGrabber : public AnalyzerChannelDataHolder {
public:
  SignalGrabber(AnalyzerChannelData *channelData, bool useData2, U64 mask, int shift);
  virtual ~SignalGrabber();

  void clear();
  void grab();
  void stash(Frame& frame);
protected:
  bool mUseData2;
  U64 mMask;
  U32 mShift;
  U64 mSignal;
};


#endif //DAISY_ANALYZER_SIGNALGRABBER_H

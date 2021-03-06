//
// Created by willy on 4/20/21.
//

#ifndef DAISY_ANALYZER_ANALYZERCHANNELDATAHOLDER_H
#define DAISY_ANALYZER_ANALYZERCHANNELDATAHOLDER_H


#include <AnalyzerChannelData.h>

class AnalyzerChannelDataHolder {

public:
  explicit AnalyzerChannelDataHolder(AnalyzerChannelData *channelData);
  U32 advanceToAbsolutePosition(U64 sample_number);
protected:
  AnalyzerChannelData *mChannelData;
};


#endif //DAISY_ANALYZER_ANALYZERCHANNELDATAHOLDER_H

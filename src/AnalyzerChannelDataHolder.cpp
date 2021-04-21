//
// Created by willy on 4/20/21.
//

#include "AnalyzerChannelDataHolder.h"

AnalyzerChannelDataHolder::AnalyzerChannelDataHolder(AnalyzerChannelData *channelData)
    : mChannelData(channelData) {}

U32 AnalyzerChannelDataHolder::advanceToAbsolutePosition(U64 sample_number) {
  return mChannelData->AdvanceToAbsPosition(sample_number);
}
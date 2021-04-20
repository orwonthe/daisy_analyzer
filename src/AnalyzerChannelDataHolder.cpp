//
// Created by willy on 4/20/21.
//

#include "Clocker.h"
#include "AnalyzerChannelDataHolder.h"

AnalyzerChannelDataHolder::AnalyzerChannelDataHolder(AnalyzerChannelData *channelData)
    : mChannelData(channelData) {}

U64 AnalyzerChannelDataHolder::getSampleNumber() {
  return mChannelData->GetSampleNumber();
}

U32 AnalyzerChannelDataHolder::advanceToAbsolutePosition(U64 sample_number) {
  return mChannelData->AdvanceToAbsPosition(sample_number);
}
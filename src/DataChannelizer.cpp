//
// Created by willy on 4/19/21.
//

#include "DataChannelizer.h"

// Knows how to insert and extract bits from Frame object.
DataChannelizer::DataChannelizer(
    const char *title,
    const char *label,
    const char *tooltip,
    bool optional,
    bool useData2,
    U32 bitsWide,
    U32 shift) :
    Channelizer(title, label, tooltip, optional),
    mUseData2(useData2),
    mBitsWide(bitsWide),
    mShift(shift) {
  if (bitsWide < 64) {
    mMask = (1 << bitsWide) - 1;
  } else {
    mMask = 0xFFFFffffFFFFffff;
  }
}

void DataChannelizer::getNumberString(Frame &frame, DisplayBase display_base, char *result_string,
                                      U32 result_string_max_length) {
  U64 data_value = mUseData2 ? frame.mData2 : frame.mData1;
  AnalyzerHelpers::GetNumberString((data_value >> mShift) & mMask, display_base, mBitsWide, result_string, result_string_max_length);
}

DataChannelizer::~DataChannelizer() = default;





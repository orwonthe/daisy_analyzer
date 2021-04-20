#include "DaisyAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "DaisyAnalyzer.h"
#include "DaisyAnalyzerSettings.h"
#include <iostream>
#include <sstream>

#pragma warning(disable: 4996) //warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.

DaisyAnalyzerResults::DaisyAnalyzerResults(DaisyAnalyzer *analyzer, DaisyAnalyzerSettings *settings,
                                           MasterChannelizerManager *channelizerManager)
    : AnalyzerResults(),
      mSettings(settings),
      mAnalyzer(analyzer),
      mChannelizerManager(channelizerManager) {
}

DaisyAnalyzerResults::~DaisyAnalyzerResults() = default;

void DaisyAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base) {
  ClearResultStrings();
  Frame frame = GetFrame(frame_index);

  if ((frame.mFlags & SPI_ERROR_FLAG) == 0) {
    char number_str[128];
    if (mChannelizerManager->getNumberString(frame, channel, display_base, number_str, 128)) {
      AddResultString(number_str);
    } else {
      AddResultString("Error");
      AddResultString("Non data channel");
    }
  } else {
    AddResultString("Error");
    AddResultString("Settings mismatch");
    AddResultString("The initial (idle) state of the CLK line does not match the settings.");
  }
}

void DaisyAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 /*export_type_user_id*/) {
  //export_type_user_id is only important if we have more than one export type.

  std::stringstream ss;
  void *export_file = AnalyzerHelpers::StartFile(file);
  U64 trigger_sample = mAnalyzer->GetTriggerSample();
  U32 sample_rate = mAnalyzer->GetSampleRate();
  std::vector<DataChannelizer *> definedDataChannelizers;
  mChannelizerManager->grabDefinedDataChannels(definedDataChannelizers);

  ss << "Time [s],Packet ID";
  for (DataChannelizer *channelizer: definedDataChannelizers) {
    ss << "," << channelizer->title();
  }
  ss << std::endl;
  AnalyzerHelpers::AppendToFile((U8 *) ss.str().c_str(), ss.str().length(), export_file);

  U64 num_frames = GetNumFrames();
  for (U32 frame_index = 0; frame_index < num_frames; frame_index++) {
    Frame frame = GetFrame(frame_index);

    if ((frame.mFlags & SPI_ERROR_FLAG) != 0)
      continue;

    ss.str(std::string());

    char time_str[128];
    AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);
    ss << time_str << ",";

    U64 packet_id = GetPacketContainingFrameSequential(frame_index);
    if (packet_id != INVALID_RESULT_INDEX)
      ss << packet_id;

    for (DataChannelizer *channelizer: definedDataChannelizers) {
      char value_str[128] = "";
      channelizer->getNumberString(frame, display_base, value_str, 128);
      ss << "," << value_str;
    }
    ss << std::endl;
    AnalyzerHelpers::AppendToFile((U8 *) ss.str().c_str(), ss.str().length(), export_file);

    if (UpdateExportProgressAndCheckForCancel(frame_index, num_frames)) {
      AnalyzerHelpers::EndFile(export_file);
      return;
    }
  }

  UpdateExportProgressAndCheckForCancel(num_frames, num_frames);
  AnalyzerHelpers::EndFile(export_file);
}

void DaisyAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base) {
  ClearTabularText();
  Frame frame = GetFrame(frame_index);

  std::stringstream ss;
  if ((frame.mFlags & SPI_ERROR_FLAG) == 0) {
    std::vector<DataChannelizer *> definedDataChannelizers;
    mChannelizerManager->grabDefinedDataChannels(definedDataChannelizers);
    const char *delim = "";
    for (DataChannelizer *channelizer: definedDataChannelizers) {
      char value_str[128] = "";
      channelizer->getNumberString(frame, display_base, value_str, 128);
      ss << delim << channelizer->label() << ": " << value_str;
      delim = "; ";
    }
  } else {
    ss << "The initial (idle) state of the CLK line does not match the settings.";
  }
  AddTabularText(ss.str().c_str());
}

void DaisyAnalyzerResults::GeneratePacketTabularText(U64 /*packet_id*/,
                                                     DisplayBase /*display_base*/)  //unrefereced vars commented out to remove warnings.
{
  ClearResultStrings();
  AddResultString("not supported");
}

void DaisyAnalyzerResults::GenerateTransactionTabularText(U64 /*transaction_id*/,
                                                          DisplayBase /*display_base*/)  //unrefereced vars commented out to remove warnings.
{
  ClearResultStrings();
  AddResultString("not supported");
}

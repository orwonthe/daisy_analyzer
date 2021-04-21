//
// Created by willy on 4/21/21.
//

#include "ChannelizedAnalyzerSettings.h"

ChannelizedAnalyzerSettings::ChannelizedAnalyzerSettings() = default;

void ChannelizedAnalyzerSettings::addChannel(Channel& channel, const char* channel_label, bool is_used) {
  AddChannel(channel, channel_label, is_used);
}

void ChannelizedAnalyzerSettings::addInterface(AnalyzerSettingInterface* analyzer_setting_interface) {
  AddInterface(analyzer_setting_interface);
}


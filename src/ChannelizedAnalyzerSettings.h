//
// Created by willy on 4/21/21.
//

#ifndef DAISY_ANALYZER_CHANNELIZEDANALYZERSETTINGS_H
#define DAISY_ANALYZER_CHANNELIZEDANALYZERSETTINGS_H

#include <AnalyzerSettings.h>


class ChannelizedAnalyzerSettings : public AnalyzerSettings {
public:
  ChannelizedAnalyzerSettings();
protected:
  friend class Channelizer;
  void addChannel(Channel& channel, const char* channel_label, bool is_used );
  void addInterface(AnalyzerSettingInterface* analyzer_setting_interface);
};


#endif //DAISY_ANALYZER_CHANNELIZEDANALYZERSETTINGS_H

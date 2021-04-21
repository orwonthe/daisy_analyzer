//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_CHANNELIZERMANAGER_H
#define DAISY_ANALYZER_CHANNELIZERMANAGER_H

#include "ChannelizerOwner.h"

class ChannelizerManager : public ChannelizerOwner {
public:
  ChannelizerManager();

  virtual ~ChannelizerManager();

  // Adding channels.
  void addChannelizer(Channelizer *channelizer, bool transferOwnership = true);

  // Managing channelizers
  void addChannelInterfaces(ChannelizedAnalyzerSettings *settings);
  void addChannelsUnused(ChannelizedAnalyzerSettings *settings);
  void addChannels(ChannelizedAnalyzerSettings *settings);
  void setChannelsFromInterfaces();
  void setInterfacesFromChannels();
  // Attributes
  int definedChannelCount();
  bool hasAtLeastOneDefinedChannel();
  std::string titles(const char* delim=", ");
  std::vector<Channelizer *> definedChannels();

  // Archiving and exporting
  void loadFromArchive(SimpleArchive& archive);
  void saveToArchive(SimpleArchive& archive);

protected:
  std::vector<Channelizer *> mChannelizers;
};


#endif //DAISY_ANALYZER_CHANNELIZERMANAGER_H

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
  void addChannelInterfaces(AnalyzerSettings *settings);
  void addChannelsUnused(AnalyzerSettings *settings);
  void addChannels(AnalyzerSettings *settings);
  void setChannelsFromInterfaces(AnalyzerSettings *settings);

  // Attributes
  int definedChannelCount();
  bool hasAtLeastOneDefinedChannel();
  std::string titles(const char* delim=", ");
  void grabDefinedChannels(std::vector<Channelizer *> &channelizers);

  // Archiving and exporting
  void loadFromArchive(SimpleArchive& archive);
  void saveToArchive(SimpleArchive& archive);

protected:
  std::vector<Channelizer *> mChannelizers;
};


#endif //DAISY_ANALYZER_CHANNELIZERMANAGER_H

//
// Created by willy on 4/18/21.
//

#ifndef DAISY_ANALYZER_CHANNELIZER_H
#define DAISY_ANALYZER_CHANNELIZER_H

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerResults.h>
#include <AnalyzerHelpers.h>
#include "ChannelizedAnalyzerSettings.h"

class Channelizer {
public:
  Channelizer(const char *title, const char *label, const char *tooltip, bool optional);
  virtual ~Channelizer();

  // Attributes
  bool channelIsDefined();
  bool channelFromInterfaceIsDefined();
  AnalyzerSettingInterfaceChannel *getInterface();
  bool isMyChannel(Channel &channel);
  const char *title();
  const char *label();
  const char *tooltip();

  // Interface manipulations
  void addChannel(ChannelizedAnalyzerSettings *settings);
  void addChannelUnused(ChannelizedAnalyzerSettings *settings);
  void addChannel(ChannelizedAnalyzerSettings *settings, bool isUsed);
  void addInterface(ChannelizedAnalyzerSettings *settings);
  void setChannelFromInterface();
  void setInterfaceFromChannel();
  void markChannelAsBubbleWorthy(AnalyzerResults *analyzerResults);
  SimulationChannelDescriptor *
  addSimulationChannelDescriptor(SimulationChannelDescriptorGroup &group, U32 sample_rate, BitState intial_bit_state);

  // Archiving
  void loadFromArchive(SimpleArchive &archive);
  void saveToArchive(SimpleArchive &archive);

  // Utilities that work with lists of Channelizer
  static bool hasOverlap(std::vector<Channelizer *> &channels);
  static int definedChannelCountFromInterface(std::vector<Channelizer *> &channels);
  static std::string titles(std::vector<Channelizer *> &channels, const char *delim = ", ");

protected:
  AnalyzerChannelData *getAnalyzerChannelData(Analyzer2 *analyzer);
  const char *mTitle;
  const char *mLabel;
  const char *mTooltip;
  bool mOptional;
  Channel mChannel;
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mChannelInterface;

};


#endif //DAISY_ANALYZER_CHANNELIZER_H

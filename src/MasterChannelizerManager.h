//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_MASTERCHANNELIZERMANAGER_H
#define DAISY_ANALYZER_MASTERCHANNELIZERMANAGER_H

#include "ClockingChannelizerManager.h"
#include "DataChannelizerManager.h"

class MasterChannelizerManager {
public:
  MasterChannelizerManager();

  virtual ~MasterChannelizerManager();

  // Adding channelizers
  void addClockingChannelizer(ClockingChannelizer *clockingChannelizer, bool transferOwnership = true);
  void addDataChannelizer(DataChannelizer *dataChannelizer, bool transferOwnership = true);

  // Managing channelizers
  void addChannelInterfaces(AnalyzerSettings *settings);
  void addChannelsUnused(AnalyzerSettings *settings);
  void addChannels(AnalyzerSettings *settings);
  void setChannelsFromInterfaces(AnalyzerSettings *settings);
  void setInterfacesFromChannels(AnalyzerSettings *settings);
  void markDataChannelsAsBubbleWorthy(AnalyzerResults* analyzerResults);

  // Attributes
  bool doChannelsOverlap();
  bool hasAtLeastOneDataChannel();
  std::string dataTitles(const char *delim = ", ");
  std::string activeDataTitles(const char *delim = ",");
  DataChannelizer* getDataChannelizer(Channel &channel);
  void grabDefinedDataChannels(std::vector<DataChannelizer *> &dataChannelizers);

  // Extracting results
  bool getNumberString(Frame &frame, Channel &channel, DisplayBase display_base, char *result_string,
                       U32 result_string_max_length);

  // Archiving and exporting
  void loadFromArchive(SimpleArchive &archive);
  void saveToArchive(SimpleArchive &archive);

protected:
  ClockingChannelizerManager mClockingManager;
  DataChannelizerManager mDataManager;
  std::vector<Channelizer *> mChannelizers;
};


#endif //DAISY_ANALYZER_MASTERCHANNELIZERMANAGER_H

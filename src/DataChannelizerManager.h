//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_DATACHANNELIZERMANAGER_H
#define DAISY_ANALYZER_DATACHANNELIZERMANAGER_H

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerHelpers.h>
#include "DataChannelizer.h"
#include "ChannelizerManager.h"

class DataChannelizerManager : public ChannelizerManager {
public:
  DataChannelizerManager();

  virtual ~DataChannelizerManager();

  void addDataChannelizer(DataChannelizer *dataChannelizer, bool transferOwnership = true);
  DataChannelizer* getDataChannelizer(Channel &channel);

  void grabDefinedDataChannels(std::vector<DataChannelizer *> &dataChannelizers);
  void markChannelsAsBubbleWorthy(AnalyzerResults* analyzerResults);

protected:
  std::vector<DataChannelizer *> mDataChannelizers;
};

#endif //DAISY_ANALYZER_DATACHANNELIZERMANAGER_H


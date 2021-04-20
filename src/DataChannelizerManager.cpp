//
// Created by willy on 4/19/21.
//

#include "DataChannelizerManager.h"

DataChannelizerManager::DataChannelizerManager() = default;

DataChannelizerManager::~DataChannelizerManager() {
  mDataChannelizers.clear();
}

void DataChannelizerManager::addDataChannelizer(DataChannelizer *dataChannelizer, bool transferOwnership) {
  mDataChannelizers.push_back(dataChannelizer);
  addChannelizer(dataChannelizer, transferOwnership);
}

DataChannelizer *DataChannelizerManager::getDataChannelizer(Channel &channel) {
  for(DataChannelizer* channelizer: mDataChannelizers) {
    if(channelizer->isMyChannel(channel)) {
      return channelizer;
    }
  }
  return nullptr;
}

void DataChannelizerManager::grabDefinedDataChannels(std::vector<DataChannelizer *> &dataChannelizers) {
  for( DataChannelizer* channelizer: mDataChannelizers) {
    if(channelizer->channelIsDefined()) {
      dataChannelizers.push_back(channelizer);
    }
  }
}

void DataChannelizerManager::markChannelsAsBubbleWorthy(AnalyzerResults *analyzerResults) {
  for( DataChannelizer* channelizer: mDataChannelizers) {
    channelizer->markChannelAsBubbleWorthy(analyzerResults);
  }
}

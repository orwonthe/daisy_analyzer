//
// Created by willy on 4/19/21.
//

#include "MasterChannelizerManager.h"

MasterChannelizerManager::MasterChannelizerManager() = default;

MasterChannelizerManager::~MasterChannelizerManager() = default;

void
MasterChannelizerManager::addClockingChannelizer(ClockingChannelizer *clockingChannelizer, bool transferOwnership) {
  mClockingManager.addClockingChannelizer(clockingChannelizer, transferOwnership);
  mChannelizers.push_back(clockingChannelizer);
}

void MasterChannelizerManager::addDataChannelizer(DataChannelizer *dataChannelizer, bool transferOwnership) {
  mDataManager.addDataChannelizer(dataChannelizer, transferOwnership);
  mChannelizers.push_back(dataChannelizer);
}

void MasterChannelizerManager::addChannelInterfaces(AnalyzerSettings *settings) {
  mClockingManager.addChannelInterfaces(settings);
  mDataManager.addChannelInterfaces(settings);
}

void MasterChannelizerManager::addChannelsUnused(AnalyzerSettings *settings) {
  mClockingManager.addChannelsUnused(settings);
  mDataManager.addChannelsUnused(settings);
}

void MasterChannelizerManager::addChannels(AnalyzerSettings *settings) {
  mClockingManager.addChannels(settings);
  mDataManager.addChannels(settings);
}

void MasterChannelizerManager::setChannelsFromInterfaces(AnalyzerSettings *settings) {
  mClockingManager.setChannelsFromInterfaces(settings);
  mDataManager.setChannelsFromInterfaces(settings);
}

void MasterChannelizerManager::setInterfacesFromChannels(AnalyzerSettings *settings) {
  mClockingManager.setInterfacesFromChannels(settings);
  mDataManager.setInterfacesFromChannels(settings);
}


bool MasterChannelizerManager::doChannelsOverlap() {
  return Channelizer::hasOverlap(mChannelizers);
}

bool MasterChannelizerManager::hasAtLeastOneDataChannel() {
  return mDataManager.hasAtLeastOneDefinedChannel();
}

void MasterChannelizerManager::loadFromArchive(SimpleArchive &archive) {
  mClockingManager.loadFromArchive(archive);
  mDataManager.loadFromArchive(archive);
}

void MasterChannelizerManager::saveToArchive(SimpleArchive &archive) {
  mClockingManager.saveToArchive(archive);
  mDataManager.saveToArchive(archive);
}

std::string MasterChannelizerManager::dataTitles(const char *delim) {
  return mDataManager.titles(delim);
}

bool
MasterChannelizerManager::getNumberString(Frame &frame, Channel &channel, DisplayBase display_base, char *result_string,
                                          U32 result_string_max_length) {
  DataChannelizer *channelizer = getDataChannelizer(channel);
  if (channelizer) {
    channelizer->getNumberString(frame, display_base, result_string, result_string_max_length;
    return true;
  } else { return false; }
}

DataChannelizer *MasterChannelizerManager::getDataChannelizer(Channel &channel) {
  return mDataManager.getDataChannelizer(channel);
}

std::string MasterChannelizerManager::activeDataTitles(const char *delim) {
  std::vector<Channelizer *> definedDataChannelizers;
  mDataManager.grabDefinedChannels(definedDataChannelizers);
  return Channelizer::titles(definedDataChannelizers);
}

void MasterChannelizerManager::grabDefinedDataChannels(std::vector<DataChannelizer *> &dataChannelizers) {
  mDataManager.grabDefinedDataChannels(dataChannelizers);
}

void MasterChannelizerManager::markDataChannelsAsBubbleWorthy(AnalyzerResults *analyzerResults) {
  mDataManager.markChannelsAsBubbleWorthy(analyzerResults);
}


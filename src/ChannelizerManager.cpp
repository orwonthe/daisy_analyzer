//
// Created by willy on 4/19/21.
//

#include "ChannelizerManager.h"

ChannelizerManager::ChannelizerManager() = default;

ChannelizerManager::~ChannelizerManager() = default;

void ChannelizerManager::addChannelizer(Channelizer *channelizer, bool transferOwnership) {
  mChannelizers.push_back(channelizer);
  if (transferOwnership) { addOwnedChannelizer(channelizer); }
}

void ChannelizerManager::addChannelInterfaces(AnalyzerSettings *settings) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->addInterface(settings);
}

void ChannelizerManager::addChannelsUnused(AnalyzerSettings *settings) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->addChannelUnused(settings);
}

void ChannelizerManager::addChannels(AnalyzerSettings *settings) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->addChannel(settings);
}

void ChannelizerManager::setChannelsFromInterfaces(AnalyzerSettings *settings) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->setChannelFromInterface(settings);
}

int ChannelizerManager::definedChannelCount() {
  return Channelizer::definedChannelCountFromInterface(mChannelizers);
}

bool ChannelizerManager::hasAtLeastOneDefinedChannel() {
  return definedChannelCount() > 0;
}

std::string ChannelizerManager::titles(const char *delim) {
  return Channelizer::titles(mChannelizers, delim);
}

void ChannelizerManager::loadFromArchive(SimpleArchive &archive) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->loadFromArchive(archive);
}

void ChannelizerManager::saveToArchive(SimpleArchive &archive) {
  for (Channelizer *channelizer: mChannelizers)
    channelizer->saveToArchive(archive);
}

void ChannelizerManager::grabDefinedChannels(std::vector<Channelizer *> &channelizers) {
  for( Channelizer* channelizer: mChannelizers) {
    if(channelizer->channelIsDefined()) {
      channelizers.push_back(channelizer);
    }
  }
}



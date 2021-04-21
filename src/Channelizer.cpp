//
// Created by willy on 4/18/21.
//

#include "Channelizer.h"
#include <AnalyzerHelpers.h>
#include <AnalyzerChannelData.h>

Channelizer::Channelizer(const char *title, const char *label, const char *tooltip, bool optional) :
    mChannel(UNDEFINED_CHANNEL),
    mTitle(title),
    mLabel(label),
    mTooltip(tooltip),
    mOptional(optional) {
  mChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
  mChannelInterface->SetTitleAndTooltip(mTitle, mTooltip);
  mChannelInterface->SetChannel(mChannel);
  mChannelInterface->SetSelectionOfNoneIsAllowed(mOptional);
}

Channelizer::~Channelizer() = default;

AnalyzerSettingInterfaceChannel *Channelizer::getInterface() {
  return mChannelInterface.get();
}

bool Channelizer::channelIsDefined() {
  return mChannel != UNDEFINED_CHANNEL;
}

bool Channelizer::channelFromInterfaceIsDefined() {
  return mChannelInterface->GetChannel() != UNDEFINED_CHANNEL;
}

void Channelizer::setChannelFromInterface() {
  mChannel = mChannelInterface->GetChannel();
}

void Channelizer::setInterfaceFromChannel() {
  mChannelInterface->SetChannel(mChannel);
}

void Channelizer::addChannelUnused(ChannelizedAnalyzerSettings *settings) {
  addChannel(settings, false);
}

void Channelizer::addChannel(ChannelizedAnalyzerSettings *settings) {
  addChannel(settings, mChannel != UNDEFINED_CHANNEL);
}

void Channelizer::addChannel(ChannelizedAnalyzerSettings *settings, bool isUsed) {
  settings->addChannel(mChannel, mLabel, isUsed);
}

void Channelizer::addInterface(ChannelizedAnalyzerSettings *settings) {
  settings->addInterface(mChannelInterface.get());
}

bool Channelizer::hasOverlap(std::vector<Channelizer *> &channelizers) {
  std::vector<Channel> channels;
  for (Channelizer *pChannel: channelizers) {
    channels.push_back(pChannel->getInterface()->GetChannel());
  }
  return AnalyzerHelpers::DoChannelsOverlap(&channels[0], (U32) channels.size());
}

int Channelizer::definedChannelCountFromInterface(std::vector<Channelizer *> &channels) {
  int count = 0;
  for (Channelizer *pChannel: channels) {
    if (pChannel && pChannel->channelFromInterfaceIsDefined()) {
      count++;
    }
  }
  return count;
}

void Channelizer::loadFromArchive(SimpleArchive &archive) {
  archive >> mChannel;
}

void Channelizer::saveToArchive(SimpleArchive &archive) {
  archive << mChannel;
}

bool Channelizer::isMyChannel(Channel &channel) {
  return mChannel == channel;
}

const char *Channelizer::title() {
  return mTitle;
}

const char *Channelizer::label() {
  return mLabel;
}

const char *Channelizer::tooltip() {
  return mTooltip;
}

std::string Channelizer::titles(std::vector<Channelizer *> &channels, const char *delim) {
  std::string result;
  const char *current_delim = "";
  for (Channelizer *channelizer: channels) {
    result += current_delim;
    result += channelizer->title();
    current_delim = delim;
  }
  return result;
}

void Channelizer::markChannelAsBubbleWorthy(AnalyzerResults *analyzerResults) {
  if (mChannel != UNDEFINED_CHANNEL) {
    analyzerResults->AddChannelBubblesWillAppearOn(mChannel);
  }
}

AnalyzerChannelData *Channelizer::getAnalyzerChannelData(Analyzer2* analyzer) {
  return analyzer->GetAnalyzerChannelData(mChannel);
}

SimulationChannelDescriptor *
Channelizer::addSimulationChannelDescriptor(SimulationChannelDescriptorGroup &group, U32 sample_rate,
                                            BitState intial_bit_state) {
  return group.Add(mChannel, sample_rate, intial_bit_state);
}


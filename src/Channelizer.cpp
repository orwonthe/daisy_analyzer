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
    mChannelInterface(tooltip),
    mOptional(optional) {
  mChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
  mChannelInterface->SetTitleAndTooltip(mChannelInterface, mTitle, mTooltip);
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
  return mChannelInterface.GetChannel() != UNDEFINED_CHANNEL;
}

void Channelizer::setChannelFromInterface() {
  mChannel = mChannelInterface->GetChannel();
}

void Channelizer::setInterfaceFromChannel() {
  mChannelInterface->SetChannel(mChannel);
}

void Channelizer::addChannelUnused(AnalyzerSettings *settings) {
  addChannel(settings, false);
}

void Channelizer::addChannel(AnalyzerSettings *settings) {
  addChannel(settings, mChannel != UNDEFINED_CHANNEL);
}

void Channelizer::addChannel(AnalyzerSettings *settings, bool isUsed) {
  settings->AddChannel(mChannel, mLabel, isUsed);
}

void Channelizer::addInterface(AnalyzerSettings *settings) {
  settings->AddInterface(mChannelInterface.get());
}

bool Channelizer::hasOverlap(std::vector<Channelizer *> &channels) {
  std::vector<Channel> channels;
  for (Channelizer *pChannel: channels) {
    channels.push_back(pChannel->getInterface()->GetChannel());
  }
  return AnalyzerHelpers::DoChannelsOverlap(&channels[0], channels.size());
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
  for (Channelizer *channelizer: mChannelizers) {
    result += current_delim;
    result += channelizer->title();
    current_delim = delim;
  }
  return result;
}

void Channelizer::markChannelAsBubbleWorthy(AnalyzerResults *analyzerResults) {
  if (mChannel != UNDEFINED_CHANNEL) {
    mResults->AddChannelBubblesWillAppearOn(mChannel);
  }
}

AnalyzerChannelData *Channelizer::getAnalyzerChannelData(Analyzer2* analyzer) {
  return analyzer->GetAnalyzerChannelData(mChannel);
}


//
// Created by willy on 4/19/21.
//

#include "ChannelizerOwner.h"

ChannelizerOwner::ChannelizerOwner() = default;

ChannelizerOwner::~ChannelizerOwner() {
  for (Channelizer *channelizer: mOwnedChannelizers) { delete channelizer; }
  mOwnedChannelizers.clear();
}

void ChannelizerOwner::addOwnedChannelizer(Channelizer *ownedChannelizer) {
  mOwnedChannelizers.push_back(ownedChannelizer);
}

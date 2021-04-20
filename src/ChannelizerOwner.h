//
// Created by willy on 4/19/21.
//

#ifndef DAISY_ANALYZER_CHANNELIZEROWNER_H
#define DAISY_ANALYZER_CHANNELIZEROWNER_H

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerHelpers.h>
#include "Channelizer.h"

class ChannelizerOwner {
public:
  ChannelizerOwner();

  virtual ~ChannelizerOwner();

protected:
  void addOwnedChannelizer(Channelizer *ownedChannelizer);

  std::vector<Channelizer *> mOwnedChannelizers;
};


#endif //DAISY_ANALYZER_CHANNELIZEROWNER_H

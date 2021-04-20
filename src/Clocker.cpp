//
// Created by willy on 4/20/21.
//

#include "Clocker.h"
#include "AnalyzerChannelDataHolder.h"

Clocker::~Clocker() {

}

U64 Clocker::advanceToRisingEdge() {
  reachDesiredState(BIT_LOW);
  mChannelData->AdvanceToNextEdge();
  return mChannelData->GetSampleNumber();
}

U64 Clocker::advanceToFallingEdge() {
  reachDesiredState(BIT_HIGH);
  mChannelData->AdvanceToNextEdge();
  return mChannelData->GetSampleNumber();
}

U64 Clocker::reachDesiredState(BitState desiredState) {
  if (mChannelData->GetBitState() != desiredState) {
    mChannelData->AdvanceToNextEdge();
  }
  return mChannelData->GetSampleNumber();
}

FallingClocker::FallingClocker(AnalyzerChannelData *channelData) : AnalyzerChannelDataHolder(channelData) {

}

FallingClocker::~FallingClocker() {

}

U64 FallingClocker::advanceToMarkerBegin() {
  return advanceToFallingEdge();
}

U64 FallingClocker::advanceToMarkerEnd() {
  return advanceToRisingEdge();
}

U64 FallingClocker::reachInactiveState() {
  return reachDesiredState(BIT_HIGH);
}

U64 FallingClocker::reachActiveState() {
  return reachDesiredState(BIT_LOW;
}

RisingClocker::RisingClocker(AnalyzerChannelData *channelData) : AnalyzerChannelDataHolder(channelData) {}

RisingClocker::~RisingClocker() {

}

U64 RisingClocker::advanceToMarkerBegin() {
  return advanceToRisingEdge();
}

U64 RisingClocker::advanceToMarkerEnd() {
  return advanceToFallingEdge();
}

U64 RisingClocker::reachInactiveState() {
  return reachDesiredState(BIT_LOW;
}

U64 RisingClocker::reachActiveState() {
  return reachDesiredState(BIT_HIGH);
}

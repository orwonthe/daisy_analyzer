#include "DaisyAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

DaisyAnalyzerSettings::DaisyAnalyzerSettings(MasterChannelizerManager *channelizerManager) : mChannelizerManager(
    channelizerManager) {

  // Let the world know about the channel interfaces.
  mChannelizerManager->addChannelInterfaces(this);

  // Define how export is going to work.
  AddExportOption(0, "Export as text/csv file");
  AddExportExtension(0, "text", "txt");
  AddExportExtension(0, "csv", "csv");

  ClearChannels();

  // Let the world know about the channels.
  mChannelizerManager->addChannelsUnused(this);
}

DaisyAnalyzerSettings::~DaisyAnalyzerSettings() = default;

bool DaisyAnalyzerSettings::SetSettingsFromInterfaces() {

  if (mChannelizerManager->doChannelsOverlap()) {
    SetErrorText("Please select different channels for each input.");
    return false;
  }

  if (!mChannelizerManager->hasAtLeastOneDataChannel()) {
    std::string message = "Please select at least one input from: " + mChannelizerManager->dataTitles();
    SetErrorText(message.c_str());
    return false;
  }

  mChannelizerManager->setChannelsFromInterfaces(this);
  ClearChannels();
  mChannelizerManager->addChannels(this);
  return true;
}

void DaisyAnalyzerSettings::LoadSettings(const char *settings) {
  SimpleArchive text_archive;
  text_archive.SetString(settings);

  const char *name_string;  //the first thing in the archive is the name of the protocol analyzer that the data belongs to.
  text_archive >> &name_string;
  if (strcmp(name_string, "DaisyAnalyzer") != 0)
    AnalyzerHelpers::Assert("DaisyAnalyzer: Provided with a settings string that doesn't belong to us;");

  mChannelizerManager->loadFromArchive(text_archive);
  ClearChannels();
  mChannelizerManager->addChannels(this);
  UpdateInterfacesFromSettings();
}

const char *DaisyAnalyzerSettings::SaveSettings() {
  SimpleArchive text_archive;

  text_archive << "DaisyAnalyzer";

  mChannelizerManager->saveToArchive(text_archive);

  return SetReturnString(text_archive.GetString());
}

void DaisyAnalyzerSettings::UpdateInterfacesFromSettings() {
  mChannelizerManager->setInterfacesFromChannels(this);
}

//bool DaisyAnalyzerSettings::hasAtLeastOneDataChannel() {
//  return DataChannelizer::hasInterfaceDefinedChannel(mDataChannels);
//}
//
//bool DaisyAnalyzerSettings::isServo(Channel &channel) {
//  return mServoIn.isMyChannel(channel) || mServoOut.isMyChannel(channel);
//}
//
//bool DaisyAnalyzerSettings::isConsole(Channel &channel) {
//  return mConsoleIn.isMyChannel(channel) || mConsoleOut.isMyChannel(channel);
//}
//
//bool DaisyAnalyzerSettings::isDataIn(Channel &channel) {
//  return mServoIn.isMyChannel(channel) || mConsoleIn.isMyChannel(channel);
//}
//
//bool DaisyAnalyzerSettings::isDataOut(Channel &channel) {
//  return mConsoleOut.isMyChannel(channel) || mServoOut.isMyChannel(channel);
//}
//
//bool DaisyAnalyzerSettings::hasServoIn() {
//  return mServoIn.channelIsDefined();
//}
//
//bool DaisyAnalyzerSettings::hasServoOut() {
//  return mServoOut.channelIsDefined();
//}
//
//bool DaisyAnalyzerSettings::hasConsoleIn() {
//  return mConsoleIn.channelIsDefined();
//}
//
//bool DaisyAnalyzerSettings::hasConsoleOut() {
//  return  mConsoleOut.channelIsDefined();
//}
//



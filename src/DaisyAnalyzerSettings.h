#ifndef DAISY_ANALYZER_SETTINGS
#define DAISY_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerHelpers.h>
#include "MasterChannelizerManager.h"

class DaisyAnalyzerSettings : public AnalyzerSettings
{
public:
	DaisyAnalyzerSettings(MasterChannelizerManager* channelizerManager);
	virtual ~DaisyAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	void UpdateInterfacesFromSettings();

//	bool isServo(Channel& channel);
//	bool isConsole(Channel& channel);
//	bool isDataIn(Channel& channel);
//	bool isDataOut(Channel& channel);
//
//	bool hasServoIn();
//	bool hasServoOut();
//	bool hasConsoleIn();
//	bool hasConsoleOut();

protected:
  MasterChannelizerManager* mChannelizerManager;

  // Archiving
//  void loadFromArchive(SimpleArchive& archive);
//  void saveToArchive(SimpleArchive& archive);

	// Data channels
//  DataChannelizer mServoIn;
//  DataChannelizer mServoOut;
//  DataChannelizer mConsoleIn;
//  DataChannelizer mConsoleOut;

	// Clocking channels
//  Channelizer mShiftClock;
//  Channelizer mLoadClock;

  // Channel list
//  std::vector<Channelizer*> mChannels;
//  std::vector<DataChannelizer*> mDataChannels;
//  std::vector<Channelizer*> mClockingChannels;

	// Fixed values, not actually settings.
//	U32 mBitsPerTransfer;

};

#endif //DAISY_ANALYZER_SETTINGS

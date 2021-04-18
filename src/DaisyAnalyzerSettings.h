#ifndef DAISY_ANALYZER_SETTINGS
#define DAISY_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class DaisyAnalyzerSettings : public AnalyzerSettings
{
public:
	DaisyAnalyzerSettings();
	virtual ~DaisyAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	void UpdateInterfacesFromSettings();
	
	Channel mServoInChannel;
	Channel mServoOutChannel;
	Channel mConsoleInChannel;
	Channel mConsoleOutChannel;
	Channel mShiftClockChannel;
	Channel mLoadClockChannel;
	AnalyzerEnums::ShiftOrder mShiftOrder;
	U32 mBitsPerTransfer;
	BitState mClockInactiveState;
	AnalyzerEnums::Edge mDataValidEdge;
	BitState mEnableActiveState;


protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mServoInChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mServoOutChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mConsoleInChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mConsoleOutChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mShiftClockChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mLoadClockChannelInterface;
//	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mShiftOrderInterface;
//	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mBitsPerTransferInterface;
//	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mClockInactiveStateInterface;
//	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mDataValidEdgeInterface;
//	std::auto_ptr< AnalyzerSettingInterfaceNumberList > mEnableActiveStateInterface;
};

#endif //DAISY_ANALYZER_SETTINGS

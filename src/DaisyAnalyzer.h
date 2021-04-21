#ifndef SPI_ANALYZER_H
#define SPI_ANALYZER_H

#include <Analyzer.h>
#include "DaisyAnalyzerResults.h"
#include "DaisySimulationDataGenerator.h"
#include "DaisyChannelizerManager.h"
#include "SignalGrabber.h"

//class DaisyAnalyzerSettings;

class DaisyAnalyzer : public Analyzer2 {
public:
  DaisyAnalyzer();
  virtual ~DaisyAnalyzer();
  virtual void SetupResults();
  virtual void WorkerThread();

  virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate,
                                     SimulationChannelDescriptor **simulation_channels);
  virtual U32 GetMinimumSampleRateHz();

  virtual const char *GetAnalyzerName() const;
  virtual bool NeedsRerun();

protected:
  void setup();
  void startNewPacket();
  void advanceToLoadEventEnd();
  void advanceToShiftEventEnd();
  void syncAllChannels();
  void captureCurrentDataSignals();
  void loadFrameWithSignalData(Frame& frame);
  void getWord();

//#pragma warning( push )
//#pragma warning( disable : 4251 ) //warning C4251: 'SerialAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class
protected:  //vars
  std::unique_ptr<DaisyAnalyzerSettings> mSettings;
  std::unique_ptr<DaisyAnalyzerResults> mResults;
  bool mSimulationInitialized;
  std::unique_ptr<DaisySimulationDataGenerator> mSimulationDataGenerator;

  U64 mCurrentSample;
  std::vector<U64> mArrowLocations;

  std::unique_ptr<DaisyChannelizerManager> mChannelizerManager;
  std::unique_ptr<Clocker> mLoadClocker;
  std::unique_ptr<Clocker> mShiftClocker;
  std::vector<SignalGrabber *> mSignalGrabbers;

private:

//#pragma warning( pop )
  void deleteSignalGrabbers();
  void resetClockers();
  void resetSignalGrabbers();
  void zeroSignalGrabbers();
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //SPI_ANALYZER_H

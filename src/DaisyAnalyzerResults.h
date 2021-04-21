#ifndef SPI_ANALYZER_RESULTS
#define SPI_ANALYZER_RESULTS

#include <AnalyzerResults.h>
#include "DaisyChannelizerManager.h"

#define SPI_ERROR_FLAG ( 1 << 0 )

class DaisyAnalyzer;

class DaisyAnalyzerSettings;

class DaisyAnalyzerResults : public AnalyzerResults {
public:
  DaisyAnalyzerResults(DaisyAnalyzer *analyzer, DaisyAnalyzerSettings *settings,
                       DaisyChannelizerManager *channelizerManager);
  virtual ~DaisyAnalyzerResults();

  virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
  virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

  virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
  virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
  virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions

protected:  //vars
  DaisyAnalyzerSettings *mSettings;
  DaisyAnalyzer *mAnalyzer;
  DaisyChannelizerManager *mChannelizerManager;
};

#endif //SPI_ANALYZER_RESULTS

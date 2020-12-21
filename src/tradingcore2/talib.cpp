#include <ta_libc.h>
#include <tradingcore2/talib.h>

CR2BEGIN

bool initTALib() {
  auto retCode = TA_Initialize();
  if (retCode != TA_SUCCESS) {
    LOG(ERROR) << "initTALib:TA_Initialize " << retCode;

    return false;
  }

  return true;
}

void releaseTALib() { TA_Shutdown(); }

CR2END
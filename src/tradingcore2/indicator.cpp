#include <tradingcore2/csv.h>
#include <tradingcore2/indicator.h>

#include <functional>

CR2BEGIN

// saveCSV - save to a csv file
void Indicator::saveCSV(const char* fn) {
  auto onhead = [](FILE* fp) { fprintf(fp, "ts,val\r\n"); };

  auto pIndicator = this;

  auto onrow = [&pIndicator](FILE* fp, int row) {
    if (pIndicator->getLength() <= row) {
      return false;
    }

    fprintf(fp, "%ld,%.4f\r\n", pIndicator->getTimeStamp(row),
            pIndicator->getSingleValue(row)->value);

    return true;
  };

  tr2::saveCSV(fn, onhead, onrow);
}

CR2END
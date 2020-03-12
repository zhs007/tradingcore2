#include <assert.h>
#include <tradingcore2/utils.h>
#include <ctime>
#include <iomanip>
#include <sstream>

CR2BEGIN

TimeStamp str2timestamp(const char* str, const char* format) {
  try {
    tm t;
    std::istringstream ss(str);
    ss >> std::get_time(&t, format);

    time_t ts = mktime(&t);

    return ts;
  } catch (...) {
    return 0;
  }
}

CR2END
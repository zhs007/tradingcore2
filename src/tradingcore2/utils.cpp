#include <assert.h>
#include <tradingcore2/utils.h>
#include <iomanip>
#include <sstream>

CR2BEGIN

TimeStamp str2timestampUTC(const char* str, const char* format) {
  try {
    tm t;
    memset(&t, 0, sizeof(tm));

    std::istringstream ss(str);
    ss >> std::get_time(&t, format);

    return timegm(&t) - timezone;
  } catch (...) {
    return 0;
  }
}

void str2time(const char* str, const char* format, tm& t) {
  try {
    memset(&t, 0, sizeof(tm));

    std::istringstream ss(str);
    ss >> std::get_time(&t, format);
  } catch (...) {
    return;
  }
}

TimeStamp time2timestampUTC(tm& t) { return timegm(&t) - timezone; }

CR2END
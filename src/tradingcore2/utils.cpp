
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

// calcQuartile - sortedArr is a sorted array
void calcQuartile(float* sortedArr, size_t stride, size_t n, float& q1,
                  float& q2, float& q3) {
  assert(n > 4);

  int i1 = (n + 1) / 4;
  int i2 = (n + 1) / 2;
  int i3 = (n + 1) * 3 / 4;
  int o1 = (n + 1) % 4;
  int o2 = (n + 1) % 2;
  int o3 = (n + 1) * 3 % 4;

  if (o1 == 0) {
    q1 = sortedArr[i1 - 1];
  } else {
    q1 = sortedArr[i1 - 1] * (4 - o1) / 4.0 + sortedArr[i1] * o1 / 4.0;
  }

  if (o2 == 0) {
    q2 = sortedArr[i2 - 1];
  } else {
    q2 = sortedArr[i2 - 1] / 2.0 + sortedArr[i2] / 2.0;
  }

  if (o3 == 0) {
    q3 = sortedArr[i3 - 1];
  } else {
    q3 = sortedArr[i3 - 1] * (4 - o3) / 4.0 + sortedArr[i3] * o3 / 4.0;
  }
}

CR2END

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tradingcore2/utils.h>

#include <iomanip>
#include <sstream>

#include "version.h"

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
  // https://baike.baidu.com/item/%E5%9B%9B%E5%88%86%E4%BD%8D%E6%95%B0
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

// joinPath - "/dir1", "f0.txt" => "/dir1/f0.txt"
//            "/dir1/", "f0.txt" => "/dir1/f0.txt"
std::string joinPath(const char* root, const char* fn) {
  assert(root != NULL);
  assert(fn != NULL);

  std::string str = root;

  if (str[str.size() - 1] == '/') {
    str += fn;
  } else {
    str += '/';
    str += fn;
  }

  return str;
}

// scaleValue - 1.57, 0.05 -> 1.60
//              -0.23, 0.05 -> -0.25
IndicatorDataValue scaleValue(IndicatorDataValue val, IndicatorDataValue off) {
  assert(off > 0);

  if (val > 0) {
    auto ret = val / off;
    int iret = ret;
    if (iret == ret) {
      return val;
    }

    return (iret + 1) * off;
  } else if (val < 0) {
    auto ret = val / off;
    int iret = ret;
    if (iret == ret) {
      return val;
    }

    return (iret - 1) * off;
  }

  return 0;
}

// foreachPathWithExt - for each path with ext filename
void foreachPathWithExt(const char* dir, const char* extfn,
                        FuncOnForEachFile func) {
  auto fd = opendir(dir);
  if (fd == NULL) {
    return;
  }

  dirent* cd;
  while ((cd = readdir(fd)) != NULL) {
    auto ci = strstr(cd->d_name, extfn);
    if (ci - cd->d_name == strlen(cd->d_name) - strlen(extfn)) {
      func(dir, cd->d_name);
    }
  }

  closedir(fd);
}

void timestamp2timeUTC(time_t ts, tm& t) { gmtime_r(&ts, &t); }

// getYearWeek - get weeks of the year with ts
int getYearWeek(time_t ts) {
  tm ctm;
  tm fctm;

  timestamp2timeUTC(ts, ctm);
  if (ctm.tm_yday == 0) {
    return 0;
  }

  int cw = ctm.tm_yday / 7;

  char buf[1024];
  sprintf(buf, "%d0101", ctm.tm_year + 1900);
  auto fts = tr2::str2timestampUTC(buf, "%Y%m%d");

  timestamp2timeUTC(fts, fctm);
  // 如果元旦是周5，那么除元旦以外的周1-周5都+1
  if (ctm.tm_wday <= fctm.tm_wday) {
    return cw + 1;
  }

  return cw;
}

// getYearMonth - get month of the year with ts
int getYearMonth(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);

  return ctm.tm_mon;
}

// getYear - get year
int getYear(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);

  return ctm.tm_year + 1900;
}

// getDate - get date, it's like 20201001
int getDate(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);

  return (ctm.tm_year + 1900) * 10000 + (ctm.tm_mon + 1) * 100 + ctm.tm_mday;
}

const char* getVersion() { return TC2_VERSION; }

CR2END
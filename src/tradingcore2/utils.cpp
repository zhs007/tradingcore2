
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

  char buf[1024];
  sprintf(buf, "%d0101", ctm.tm_year + 1900);
  auto fts = tr2::str2timestampUTC(buf, "%Y%m%d");

  timestamp2timeUTC(fts, fctm);

  return (ctm.tm_yday + 1 + fctm.tm_wday) / 7;
}

// getYearWeekEx - it's like 202001
int getYearWeekEx(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);
  auto year = ctm.tm_year + 1900;
  int yw = getYearWeek(ts);

  return year * 100 + yw;
}

// getYearMonth - get month of the year with ts
int getYearMonth(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);

  return ctm.tm_mon;
}

// getYearMonthEx - it's like 202001
int getYearMonthEx(time_t ts) {
  tm ctm;

  timestamp2timeUTC(ts, ctm);
  auto year = ctm.tm_year + 1900;
  int ym = getYearMonth(ts);

  return year * 100 + ym;
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

// calcWeekOffWithWeekDay -
// 主要用于定投，计算2个时间点之间的周差，以特定周几为基准
// 譬如以周三为基准，这周二相对上周五其实不足一周
// 譬如以周三为基准，这周三相对上周五是一周
int calcWeekOffWithWeekDay(time_t lastts, time_t ts, int weekday) {
  tm t;
  timestamp2timeUTC(ts, t);

  // 如果第一次，只有明确是周几，才返回
  if (lastts == 0) {
    if (t.tm_wday == weekday) {
      return 1;
    }
  } else {
    tm lt;
    timestamp2timeUTC(lastts, lt);

    // 如果是一天，返回false
    if (lt.tm_year == t.tm_year && lt.tm_yday == t.tm_yday) {
      return 0;
    }

    // 如果上一次就没到
    if (lt.tm_wday < weekday) {
      auto nextweekts = getNextSunday(lastts);
      // 如果还在这一周，只有当前时间过了，才返回1，否则返回0
      if (ts < nextweekts) {
        if (t.tm_wday >= weekday) {
          return 1;
        }

        return 0;
      } else {
        auto cnextweekts = getNextSunday(ts);
        auto wd = (cnextweekts - nextweekts) / (7 * 24 * 60 * 60);
        // 如果过了至少一周，如果当前时间过了，返回周差+1
        // 如果刚好，返回周差+1
        // 如果不到，返回周差
        if (t.tm_wday >= weekday) {
          return wd + 1;
        }

        return wd;
      }
    } else if (lt.tm_wday >= weekday) {
      auto nextweekts = getNextSunday(lastts);
      // 如果还在这一周，返回0
      if (ts < nextweekts) {
        return 0;
      } else {
        auto cnextweekts = getNextSunday(ts);
        auto wd = (cnextweekts - nextweekts) / (7 * 24 * 60 * 60);

        // 如果过了至少一周，如果当前时间过了，返回周差
        // 如果刚好，返回周差
        // 如果不到，返回周差-1
        if (t.tm_wday >= weekday) {
          return wd;
        }

        return wd - 1;
      }
    }
  }

  return 0;
}

// calcMonthOffWithMonthDay -
// 主要用于定投，计算2个时间点之间的月差，以特定几号为基准
// 譬如以30号为基准，3月31号相对1月30号是2个月
int calcMonthOffWithMonthDay(time_t lastts, time_t ts, int monthday) {
  tm t;
  timestamp2timeUTC(ts, t);

  // 如果第一次，大于等于就返回
  if (lastts == 0) {
    if (t.tm_mday >= monthday) {
      return 1;
    }
  } else {
    tm lt;
    timestamp2timeUTC(lastts, lt);

    // 如果是一天，返回0
    if (lt.tm_year == t.tm_year && lt.tm_yday == t.tm_yday) {
      return 0;
    }

    // 如果上一次就没到
    if (lt.tm_mday < monthday) {
      auto nextmonthts = getNextMonth(lastts);
      // 如果还在这一个月，只有当前时间过了，才返回1，否则返回0
      if (ts < nextmonthts) {
        if (t.tm_mday >= monthday) {
          return 1;
        }

        return 0;
      } else {
        auto cnextmonthts = getNextMonth(ts);
        auto md = calcMonthOff(cnextmonthts, nextmonthts);
        // 如果过了至少一月，如果当前时间过了，返回月差+1
        // 如果刚好，返回月差+1
        // 如果不到，返回月差
        if (t.tm_mday >= monthday) {
          return md + 1;
        }

        return md;
      }
    } else if (lt.tm_mday >= monthday) {
      auto nextmonthts = getNextMonth(lastts);
      // 如果还在这一个月，返回0
      if (ts < nextmonthts) {
        return 0;
      } else {
        auto cnextmonthts = getNextMonth(ts);
        auto md = calcMonthOff(cnextmonthts, nextmonthts);

        // 如果过了至少一个月，如果当前时间过了，返回月差
        // 如果刚好，返回月差
        // 如果不到，返回月差-1
        if (t.tm_mday >= monthday) {
          return md;
        }

        return md - 1;
      }
    }
  }

  return 0;
}

// getNextSunday - 返回下一个周日的0点0分0秒
time_t getNextSunday(time_t ts) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  auto cts = ts - ctm.tm_hour * 60 * 60 - ctm.tm_min * 60 - ctm.tm_sec;
  auto dayoff = 6 - ctm.tm_wday;

  return cts + (dayoff + 1) * 24 * 60 * 60;
}

// getNextMonth - 返回下一个月1号的0点0分0秒
time_t getNextMonth(time_t ts) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  ctm.tm_hour = 0;
  ctm.tm_min = 0;
  ctm.tm_sec = 0;

  if (ctm.tm_mon == 11) {
    ctm.tm_year++;
    ctm.tm_mon = 0;
  } else {
    ctm.tm_mon++;
  }

  ctm.tm_mday = 1;

  ctm.tm_wday = 0;
  ctm.tm_yday = 0;
  ctm.tm_gmtoff = 0;

  return timegm(&ctm) - timezone;
}

// calcMonthOff - 计算月差，仅考虑月差，12月1号和11月31号，相差1个月
int calcMonthOff(time_t ts0, time_t ts1) {
  if (ts1 == ts0) {
    return 0;
  } else if (ts1 < ts0) {
    std::swap(ts0, ts1);
  }

  tm ctm0;
  timestamp2timeUTC(ts0, ctm0);
  tm ctm1;
  timestamp2timeUTC(ts1, ctm1);

  auto yoff = ctm1.tm_year - ctm0.tm_year;
  if (yoff == 0) {
    return ctm1.tm_mon - ctm0.tm_mon;
  }

  return yoff * 12 + ctm1.tm_mon - ctm0.tm_mon;
}

// getTotalYearDays - 计算这一年的天数，闰年366天，否则365天
int getTotalYearDays(int year) {
  if (year % 400 == 0 or (year % 4 == 0 and year % 100 != 0)) {
    return 366;
  }

  return 365;
}

// countValues4Year - 根据value里的时间，和value数量，计算一年平均多少个value
float countValues4Year(time_t startts, time_t endts, int valuenums) {
  tm stm;
  tm etm;
  timestamp2timeUTC(startts, stm);
  timestamp2timeUTC(endts, etm);

  auto sty = stm.tm_yday / float(getTotalYearDays(stm.tm_year + 1900));
  auto ety = etm.tm_yday / float(getTotalYearDays(etm.tm_year + 1900));

  auto fy = etm.tm_year - stm.tm_year - 1 + 1 - sty + ety;

  return int(valuenums / fy);
}

const char* getVersion() { return TC2_VERSION; }

void logProtobuf(const char* title, const google::protobuf::Message& message) {
  std::string str;
  google::protobuf::util::MessageToJsonString(message, &str);

  LOG(INFO) << title << str;
}

void saveProtobuf(const char* fn, const google::protobuf::Message& message) {
  assert(fn != NULL);

  std::string str;
  google::protobuf::util::MessageToJsonString(message, &str);

  FILE* fp;

  fp = fopen(fn, "w");
  if (fp == NULL) {
    return;
  }

  fwrite(str.c_str(), str.size(), 1, fp);

  fclose(fp);
}

// splitStr - split a string
bool splitStr(std::vector<std::string>& arr, const char* str,
              const char* pattern) {
  arr.clear();

  char* strc = new char[strlen(str) + 1];
  strcpy(strc, str);

  char* tmp = strtok(strc, pattern);
  while (tmp != NULL) {
    arr.push_back(tmp);
    tmp = strtok(NULL, pattern);
  }
  delete[] strc;

  return true;
}

CR2END
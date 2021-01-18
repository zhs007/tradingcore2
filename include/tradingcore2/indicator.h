#ifndef __TRADINGCORE2_INDICATOR_H__
#define __TRADINGCORE2_INDICATOR_H__

#include <tradingcore2/basedef.h>

#include <map>
#include <string>

CR2BEGIN

struct IndicatorData_singleValue {
  IndicatorDataValue value;
};

enum IndicatorBuild2Type { IB2T_NONE, IB2T_DAY };

struct IndicatorParams {
  std::string name;
  int avgtimes;
  IndicatorBuild2Type b2type;
  int64_t b2OffTime;
  std::vector<std::string> assetsNames;

  void clear() {
    this->name.clear();
    this->avgtimes = 0;
    this->b2type = IB2T_NONE;
    this->b2OffTime = 0;
    this->assetsNames.clear();
  }
};

bool parseIndicatorParams(IndicatorParams& params, const char* fullname,
                          const char* assetname);

class Indicator {
 public:
  Indicator(const char* fullname, const char* assetsName)
      : m_fullname(fullname) {
    parseIndicatorParams(this->m_params, fullname, assetsName);
  }
  virtual ~Indicator() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length) = 0;

  virtual bool build2(Exchange& exchange, const char* assetsName,
                      const char* assetsName2, IndicatorBuild2Type b2t,
                      int64_t ot, int start, int length) = 0;

  virtual const IndicatorData_singleValue* getSingleValue(int index) const {
    assert(false && "unimplemented");

    return NULL;
  }

  virtual const IndicatorData_singleValue* getMinSingleValue(int& index) const {
    assert(false && "unimplemented");

    return NULL;
  }

  virtual const IndicatorData_singleValue* getMaxSingleValue(int& index) const {
    assert(false && "unimplemented");

    return NULL;
  }

  virtual TimeStamp getTimeStamp(int index) const = 0;

  virtual int getLength() const = 0;

 public:
  // saveCSV - save to a csv file
  void saveCSV(const char* fn);

 public:
  const IndicatorParams& getParams() const { return this->m_params; }

 protected:
  std::string m_fullname;
  IndicatorParams m_params;
};

CR2END

#endif  // __TRADINGCORE2_INDICATOR_H__
#ifndef __TRADINGCORE2_INDICATOR_H__
#define __TRADINGCORE2_INDICATOR_H__

#include <tradingcore2/basedef.h>
#include <map>
#include <string>

CR2BEGIN

struct IndicatorData_singleValue {
  IndicatorDataValue value;
};

class Indicator {
 public:
  Indicator() {}
  virtual ~Indicator() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length) = 0;

  virtual const IndicatorData_singleValue* getSingleValue(int index) const {
    assert(false && "unimplemented");

    return NULL;
  }

  virtual int getLength() const = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_INDICATOR_H__
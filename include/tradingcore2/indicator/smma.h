#ifndef __TRADINGCORE2_INDICATOR_SMMA_H__
#define __TRADINGCORE2_INDICATOR_SMMA_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <vector>

CR2BEGIN

//!! https://en.wikipedia.org/wiki/Moving_average

class IndicatorSMMA {
 public:
  struct Node {
    TimeStamp ts;
    IndicatorData_singleValue val;
  };

  typedef std::vector<Node> List;

 public:
  IndicatorSMMA(int avgtimes) : m_avgtimes(avgtimes), m_iStart(-1) {
    assert(avgtimes > 1);
  }
  virtual ~IndicatorSMMA() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length);

  virtual const IndicatorData_singleValue* getSingleValue(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return &(m_lst[index].val);
  }

  virtual int getLength() const { return m_lst.size(); }

 public:
  void release() { m_lst.clear(); }

  void pushData(TimeStamp ts, IndicatorDataValue val);

 protected:
  int m_avgtimes;
  List m_lst;
  int m_iStart;
};

CR2END

#endif  // __TRADINGCORE2_INDICATOR_SMMA_H__
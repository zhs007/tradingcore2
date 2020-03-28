#ifndef __TRADINGCORE2_INDICATOR_RSI_H__
#define __TRADINGCORE2_INDICATOR_RSI_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <vector>

CR2BEGIN

//!! https://en.wikipedia.org/wiki/Relative_strength_index

class IndicatorRSI : public Indicator {
 public:
  struct Node {
    TimeStamp ts;
    IndicatorDataValue u;
    IndicatorDataValue d;
    IndicatorDataValue smmau;
    IndicatorDataValue smmad;
    IndicatorData_singleValue val;
  };

  typedef std::vector<Node> List;

 public:
  IndicatorRSI(int avgtimes) : m_avgtimes(avgtimes), m_iStart(-1) {
    assert(avgtimes >= 1);
  }
  virtual ~IndicatorRSI() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length);

  virtual const IndicatorData_singleValue* getSingleValue(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return &(m_lst[index].val);
  }

  virtual TimeStamp getTimeStamp(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].ts;
  }

  virtual int getLength() const { return m_lst.size(); }

 public:
  void release() { m_lst.clear(); }

  void pushData(TimeStamp ts, IndicatorDataValue u, IndicatorDataValue d,
                IndicatorDataValue smmau, IndicatorDataValue smmad,
                IndicatorDataValue val);

  IndicatorDataValue getU(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].u;
  }

  IndicatorDataValue getD(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].d;
  }

  IndicatorDataValue getSMMAU(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].smmau;
  }

  IndicatorDataValue getSMMAD(int index) const {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].smmad;
  }

 protected:
  int m_avgtimes;
  List m_lst;
  int m_iStart;
};

// NewIndicatorRSI - new IndicatorRSI
Indicator* NewIndicatorRSI(int avgtimes);

CR2END

#endif  // __TRADINGCORE2_INDICATOR_RSI_H__
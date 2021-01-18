#ifndef __TRADINGCORE2_INDICATOR_RSI_H__
#define __TRADINGCORE2_INDICATOR_RSI_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <vector>

CR2BEGIN

//!! https://en.wikipedia.org/wiki/Relative_strength_index

class IndicatorRSI final : public Indicator {
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
  // newIndicator - new IndicatorRSI
  static Indicator* newIndicator(const char* fullname, const char* assetsName);
  // isMine - isMine
  static bool isMine(const char* name);

 protected:
  IndicatorRSI(const char* fullname, const char* assetsName)
      : Indicator(fullname, assetsName), m_iStart(-1) {
    m_avgtimes = this->m_params.avgtimes;
    assert(m_avgtimes >= 1);
  }
  virtual ~IndicatorRSI() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length) override;

  virtual bool build2(Exchange& exchange, const char* assetsName,
                      const char* assetsName2, IndicatorBuild2Type b2t,
                      int64_t ot, int start, int length) override;

  virtual const IndicatorData_singleValue* getSingleValue(
      int index) const override {
    assert(index >= 0);
    assert(index < m_lst.size());

    return &(m_lst[index].val);
  }

  virtual const IndicatorData_singleValue* getMinSingleValue(
      int& index) const override;

  virtual const IndicatorData_singleValue* getMaxSingleValue(
      int& index) const override;

  virtual TimeStamp getTimeStamp(int index) const override {
    assert(index >= 0);
    assert(index < m_lst.size());

    return m_lst[index].ts;
  }

  virtual int getLength() const override { return m_lst.size(); }

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

// // NewIndicatorRSI - new IndicatorRSI
// Indicator* NewIndicatorRSI(int avgtimes);

CR2END

#endif  // __TRADINGCORE2_INDICATOR_RSI_H__
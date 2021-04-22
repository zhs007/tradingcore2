#ifndef __TRADINGCORE2_INDICATOR_TA_RSI_H__
#define __TRADINGCORE2_INDICATOR_TA_RSI_H__

#include <ta_func.h>
#include <ta_libc.h>
#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <vector>

CR2BEGIN

//!! https://ta-lib.org/

class IndicatorTA_RSI final : public Indicator {
 public:
  struct Node {
    TimeStamp ts;
    IndicatorData_singleValue val;
  };

  typedef std::vector<Node> List;

 public:
  // newIndicator - new IndicatorTA_RSI
  static Indicator* newIndicator(const char* fullname, const char* assetsName);
  // isMine - isMine
  static bool isMine(const char* name);

 protected:
  IndicatorTA_RSI(const char* fullname, const char* assetsName)
      : Indicator(fullname, assetsName), m_iStart(-1) {
    m_avgtimes = this->m_params.avgtimes;
    assert(m_avgtimes >= 1);
  }
  virtual ~IndicatorTA_RSI() {}

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

  void pushData(TimeStamp ts, IndicatorDataValue val);

 protected:
  void _buildFirst(Exchange& exchange, const char* assetsName, int start,
                   int length, Money& totalPrice);

  bool _build_avg1(Exchange& exchange, const char* assetsName, int start,
                   int length);

 protected:
  int m_avgtimes;
  List m_lst;
  int m_iStart;
};

// // NewIndicatorTA_MA - new IndicatorTA_MA
// Indicator* NewIndicatorTA_MA(int avgtimes);

CR2END

#endif  // __TRADINGCORE2_INDICATOR_TA_RSI_H__
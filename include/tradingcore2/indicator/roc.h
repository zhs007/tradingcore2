#ifndef __TRADINGCORE2_INDICATOR_ROC_H__
#define __TRADINGCORE2_INDICATOR_ROC_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <vector>

CR2BEGIN

// https://school.stockcharts.com/doku.php?id=technical_indicators:rate_of_change_roc_and_momentum

class IndicatorROC final : public Indicator {
 public:
  struct Node {
    TimeStamp ts;
    IndicatorData_singleValue val;
  };

  typedef std::vector<Node> List;

 public:
  IndicatorROC(int avgtimes) : m_avgtimes(avgtimes), m_iStart(-1) {
    assert(avgtimes >= 1);
  }
  virtual ~IndicatorROC() {}

 public:
  virtual bool build(Exchange& exchange, const char* assetsName, int start,
                     int length) override;

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
                   int length);

 protected:
  int m_avgtimes;
  List m_lst;
  int m_iStart;
};

// NewIndicatorROC - new IndicatorROC
Indicator* NewIndicatorROC(int avgtimes);

CR2END

#endif  // __TRADINGCORE2_INDICATOR_ROC_H__
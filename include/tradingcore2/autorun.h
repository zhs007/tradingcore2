#ifndef __TRADINGCORE2_AUTORUN_H__
#define __TRADINGCORE2_AUTORUN_H__

#include <tradingcore2/basedef.h>

CR2BEGIN

template <typename T>
class AutoIncDec {
 public:
  AutoIncDec(T* pObj) : m_pObj(pObj) { (*m_pObj)++; }
  ~AutoIncDec() { (*m_pObj)--; }

 protected:
  T* m_pObj;
};

CR2END

#endif  // __TRADINGCORE2_AUTORUN_H__
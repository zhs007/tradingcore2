#ifndef __TRADINGCORE2_SPINLOCK_H__
#define __TRADINGCORE2_SPINLOCK_H__

#include <tradingcore2/basedef.h>

#include <atomic>
#include <thread>

CR2BEGIN

template <typename T>
class SpinLockMaxVal {
 public:
  SpinLockMaxVal(std::atomic<T>* pObj, T maxval) : m_pObj(pObj) {
    T val;
  spinlockmaxval_con_start:
    while ((val = (*m_pObj)) >= maxval) {
      // LOG(DEBUG) << "val : " << val << " maxval : " << maxval;
      std::this_thread::yield();
    }

    // LOG(DEBUG) << "val : " << val;

    T cv = val;
    if (!m_pObj->compare_exchange_strong(cv, val + 1)) {
      goto spinlockmaxval_con_start;
    }
  }

  ~SpinLockMaxVal() { (*m_pObj)--; }

 protected:
  std::atomic<T>* m_pObj;
};

CR2END

#endif  // __TRADINGCORE2_SPINLOCK_H__
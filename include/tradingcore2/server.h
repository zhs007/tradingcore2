#ifndef __TRADINGCORE2_SERVER_H__
#define __TRADINGCORE2_SERVER_H__

#include <tradingcore2/basedef.h>

#include <string>

CR2BEGIN

class Server {
 public:
  Server() {}
  virtual ~Server() {}

 public:
  virtual void run() = 0;

 protected:
};

CR2END

#endif  // __TRADINGCORE2_SERVER_H__
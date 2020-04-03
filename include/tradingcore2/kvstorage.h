#ifndef __TRADINGCORE2_KVSTORAGE_H__
#define __TRADINGCORE2_KVSTORAGE_H__

#include <rocksdb/c.h>
#include <tradingcore2/basedef.h>

#include <string>

CR2BEGIN

class KVStorage {
 public:
  KVStorage() : m_db(NULL), m_options(NULL) {}
  ~KVStorage() { this->close(); }

 public:
  bool open(const char* dirName);

  void close();

 protected:
  rocksdb_t* m_db;
  rocksdb_options_t* m_options;
};

CR2END

#endif  // __TRADINGCORE2_KVSTORAGE_H__
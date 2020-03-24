#ifndef __TRADINGCORE2_CSV_H__
#define __TRADINGCORE2_CSV_H__

#include <stdio.h>
#include <tradingcore2/basedef.h>

#include <functional>
#include <string>

CR2BEGIN

class CSVLoader {
 public:
  CSVLoader()
      : m_buff(NULL), m_width(0), m_height(0), m_head(NULL), m_data(NULL) {}
  ~CSVLoader() { this->release(); }

 public:
  void load(const char* fn);

  void release();

 public:
  int getWidth() const { return this->m_width; }

  int getHeight() const { return this->m_height; }

  const char* getHead(int x) const {
    assert(x >= 0);
    assert(x < this->m_width);

    return m_head[x];
  }

  const char* getData(int x, int y) const {
    assert(x >= 0);
    assert(x < this->m_width);
    assert(y >= 0);
    assert(y < this->m_height);

    return m_data[y][x];
  }

 protected:
  char* m_buff;
  int m_width, m_height;
  char** m_head;
  char*** m_data;
};

// FuncOnSaveCSVRow - for saveCSV, row starts at 0
//      - If it returns false, the save is complete
typedef std::function<bool(FILE* fp, int row)> FuncOnSaveCSVRow;
// FuncOnSaveCSVHead - for saveCSV
typedef std::function<void(FILE* fp)> FuncOnSaveCSVHead;

// saveCSV - save a csv file
void saveCSV(const char* fn, FuncOnSaveCSVHead funcOnHead,
             FuncOnSaveCSVRow funcOnRow);

CR2END

#endif  // __TRADINGCORE2_CSV_H__
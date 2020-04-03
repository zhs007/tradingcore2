#include <tradingcore2/kvstorage.h>
#include <unistd.h>

CR2BEGIN

bool KVStorage::open(const char* dirName) {
  m_options = rocksdb_options_create();

  long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
  rocksdb_options_increase_parallelism(m_options, (int)(cpus));
  rocksdb_options_optimize_level_style_compaction(m_options, 0);
  // create the DB if it's not already present
  rocksdb_options_set_create_if_missing(m_options, 1);

  char* err = NULL;
  m_db = rocksdb_open(m_options, dirName, &err);
  assert(!err);

  return true;
}

void KVStorage::close() {
  if (m_options != NULL) {
    rocksdb_options_destroy(m_options);

    m_options = NULL;
  }

  if (m_db != NULL) {
    rocksdb_close(m_db);

    m_db = NULL;
  }
}

CR2END
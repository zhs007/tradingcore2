cd $PWD/deps/rocksdb
mkdir build
cd build
cmake -DROCKSDB_BUILD_SHARED=OFF ..
make rocksdb
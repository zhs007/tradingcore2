git submodule init
git submodule update --recursive
git submodule update --init --recursive

cd deps/gsl
mkdir build
cd build
cmake ..
make
cd..
cd..

cd deps/grpc/third_party
rm -rf zlib
git submodule init
git submodule update
cd..
cd..
cd..

cd deps/gflags
mkdir cmakebuild
cd cmakebuild
cmake ..
make
make install
cd..
cd..

cd deps/rocksdb
mkdir build
cd build
cmake ..
make
cd..
cd..
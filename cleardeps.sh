sh cleargflags.sh

rm -rf $PWD/deps/glog/cmakebuild
rm -rf $PWD/deps/grpc/cmakebuild
rm -rf $PWD/deps/googletest/build
rm -rf $PWD/deps/gsl/build
rm -rf $PWD/deps/rocksdb/build
rm -rf $PWD/deps/yaml-cpp/build
rm -rf $PWD/deps/zlib/build

sh cleartalib.sh
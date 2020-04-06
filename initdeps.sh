git submodule init
git submodule update --recursive
git submodule update --init --recursive

sh buildgsl.sh

sh fixgrpc.sh

sh buildgflags.sh

sh buildrocksdb.sh

sh buildyamlcpp.sh
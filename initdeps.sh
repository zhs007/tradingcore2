git submodule init
git submodule update --recursive
git submodule update --init --recursive

sh buildgoogletest.sh

sh buildgsl.sh

sh fixgrpc.sh

sh buildgrpc.sh

sh buildglog.sh

sh buildgflags.sh

sh buildrocksdb.sh

sh buildyamlcpp.sh

sh buildtalib.sh
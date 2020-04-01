git submodule init
git submodule update --recursive
git submodule update --init --recursive

cd deps/gsl
mkdir build
cd build
cmake ..
make
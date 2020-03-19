git submodule init
git submodule update --recursive

cd deps/gsl
mkdir build
cd build
cmake ..
make
cd deps/grpc
mkdir cmakebuild
cd cmakebuild
cmake ..
# make grpc grpc++ grpc++_reflection
make
make install
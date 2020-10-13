rm -r build
rm -r lib
rm -r bin
mkdir -p build
mkdir -p lib
mkdir -p bin
(cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..)
(cd build && make)
(cd build && make install)

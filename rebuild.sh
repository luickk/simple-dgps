rm -r build
rm -r lib
rm -r bin
mkdir -p build
mkdir -p lib
mkdir -p bin
(cd build && cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..)
(cd build && make)
(cd build && make install)
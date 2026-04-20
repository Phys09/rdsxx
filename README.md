# rdsxx
Making a Redis clone, using C++ with the asio library (without boost)

Put the asio library files in ```vendored/asio``` to build.

Early commits also go through some of the asio tutorials

# Building
Standard Cmake + ninja-make project

Uses LLVM's clang compiler. If on windows, use `clang-cl` as the compiler driver

```
mkdir build
cd build
cmake ..
cmake --build .
```

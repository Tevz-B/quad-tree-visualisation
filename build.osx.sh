# !/bin/sh
## install raylib on macos with brew
# brew install raylib

mkdir -p build && cmake -B build && cmake --build build

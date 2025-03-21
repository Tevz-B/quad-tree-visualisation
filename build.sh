# !/bin/sh

## install raylib on ubuntu with apt
# sudo add-apt-repository ppa:texus/raylib
# sudo apt install libraylib5-dev -y

mkdir -p build && cmake -B build && make -C build



#!/bin/bash
#/bin/sh -> bash 지원 안될 때 #앞에 !붙여서 사용

# Update Ubuntu package
# sudo apt update
# sudo apt upgrade
# sudo apt install git cmake gcc build-essential libssl-dev valgrind

# Qt (for gui)
# sudo apt install qt5-qmake qtbase5-dev

# build openKBC
cmake -B build -S . && cd build
make -j$(nproc)

# build Qt
cd ../KBC_gui
mkdir build
cd build
qmake ..
make -j$(nproc)
# export LD_LIBRARY_PATH=../:$LD_LIBRARY_PATH

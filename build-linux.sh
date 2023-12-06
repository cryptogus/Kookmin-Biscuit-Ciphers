# build openKBC
cmake -B build -S . && cd build
make -j$(nproc)

# build Qt
cd ../KBC_gui
mkdir build
cd build
qmake ..
make -j$(nproc)``
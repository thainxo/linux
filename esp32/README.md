# Build emulator for ESP32C3
Refrence: 
1. https://github.com/espressif/qemu#
2. https://github.com/espressif/esp-toolchain-docs/blob/main/qemu/esp32c3/README.md
## Pre-build
```sh
sudo apt install ninja-build python3-sphinx libsdl2-dev
```
## Build
```sh
git clone https://gitlab.com/qemu-project/qemu.git
cd qemu
mkdir build
cd build
../configure --target-list=riscv32-softmmu     --enable-gcrypt    \
    --enable-slirp     --enable-debug     --enable-sdl     --disable-strip \
    --disable-user     --disable-capstone --disable-vnc     --disable-gtk
make
```
## Install dependence packages to run
```sh
sudo apt install ninja-build python3-sphinx libsdl2-2.0-0 libsndio7.0 libusb-1.0-0
```
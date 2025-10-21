# Install compatible packages on old linux
install-libs.sh
## Install libraries to compile packages
```sh
sudo apt-get update
sudo apt-get install -y gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
    python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 xz-utils unzip \
    patch rsync meson ninja-build
```
## Setup environement and packages to run vscode on ole linux
### Compile crosstool
```sh
cd ${HOME}
wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.26.0.tar.bz2
tar -xjf crosstool-ng-1.26.0.tar.bz2
cd crosstool-ng-1.26.0 && ./configure --prefix=${HOME}/crosstool-ng-1.26.0/out && make && make install
export PATH=$PATH:${HOME}/crosstool-ng-1.26.0/out/bin
```
### setup packages
```sh
mkdir -p ${HOME}/toolchain-dir
cd ${HOME}/toolchain-dir
ct-ng x86_64-unknown-linux-gnu
ct-ng menuconfig # kernel 5.16, compiler v9.5, disable gdb
ct-ng build
```
### setup patchelf
```sh
mkdir -p ${HOME}/patchelf
cd ${HOME}/patchelf
wget -c https://github.com/NixOS/patchelf/releases/download/0.15.5/patchelf-0.15.5-x86_64.tar.gz
tar -xvf patchelf-0.15.5-x86_64.tar.gz
```

# Setup environment to run vscode
add below variables to /etc/environment
```sh
export VSCODE_SERVER_CUSTOM_GLIBC_LINKER="<home>/x-tools/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib/ld-linux-x86-64.so.2"
export VSCODE_SERVER_CUSTOM_GLIBC_PATH="<home>/x-tools/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib"
export VSCODE_SERVER_PATCHELF_PATH="<home>/x-tools/patchelf/bin/patchelf"
```

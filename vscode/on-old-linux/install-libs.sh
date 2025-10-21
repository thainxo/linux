sudo apt-get update
sudo apt-get install -y gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
    python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 xz-utils unzip \
    patch rsync meson ninja-build

cd ${HOME}
wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.26.0.tar.bz2
tar -xjf crosstool-ng-1.26.0.tar.bz2
cd crosstool-ng-1.26.0 && ./configure --prefix=${HOME}/crosstool-ng-1.26.0/out && make && make install
export PATH=$PATH:${HOME}/crosstool-ng-1.26.0/out/bin

mkdir -p ${HOME}/toolchain-dir
cd ${HOME}/toolchain-dir
ct-ng x86_64-unknown-linux-gnu
ct-ng menuconfig # kernel 5.16, compiler v9.5, disable gdb
ct-ng build

mkdir -p ${HOME}/patchelf
cd ${HOME}/patchelf
wget -c https://github.com/NixOS/patchelf/releases/download/0.15.5/patchelf-0.15.5-x86_64.tar.gz
tar -xvf patchelf-0.15.5-x86_64.tar.gz


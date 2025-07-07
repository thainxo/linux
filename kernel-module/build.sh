# Download source code
# Ubuntu 24.04
sudo sed -i 's/Types: deb$/Types: deb deb-src/g' /etc/apt/sources.list.d/ubuntu.sources
sudo apt update
apt source linux-source-$(uname -r | awk -F- '{print $1}')

# Prepare tool
sudo apt install -y linux-headers-$(uname -r) libelf-dev
cd linux-$(uname -r | awk -F- '{print $1}')
make oldconfig
make prepare
make scripts
cp -v /usr/src/linux-headers-$(uname -r)/Module.symvers .

# Build kernel module
cd linux-$(uname -r | awk -F- '{print $1}')/fs/qnx6
make ARCH=x86_64 -C /lib/modules/$(uname -r)/build M=$(pwd) modules
make ARCH=x86_64 -C /lib/modules/$(uname -r)/build M=$(pwd) modules_install

# Load to system
sudo modprobe /home/worker/workspace/linux-6.8.0/fs/qnx6/qnx6.ko 

# mount disk
sudo mount -t qnx6 -o <qnx6.img> test/

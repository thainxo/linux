export ANDROID_BUILD_TOP=~/workspace/android10-gsi
mkdir -p ${ANDROID_BUILD_TOP}

cd ${ANDROID_BUILD_TOP}
# Initialize and sync the repo
repo init -u https://android.googlesource.com/platform/manifest -b android10-gsi
repo sync -j16

# Install lib to build android
sudo dpkg -i packages/libncurses5_6.2-0ubuntu2.1_amd64.deb
sudo dpkg -i packages/libtinfo5_6.2-0ubuntu2.1_amd64.deb
# BUILD
```sh
# generate command line to build
# export NINJA_ARGS="-t commands"
source build/envsetup.sh
lunch aosp_cf_x86_64_phone-userdebug
m -j16
```

# Package super.img
```sh
source build/envsetup.sh
lunch <PRODUCT_NAME>-<BUILD_VARIANT>
out/host/linux-x86/bin/lpmake build/make/tools/releasetools/build_super_image.py out/target/product/vsoc_x86_64/odm.img out/target/product/vsoc_x86_64/product.img out/target/product/vsoc_x86_64/system.img out/target/product/vsoc_x86_64/vendor.img out/target/product/vsoc_x86_64/system_other.img
how to build single module in android
# In ~/.bashrc or ~/.zshrc
./lpmake --metadata-size 65536 --super-name super --metadata-slots 1 --device super:4294967296 --group main:3139354624 --partition system:readonly:1434726400:main --image system=./system.img --partition vendor:readonly:330866688:main --image vendor=./vendor.img --partition product:readonly:1373761536:main --image product=./product.img --sparse --output ./super.new.img

export PYTHONPATH=${ANDROID_BUILD_TOP}/external/python/cpython2/Lib:/home/worker/workspace/android10-gsi/prebuilts/python/linux-x86/2.7.5/lib
export PYTHONHOME=/home/worker/workspace/android10-gsi/prebuilts/python/linux-x86/2.7.5
export PATH=/home/worker/workspace/android10-gsi/prebuilts/python/linux-x86/2.7.5/bin:$PATH
export PYTHON_COMMAND=/home/worker/workspace/android10-gsi/prebuilts/python/linux-x86/2.7.5/bin
PATH=out/soong/.intermediates/external/python/cpython2/py2-launcher-lib/linux_glibc_x86_64_static:out/host/linux-x86/bin/:$PATH 	    build/make/tools/releasetools/build_super_image.py -v out/target/product/vsoc_x86_64/obj/PACKAGING/target_files_intermediates/aosp_cf_x86_64_phone-target_files-eng.worker out/target/product/vsoc_x86_64/obj/PACKAGING/super.img_intermediates/super.img

/bin/bash -c "\
    (mkdir -p out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/ ) && \
    (rm -rf  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"use_dynamic_partitions=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"lpmake=lpmake\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"build_super_partition=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_metadata_device=super\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_block_devices=super\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_super_device_size=8321499136\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"dynamic_partition_list= odm product system vendor\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_partition_groups=google_dynamic_partitions\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_google_dynamic_partitions_group_size=4160749568\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"super_google_dynamic_partitions_partition_list=odm product system vendor\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"build_non_sparse_super_partition=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_image_in_update_package=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"ab_update=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"odm_image=out/target/product/vsoc_x86_64/odm.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"product_image=out/target/product/vsoc_x86_64/product.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"system_image=out/target/product/vsoc_x86_64/system.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"vendor_image=out/target/product/vsoc_x86_64/vendor.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"system_other_image=out/target/product/vsoc_x86_64/system_other.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (mkdir -p out/target/product/vsoc_x86_64/ ) && \
    (PATH=out/host/linux-x86/bin/:\$PATH build/make/tools/releasetools/build_super_image.py -v  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt out/target/product/vsoc_x86_64/super.img )"
```

cd /home/worker/workspace/android10-gsi && rm -f out/target/product/vsoc_x86_64/super.img && out/host/linux-x86/bin/lpmake \
  --metadata-size 65536 \
  --super-name super \
  --metadata-slots 2 \
  --device super:8321499136 \
  --group google_dynamic_partitions:4160749568 \
  --partition system:readonly:1048223744:google_dynamic_partitions \
  --image system=out/target/product/vsoc_x86_64/system.img \
  --partition vendor:readonly:163127296:google_dynamic_partitions \
  --image vendor=out/target/product/vsoc_x86_64/vendor.img \
  --partition product:readonly:358367232:google_dynamic_partitions \
  --image product=out/target/product/vsoc_x86_64/product.img \
  --partition odm:readonly:626688:google_dynamic_partitions \
  --image odm=out/target/product/vsoc_x86_64/odm.img \
  --output out/target/product/vsoc_x86_64/super.img
  
sudo groupadd cvdnetwork
sudo usermod -aG kvm,cvdnetwork,render worker

sudo apt-get update
sudo  apt-get install -y bridge-utils dnsmasq-base f2fs-tools  libfdt1 libusb-1.0-0
sudo apt install -y efibootmgr grub-common grub-efi-ia32-bin libarchive-tools libefiboot1t64 libefivar1t64 os-prober
sudo dpkg -i /home/worker/workspace/android/cf_packages/cuttlefish-*
HOME=$(pwd) ./bin/launch_cvd -start_vnc_server
echo "Build Success"

/bin/bash -c " \
    (mkdir -p out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/ ) && \
    (rm -rf  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"use_dynamic_partitions=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"lpmake=lpmake\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"build_super_partition=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_metadata_device=super\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_block_devices=super\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_super_device_size=8321499136\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"dynamic_partition_list= odm product system vendor\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_partition_groups=google_dynamic_partitions\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_google_dynamic_partitions_group_size=4160749568\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"super_google_dynamic_partitions_partition_list=odm product system vendor\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"build_non_sparse_super_partition=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"super_image_in_update_package=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"ab_update=true\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"odm_image=out/target/product/vsoc_x86_64/odm.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"product_image=out/target/product/vsoc_x86_64/product.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"system_image=out/target/product/vsoc_x86_64/system.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt;  echo \"vendor_image=out/target/product/vsoc_x86_64/vendor.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (echo \"system_other_image=out/target/product/vsoc_x86_64/system_other.img\" >>  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt ) && \
    (mkdir -p out/target/product/vsoc_x86_64/ ) && \
    (PATH=out/host/linux-x86/bin/:$PATH build/make/tools/releasetools/build_super_image.py -v  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt out/target/product/vsoc_x86_64/super.img )"
PATH=/home/worker/workspace/android10-gsi/prebuilts/python/linux-x86/2.7.5/bin:out/host/linux-x86/bin/:$PATH  build/make/tools/releasetools/build_super_image.py -v  out/target/product/vsoc_x86_64/obj/PACKAGING/superimage_debug_intermediates/misc_info.txt out/target/product/vsoc_x86_64/super.img
# Install grub tools
```sh
sudo apt install -y grub2 dosfstools mtools
sudo apt install -y grub-efi
sudo apt install -y grub-pc
```

# Create partitions
Use parted or fdisk to partition the USB drive. A typical setup includes:
EFI System Partition (ESP): A small FAT32 partition (e.g., 512MB) for UEFI boot.
BIOS Boot Partition: A small, unformatted partition (e.g., 2MB) for legacy BIOS boot.
Root Partition: The main partition for the Debian system (e.g., ext4). 
```sh
parted /dev/sdX mklabel gpt
parted /dev/sdX mkpart primary 1MiB 3MiB
parted /dev/sdX set 1 bios_grub on
parted /dev/sdX mkpart primary ext4 3MiB 65MiB
parted /dev/sdX mkpart primary fat32 65MiB 129MiB
parted /dev/sdX set 3 boot on
parted /dev/sdX mkpart primary ext4 129MiB 100%
```
# Format partitions
```sh
# The BIOS boot partition doesn't need a filesystem
mkfs.ext4 -F /dev/sdX2
mkfs.vfat -F32 /dev/sdX3
mkfs.ext4 -F /dev/sdX4
```
# Mount sdX to directory /mnt/usb
```sh
mkdir -p /media/usb
mount /dev/sdX2 /media/usb
mkdir -p /media/usb/efi
mount /dev/sdX3 /media/usb/efi
```

# Install grub efi
```sh
grub-install --target=x86_64-efi \
    --root-directory=/media/usb \
    --efi-directory=/media/usb/efi \
    --boot-directory=/media/usb/boot \
    --removable --no-floppy --no-nvram \
    --bootloader-id=GRUB \
    --recheck
```

# Install grub BIOS
```sh
grub-install --target=i386-pc \
    --root-directory=/media/usb \
    --boot-directory=/media/usb/boot \
    --recheck \
    /dev/sdX
```
# Support boot Windows (wimboot)
Download wimboot from https://github.com/ipxe/wimboot.git
cp wimboot to efi partitions
```sh
cp wimboot /media/usb
```

# Create grub config
```sh
mkdir -p /media/usb/boot/grub
cat <<EOF > /media/usb/boot/grub/grub.cfg
set timeout=5
set default=0
menuentry "Boot from USB" {
    insmod part_gpt
    insmod ext2
    set root=(hd0,gpt1)
    linux /vmlinuz root=/dev/sdX1 ro quiet splash
    initrd /initrd.img
}
menuentry "Boot from Hard Drive" {
    insmod part_gpt
    insmod ext2
    set root=(hd0,gpt2)
    linux /vmlinuz root=/dev/sdY1 ro quiet splash
    initrd /initrd.img
}

menuentry "Windows 11" {
    insmod part_gpt
    insmod ntfs
    set root=(hd0,gpt2)
    set isofile=/os/windows/Win11_24H2_English_x64.iso
    loopback loop (hd0,gpt3)$isofile
    linux16 /vmlinuz root=/dev/sdY1 ro quiet splash
    initrd /initrd.img
}
EOF
```

# Note
## Fix bug out of memory when booting in efi mode with tpm is enabled
```sh
rmmod tpm
``` 

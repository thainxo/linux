# Environment
MOUNT_POINT=/tmp/usb
USB_DRIVE=/dev/sdX
BIOS_PARTITION_ID=1
EFI_PARTITION_ID=2
DATA_PARTITION_ID=3

# Install grub tools
sudo apt install -y grub2 grub-efi grub-pc dosfstools mtools

# Create partitions
parted ${USB_DRIVE} mklabel gpt
parted ${USB_DRIVE} mkpart primary fat32 1MiB 65MiB
parted ${USB_DRIVE} set ${BIOS_PARTITION_ID} bios_grub on
parted ${USB_DRIVE} mkpart primary 65MiB 129MiB
parted ${USB_DRIVE} ${EFI_PARTITION_ID} boot on
parted ${USB_DRIVE} mkpart primary ext4 129MiB 100%

# Format partitions
# The BIOS boot partition doesn't need a filesystem
# Format UEFI partition to fat32
mkfs.vfat -F32 ${USB_DRIVE}${BIOS_PARTITION_ID}
# Formtat data partition to ext4
mkfs.ext4 ${USB_DRIVE}${DATA_PARTITION_ID}

# Mount sdX to directory /mnt/usb
# mount bios partition
mkdir -p ${MOUNT_POINT}
mount ${USB_DRIVE}${BIOS_PARTITION_ID} ${MOUNT_POINT}
# mount EFI partition
mkdir -p ${MOUNT_POINT}/efi
mount ${USB_DRIVE}${EFI_PARTITION_ID} ${MOUNT_POINT}/efi

# Install grub efi
grub-install --target=x86_64-efi \
    --root-directory=${MOUNT_POINT} \
    --efi-directory=${MOUNT_POINT}/efi \
    --boot-directory=${MOUNT_POINT}/boot \
    --removable --no-floppy --no-nvram \
    --bootloader-id=GRUB \
    --recheck

# Install grub BIOS
```sh
grub-install --target=i386-pc \
    --root-directory=${MOUNT_POINT} \
    --boot-directory=${MOUNT_POINT}/boot \
    --recheck \
    ${USB_DRIVE}
```
# Support boot Windows (wimboot)
cp windows/wimboot ${MOUNT_POINT}/boot

# Create grub config
mkdir -p ${MOUNT_POINT}/boot/grub
cat <<EOF > ${MOUNT_POINT}/boot/grub/grub.cfg
set timeout=5
set default=0
menuentry "Debian 12" {
    insmod part_gpt
    insmod ext2
    set root=(hd0,gpt1)
    linux /vmlinuz root=/dev/sdX1 ro quiet splash
    initrd /initrd.img
}
menuentry "Ubuntu 24.04" {
    insmod part_gpt
    insmod ext2
    set root=(hd0,gpt2)
    linux /vmlinuz root=/dev/sdY1 ro quiet splash
    initrd /initrd.img
}

menuentry "Windows 11" {
    insmod part_gpt
    insmod ntfs
    insmod fat
    insmod loopback

    set isofile=/os/windows/Win11_24H2_English_x64.iso
    loopback loop (hd0,gpt3)$isofile

    linux16 (hd0,gpt3)/boot/wimboot
    initrd16 \
        newc:bcd:(loop)/boot/bcd \
        newc:boot.sdi:(loop)/boot/boot.sdi \
        newc:boot.wim:(loop)/sources/boot.wim
    boot
}
EOF
```

# Note
## Fix bug out of memory when booting in efi mode with tpm is enabled
```sh
rmmod tpm
``` 

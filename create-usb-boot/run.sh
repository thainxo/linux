# Environment
USB_DRIVE=${1}
MOUNT_POINT=${2}

if [ -z "$USB_DRIVE" ] || [ -z "$MOUNT_POINT" ]; then
    echo "Usage: $0 <usb-drive> <mount-point>"
    echo "Example: $0 /dev/sdX /mnt/usb"
    exit 1
fi

BIOS_PARTITION_ID=1
DATA_PARTITION_ID=2
EFI_PARTITION_ID=3
OTHER_PARTITION_ID=4

# Install grub tools
apt install -y grub2 dosfstools mtools
apt install -y grub-efi
apt install -y grub-pc

# Create partitions
parted -s ${USB_DRIVE} mklabel gpt
parted -s ${USB_DRIVE} mkpart primary 1MiB 3MiB
parted -s ${USB_DRIVE} set ${BIOS_PARTITION_ID} bios_grub on

parted -s ${USB_DRIVE} mkpart primary ext4 3MiB 65MiB

parted -s ${USB_DRIVE} mkpart primary fat32 65MiB 129MiB
parted -s ${USB_DRIVE} set ${EFI_PARTITION_ID} boot on

parted -s ${USB_DRIVE} mkpart primary ext4 129MiB 100%

# Format partitions
## The BIOS boot partition doesn't need a filesystem
## Formtat data partition to ext4
mkfs.ext4 -F ${USB_DRIVE}${DATA_PARTITION_ID}

## Format UEFI partition to fat32
mkfs.vfat -F32 ${USB_DRIVE}${EFI_PARTITION_ID}
## Format other partition to ext4
mkfs.ext4 -F ${USB_DRIVE}${OTHER_PARTITION_ID}

# Mount sdX to directory /mnt/usb
## mount data partition
mkdir -p ${MOUNT_POINT}
mount ${USB_DRIVE}${DATA_PARTITION_ID} ${MOUNT_POINT}
## mount EFI partition
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
grub-install --target=i386-pc \
    --root-directory=${MOUNT_POINT} \
    --boot-directory=${MOUNT_POINT}/boot \
    --recheck \
    ${USB_DRIVE}

# Support boot Windows (wimboot)
cp windows/wimboot ${MOUNT_POINT}

# Create grub config
mkdir -p ${MOUNT_POINT}/grub
cat <<EOF > ${MOUNT_POINT}/grub/grub.cfg
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


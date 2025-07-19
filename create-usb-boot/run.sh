# Install grub
```sh
sudo apt install -y grub2 grub-efi grub-pc dosfstools mtools
```
# Mount sdX to directory /mnt/usb
```sh
mkdir -p /media/usb
mount /dev/sdX1 /media/usb
mkdir -p /media/usb/efi
mount /dev/sdX2 /media/usb/efi
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

# Install grub efi
```sh
grub-install --target=i386-pc \
    --root-directory=/media/usb \
    --boot-directory=/media/usb/boot \
    --recheck \
    /dev/sdX
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
EOF
```
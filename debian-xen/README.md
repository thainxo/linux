# Install XEN
```sh
apt install xen-system-amd64
```

# Add /usr/sbin to PATH
```sh
echo "PATH=\${PATH}:/usr/sbin" >> ~/.bashrc
```

# config XEN
## Config network interface
```sh
echo -e "\n\
\n\
auto eno1\n\
iface eno1 inet manual\n\
\n\
auto xenbr0\n\
iface xenbr0 inet static\n\
    address 192.168.0.242\n\
    netmask 255.255.255.0\n\
    gateway 192.168.0.1\n\
    dns-nameservers 192.168.0.1 8.8.4.4 8.8.8.8\n\
    bridge_ports eno1\n\
    bridge_stp off\n\
    bridge_fd 0\n\
" >> /etc/network/interfaces
```
## reload network config
```sh
systemctl restart networking
```
## reboot
```sh
/usr/sbin/reboot
```

# Create XEN image
```sh
qemu-img create -f raw /home/thai/workspace/xen/windows/windows-10.raw 64G
```

# Run Guest OS
[windows-10.cfg]
```sh
xl create /home/thai/workspace/linux/debian-xen/files/windows-10.cfg
```

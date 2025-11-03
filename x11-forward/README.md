# Remote to vscode on Ubuntu
# Install vscode on Ubuntu
## Download vscode
```sh
wget -c https://vscode.download.prss.microsoft.com/dbazure/download/stable/03c265b1adee71ac88f833e065f7bb956b60550a/code_1.105.0-1759933565_amd64.deb
```
## Install vscode
```sh
sudo apt install -y xdg-utils
sudo dpkg -i code_1.105.0-1759933565_amd64.deb 
```
# Install X11 forward
```sh
sudo add-apt-repository universe 
sudo apt-get update
sudo apt-get install x11-xserver-utils xserver-xorg libx11-dev xauth
sudo apt install xorg x11-utils
```

# connect to remote by ssh
```sh
ssh -Y -R6010:localhost:6000 <ssh_server>
export DISPLAY=:10
xclock
```
# Fix issue vscode can not set maximun windows size
edit ~/.config/Code/User/settings.json
add
{ "window.titleBarStyle": "native" } 
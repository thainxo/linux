g++ -c -fPIC -o custom-ssh-dir.o custom-ssh-dir.c
g++ -shared -o libcustomsshdir.so custom-ssh-dir.o

# new SSH configuration directory 
export SSH_CONF_DIR=<path>/.ssh

# the path of custom ssh directory library
export LD_PRELOAD="<path>/libcustomsshdir.so"
ssh <ssh_link>

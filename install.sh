#!/bin/bash

user=$(whoami)

# check if python is installed
if ! [ -x "$(command -v python)" ]; then
    echo 'Error: python is not installed.' >&2
    exit 1
fi

# move giti and pe to /usr/local/bin
sudo cp ./giti /usr/bin

# check current path
if [ -d ~/.giti ]; then
    echo "giti is already installed"
    echo "erasing old installation"
    rm -rf ~/.giti
    cd /home/$user/
    git clone -q https://github.com/3andan4/giti_super.git ./.giti
    echo "giti updated"
else
    cd /home/$user/
    git clone -q https://github.com/3andan4/giti_super.git ./.giti
    echo "giti installed"
fi

rm -rf ~/.giti/giti
rm -rf ~/.giti/install.sh
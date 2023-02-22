#!/bin/bash

set +e

if [ "$EUID" -eq 0 ]
  then echo "Please do not run this script as root"
  exit
fi

if [ -f /usr/bin/giti ]; then
    sudo rm /usr/bin/giti
fi

if [ -f /usr/bin/giti ]; then
    echo "Failed to remove old version of giti"
    exit
fi

if [ -d ~/.giti ]; then
    rm -rf ~/.giti
fi

if [ -d ~/.giti ]; then
    echo "Failed to remove old version of giti"
    exit
fi

git clone git@github.com:MaximeLeBesnerais/giti_super.git ~/.giti

if [ ! -d ~/.giti ]; then
    echo "Failed to clone the repo"
    exit
fi

sudo ln -s ~/.giti/giti /usr/bin/giti

if [ ! -f /usr/bin/giti ]; then
    echo "Failed to create symlink"
    exit
fi

# rename install.sh to update.sh
if [ -f ~/.giti/install.sh ]; then
    mv ~/.giti/install.sh ~/.giti/update.sh
fi

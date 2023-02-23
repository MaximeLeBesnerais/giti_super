#!/bin/bash

set +e


#░██████╗░██╗████████╗██╗
#██╔════╝░██║╚══██╔══╝██║
#██║░░██╗░██║░░░██║░░░██║
#██║░░╚██╗██║░░░██║░░░██║
#╚██████╔╝██║░░░██║░░░██║
#░╚═════╝░╚═╝░░░╚═╝░░░╚═╝

# put the above text in the terminal to see the logo

echo "Installing giti..."

echo "░██████╗░██╗████████╗██╗"
echo "██╔════╝░██║╚══██╔══╝██║"
echo "██║░░██╗░██║░░░██║░░░██║"
echo "██║░░╚██╗██║░░░██║░░░██║"
echo "╚██████╔╝██║░░░██║░░░██║"
echo "░╚═════╝░╚═╝░░░╚═╝░░░╚═╝"


if [ "$EUID" -eq 0 ]
  then echo "Please do not run this script as root"
  exit
fi

if [ -f /usr/bin/giti ]; then
    sudo rm /usr/bin/giti
    echo "Removed old version of giti shortcut..."
fi

if [ -f /usr/bin/giti ]; then
    echo "Failed to remove old version of giti"
    exit
fi

if [ -d ~/.giti ]; then
    rm -rf ~/.giti
    echo "Removed old version of giti folder..."
fi

if [ -d ~/.giti ]; then
    echo "Failed to remove old version of giti"
    exit
fi

git clone git@github.com:MaximeLeBesnerais/giti_super.git ~/.giti
echo "Cloned the repo..."

if [ ! -d ~/.giti ]; then
    echo "Failed to clone the repo"
    exit
fi

sudo ln -s ~/.giti/giti /usr/bin/giti
echo "Created symlink..."

if [ ! -f /usr/bin/giti ]; then
    echo "Failed to create symlink"
    exit
fi

# rename install.sh to update.sh
if [ -f ~/.giti/install.sh ]; then
    mv ~/.giti/install.sh ~/.giti/update.sh
fi

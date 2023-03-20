#!/bin/bash

set +e

beta=false
if [ "$1" = "--beta" ]; then
    beta=true
fi

beta_clone_cmd="git clone --quiet --branch opti git@github.com:MaximeLeBesnerais/giti_super.git ~/.giti"


echo "Installing giti..."
echo -e "\033[0;32m"
echo "░██████╗░██╗████████╗██╗"
echo "██╔════╝░██║╚══██╔══╝██║"
echo "██║░░██╗░██║░░░██║░░░██║"
echo "██║░░╚██╗██║░░░██║░░░██║"
echo "╚██████╔╝██║░░░██║░░░██║"
echo "░╚═════╝░╚═╝░░░╚═╝░░░╚═╝"
echo -e "\033[0m"

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

if [ "$beta" = true ]; then
    echo "Installing beta version..."
    eval "$beta_clone_cmd"
else
    echo "Installing stable version..."
    git clone --quiet git@github.com:MaximeLeBesnerais/giti_super.git ~/.giti
fi
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

echo "Installing dependencies..."
pip3 install -r ~/.giti/requirements.txt

# rename install.sh to update.sh
if [ -f ~/.giti/install.sh ]; then
    mv ~/.giti/install.sh ~/.giti/update.sh
fi

#!/bin/bash

user=$(whoami)

# check if python is installed
if ! [ -x "$(command -v python)" ]; then
    echo 'Error: python is not installed.' >&2
    exit 1
fi

sudo cp ~/.giti/giti /usr/bin/giti

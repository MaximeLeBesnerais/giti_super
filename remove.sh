#!/bin/bash

# script to remove giti

set +e

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


echo "   _|_|_|   "
echo "                                                                                       _|            "
echo " _|          _|_|      _|_|        _|    _|    _|_|    _|    _|        _|_|_|    _|_|_|    _|_|_|      _|_|_|   "
echo "   _|_|    _|_|_|_|  _|_|_|_|      _|    _|  _|    _|  _|    _|      _|    _|  _|    _|  _|    _|  _|  _|    _|  "
echo "       _|  _|        _|            _|    _|  _|    _|  _|    _|      _|    _|  _|    _|  _|    _|  _|  _|    _|  "
echo " _|_|_|      _|_|_|    _|_|_|        _|_|_|    _|_|      _|_|_|        _|_|_|    _|_|_|    _|_|_|  _|  _|    _|  "
echo "                                         _|                                          _|                          "
echo "                                     _|_|                                        _|_|                           "

#!/bin/bash

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Print banner
echo "🚀 Installing Giti C++ Overload (3.0.0)"
echo "======================================="

# Check for required commands
MISSING_DEPS=0
for cmd in git cmake make pkg-config; do
    if ! command_exists $cmd; then
        echo "❌ Required command '$cmd' not found"
        MISSING_DEPS=1
    fi
done

# Check for libgit2
if ! pkg-config --exists libgit2; then
    echo "❌ libgit2 development files not found"
    MISSING_DEPS=1
fi

# If dependencies are missing, try to install them
if [ $MISSING_DEPS -eq 1 ]; then
    echo "📦 Attempting to install missing dependencies..."
    
    if command_exists apt-get; then
        sudo apt-get update
        sudo apt-get install -y git cmake make pkg-config libgit2-dev build-essential
    elif command_exists dnf; then
        sudo dnf install -y git cmake make pkgconfig libgit2-devel gcc-c++
    elif command_exists brew; then
        brew install git cmake make pkg-config libgit2
    else
        echo "❌ Could not automatically install dependencies"
        echo "Please install the required dependencies manually:"
        echo "- git"
        echo "- cmake"
        echo "- make"
        echo "- pkg-config"
        echo "- libgit2 development files"
        echo "- C++ compiler with C++20 support"
        exit 1
    fi
fi

# Create temporary directory
TEMP_DIR=$(mktemp -d)
cd $TEMP_DIR

# Clone repository
echo "📥 Cloning Giti repository..."
git clone https://github.com/MaximeLeBesnerais/giti.git
cd giti

# Build and install
echo "🔨 Building Giti..."
if make install; then
    echo "✅ Giti installed successfully!"
    echo "Run 'giti -h' to get started"
else
    echo "❌ Installation failed"
    echo "Please check the error messages above"
    exit 1
fi

# Cleanup
cd /
rm -rf $TEMP_DIR

echo "🎉 All done! Enjoy using Giti!"
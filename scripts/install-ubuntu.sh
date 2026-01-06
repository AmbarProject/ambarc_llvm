#!/bin/bash
# Ubuntu-specific installer

echo "Installing AmbarC on Ubuntu..."
sudo apt-get update
sudo apt-get install -y wget

# Download latest release
wget https://github.com/AmbarProject/ambarc_llvm/releases/latest/download/ambarc_1.0.0_amd64.deb

# Install
sudo dpkg -i ambarc_1.0.0_amd64.deb
sudo apt-get install -f

rm ambarc_1.0.0_amd64.deb
echo "Installation complete!"
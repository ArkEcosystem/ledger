#!/usr/bin/env bash

# Linker doesn't play nice with Vagrants directory mapping when building for the NanoX.
# credit to @patriksletmo for the tip.

printf "\n%s\n\n" "Building for the Nano X..."

# clean the current project
make clean

# clear the NanoX build destination
sudo rm -rf /home/ubuntu/ledger-app-ark

# copy recursively and take ownership
sudo cp -R ~/apps/ledger-app-ark /home/ubuntu/
sudo chown $USER -R /home/ubuntu/ledger-app-ark

# move to the build directory
cd /home/ubuntu/ledger-app-ark

make

make delete

make load

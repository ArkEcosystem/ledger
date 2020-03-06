#!/usr/bin/env bash

# Linker doesn't play nice with Vagrants directory mapping when building for the NanoX.
# credit to @patriksletmo for the tip.

printf "\n%s\n\n" "Building for the Nano X..."

# clean the current project, in case a build was made in-source
make clean

# name the build dir after the project dir
BUILD_DIR=/home/ubuntu/"${PWD##*/}"

# clear the build dir if this project was already built here
if [[ -d "$BUILD_DIR" ]]; then
    sudo rm -rf "$BUILD_DIR"
fi

# copy the project recursively and take ownership of the build directory
sudo cp -R "$PWD" "$BUILD_DIR"
sudo chown "$USER" -R "$BUILD_DIR"

# move to the build directory
cd "$BUILD_DIR" || exit

make

make delete

make load

#!/usr/bin/env bash

printf "\n%s\n\n" "Building for the Nano S..."

make clean
make
make delete
make load

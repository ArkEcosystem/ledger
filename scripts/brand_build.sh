#!/usr/bin/env bash

######################
# Required Variables #
######################
#
# APPNAME=text (Ark)
# SIGN_PATH=string (e.g., "44'/1'")
# APPVERSION=string ("x.x.x")
#
# TOKEN_NAME=string
# TOKEN_NAME_LEN=int
# TOKEN_DECIMALS=int
# TOKEN_NETWORK_BYTE=int
######################

# Required Flags
while getopts a:s:v:n:l:d:b: flag
do
    case "${flag}" in
        a) APPNAME=${OPTARG};;
        s) SIGN_PATH=${OPTARG};;
        v) APPVERSION=${OPTARG};;
        n) TOKEN_NAME=${OPTARG};;
        l) TOKEN_NAME_LEN=${OPTARG};;
        d) TOKEN_DECIMALS=${OPTARG};;
        b) TOKEN_NETWORK_BYTE=${OPTARG};;
    esac
done

######################
# Example Build Call:
#
# sudo bash scripts/brand_build.sh -a Ark -s "44'/1'" -v "2.3.0" -n "ARK" -l 3 -d 8 -b 30

#
# APPNAME = Ark
# SIGN_PATH="44'/1'"
# APPVERSION="2.3.0"
# 
# TOKEN_NAME="ARK"
# TOKEN_NAME_LEN=3
# TOKEN_DECIMALS=8
# TOKEN_NETWORK_BYTE=0x1e
#

# APPNAME=Ark SIGN_PATH="44'/111'" APPVERSION="2.3.0" TOKEN_NAME="ARK" TOKEN_NAME_LEN=3 TOKEN_DECIMALS=8 TOKEN_NETWORK_BYTE=30

##################
# local variables
WORSPACE_DIR="/app"
WORSPACE_APP_DIR="/app/app"

BUILD_DIR="${WORKDIR}/build"
BUILD_BIN_DIR="${WORKDIR}/build/bin"

############################################
# build docker image if not already present
if [ "$(docker images | grep 'ledger-app-builder')" == "" ]; then
    docker build -t ledger-app-builder:.6.1-2 "${PWD}"
fi

docker run -it --rm -v "${PWD}":/app ledger-app-builder:1.6.1-2 sh -c \
    "printf '\n\n%s\n'  '1) Setting up build directory'     && \
    mkdir -p ${BUILD_DIR}                                   && \
    cp -R ${WORSPACE_DIR}/* ${BUILD_DIR}                    && \
    \
    \
    printf '\n%s\n'     '2) Building app'                   && \
    cd ${BUILD_DIR}                                         && \
    make clean                                              && \
    make                                                    && \
    \
    \
    printf '\n%s\n'     '3) Moving app binaries'            && \
    mkdir -p ${WORSPACE_APP_DIR}                            && \
    cp -rf ${BUILD_BIN_DIR}/* ${WORSPACE_APP_DIR}           && \
    \
    \
    printf '\n%s\n\n' 'All Done!'"

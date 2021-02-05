#!/usr/bin/env bash

# local variables
WORSPACE_DIR="/app"
WORSPACE_APP_DIR="/app/app"

BUILD_DIR="${WORKDIR}/build"
BUILD_BIN_DIR="${WORKDIR}/build/bin"

# build docker image if not already present
if [ "$(docker images | grep 'ledger-app-builder')" == "" ]; then
    docker build -t ledger-app-builder:1.6.1-2 "${PWD}"
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

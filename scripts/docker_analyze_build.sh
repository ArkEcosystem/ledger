#!/usr/bin/env bash

# local variables
WORSPACE_DIR=/app
BUILD_DIR="${WORKDIR}"/build
SCAN_BUILD_OUTPUT_DIR="${WORKDIR}"/app/app/output-scan-build

# build docker image if not already present
if [ "$(docker images | grep 'ledger-app-builder')" == "" ]; then
    docker build -t ledger-app-builder:1.6.1-2 "${PWD}"
fi

docker run -it --rm -v "${PWD}":/app ledger-app-builder:1.6.1-2 sh -c \
    "mkdir -p ${BUILD_DIR}                                  &&  \
    cp -R ${WORSPACE_DIR}/* ${BUILD_DIR}                    &&  \
    cd ${BUILD_DIR}                                         &&  \
    make clean                                              &&  \
    scan-build --use-cc=clang -analyze-headers                  \
    -o ${SCAN_BUILD_OUTPUT_DIR}                                 \
    make"

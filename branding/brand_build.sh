#!/usr/bin/env bash

##################
# local variables
PROJECT_ROOT=$(dirname "$0")
BRAND_DIR="${PROJECT_ROOT}"/branding

WORSPACE_DIR="/app"
WORSPACE_APP_DIR="/app/app"

BUILD_DIR="${WORKDIR}/build"
BUILD_BIN_DIR="${WORKDIR}/build/bin"

######################
# Example Build Call #
######################
# bash branding/brand_build.sh -a MyApp -s "44'/123'" -t "MAPP" -l 4 -d 8
#
# APPNAME         = MyApp
# SIGN_PATH       = "44'/123'"
# 
# TOKEN_NAME      = "MAPP"
# TOKEN_NAME_LEN  = 4
# TOKEN_DECIMALS  = 8
######################

##################
# Required Flags #
##################
while getopts a:s:t:l:d: flag
do
    case "${flag}" in
        a) APPNAME=${OPTARG};;
        s) SIGN_PATH="${OPTARG}";;
        t) TOKEN_NAME="${OPTARG}";;
        l) TOKEN_NAME_LEN=${OPTARG};;
        d) TOKEN_DECIMALS=${OPTARG};;
    esac
done

##################
# Prep Makefiles #
##################
if [[ -d "${BRAND_DIR}/Makefile.brand" ]]; then
  printf '%s\n' "Preparing Makefiles"
  mv "${PROJECT_ROOT}"/Makefile  "${BRAND_DIR}"/Makefile.ark
  mv "${BRAND_DIR}"/Makefile.brand  "${PROJECT_ROOT}"/Makefile
fi

############################################
# build docker image if not already present
if [ "$(docker images | grep 'ledger-app-builder')" == "" ]; then
    docker build -t ledger-app-builder:.6.1-2 "${PWD}"
fi

###########################################
# execute the build command via docker run
docker run -it --rm -v "${PWD}":/app ledger-app-builder:1.6.1-2 \
    sh -c "                                                     \
    export APPNAME=${APPNAME}                               &&  \
    export SIGN_PATH=\"${SIGN_PATH}\"                       &&  \
    export TOKEN_NAME=\"${TOKEN_NAME}\"                     &&  \
    export TOKEN_NAME_LEN=${TOKEN_NAME_LEN}                 &&  \
    export TOKEN_DECIMALS=${TOKEN_DECIMALS}                 &&  \
    printf '\n\n%s\n'  '1) Setting up build directory'      &&  \
    mkdir -p ${BUILD_DIR}                                   &&  \
    cp -R ${WORSPACE_DIR}/* ${BUILD_DIR}                    &&  \
    \
    \
    printf '\n%s\n'     '2) Building app'                   &&  \
    cd ${BUILD_DIR}                                         &&  \
    make clean                                              &&  \
    make                                                    &&  \
    \
    \
    printf '\n%s\n'     '3) Moving app binaries'            &&  \
    mkdir -p ${WORSPACE_APP_DIR}                            &&  \
    cp -rf ${BUILD_BIN_DIR}/* ${WORSPACE_APP_DIR}           &&  \
    \
    \
    printf '\n%s\n\n' 'All Done!'"

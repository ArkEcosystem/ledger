#!/usr/bin/env bash

######################
# Example Build Call #
######################
# bash branding/brand_flash.sh -n MyApp -p "44'/123'"
#
# APPNAME         = MyApp
# SIGN_PATH       = "44'/123'"
######################

##################
# Required Flags #
##################
while getopts n:p: flag
do
    case "${flag}" in
        n) APP_NAME=${OPTARG};;
        p) SIGN_PATH="${OPTARG}";;
    esac
done

################################
# Flash the Ledger NanoS ARK App
# Requirements:
# - python3
# - ledgerblue (pip3 install ledgerblue)
python3 -m ledgerblue.loadApp \
    --fileName "${PWD}"/app/app.hex \
    --appName ${APP_NAME} \
    --appFlags 0x240 \
    --curve secp256k1 \
    --path "44'/1'" \
    --path "${SIGN_PATH}" \
    --targetId 0x31100004 \
    --targetVersion="1.6.1" \
    --tlv \
    --delete

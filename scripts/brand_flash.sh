#!/usr/bin/env bash



##################
# Required Flags #
##################
# APPNAME=text (Ark)
# SIGN_PATH=string (e.g., "44'/1'")
##################
while getopts a:s: flag
do
    case "${flag}" in
        a) APPNAME=${OPTARG};;
        s) SIGN_PATH="${OPTARG}";;
    esac
done

######################
# Example Flash Call #
######################
# bash scripts/brand_flash.sh -a MyApp -s "44'/1'"
#
# APPNAME = MyApp
# SIGN_PATH="44'/1'"
######################

################################
# Flash the Ledger NanoS ARK App
# Requirements:
# - python3
# - ledgerblue (pip3 install ledgerblue)
python3 -m ledgerblue.loadApp \
    --fileName "${PWD}"/app/app.hex \
    --appName "${APPNAME}" \
    --appFlags 0x240 \
    --curve secp256k1 \
    --path "${SIGN_PATH}" \
    --targetId 0x31100004 \
    --targetVersion="1.6.1" \
    --tlv \
    --delete

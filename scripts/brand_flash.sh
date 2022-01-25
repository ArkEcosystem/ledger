#!/usr/bin/env bash

# Flash the Ledger NanoS ARK App
# Requirements:
# - python3
# - ledgerblue (pip3 install ledgerblue)
python3 -m ledgerblue.loadApp \
    --fileName "${APPNAME}"/app/app.hex \
    --appName "${PWD}" \
    --appFlags 0x240 \
    --curve secp256k1 \
    --path "${SIGN_PATH}" \
    --targetId 0x31100004 \
    --targetVersion="1.6.1" \
    --tlv \
    --delete

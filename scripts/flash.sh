#!/usr/bin/env bash

# Flash the Ledger NanoS ARK App
# Requirements:
# - python3
# - ledgerblue (pip3 install ledgerblue)
python3 -m ledgerblue.loadApp \
    --fileName "${PWD}"/app/app.hex \
    --appName Ark \
    --appFlags 0x240 \
    --curve secp256k1 \
    --path "44'/111'" \
    --path "44'/1'" \
    --targetId 0x31100004 \
    --targetVersion="1.6.1" \
    --icon "010000000000ffffffffffffffffffffff7ffe3ffc1ff89ff98ff1cff347e267e6ffffffffffffffff" \
    --tlv \
    --delete

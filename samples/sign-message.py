#!/usr/bin/env python
#*******************************************************************************
#*  (c) 2019 Ark.io
#*
#*  Ledger Blue
#*  (c) 2016 Ledger
#*
#*  Licensed under the Apache License, Version 2.0 (the "License");
#*  you may not use this file except in compliance with the License.
#*  You may obtain a copy of the License at
#*
#*      http://www.apache.org/licenses/LICENSE-2.0
#*
#*  Unless required by applicable law or agreed to in writing, software
#*  distributed under the License is distributed on an "AS IS" BASIS,
#*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#*  See the License for the specific language governing permissions and
#*  limitations under the License.
#********************************************************************************

# Sign a message on Ledger Nano S, using the Ark app on the device.

# Based on https://github.com/LedgerHQ/ledger-sample-apps/blob/68cf07b/blue-app-samplesign/demo.py

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
from secp256k1 import PublicKey

# Copied from https://github.com/ArkEcosystem/ledger/blob/49db08b/src/main.c
cla = bytes("e0".decode("hex"))
ins_get_public_key = bytes("02".decode("hex"))
ins_sign_message = bytes("08".decode("hex"))
p1_non_confirm = 0x00
p1_confirm = 0x01
p1_first = 0x00
p1_more = 0x01
p1_last = 0x80
p2_no_chaincode = 0x00
p2_chaincode = 0x01
p2_secp256k1 = 0x40
p2_ed25519 = 0x80

# Use one of 44'/1' or 44'/111', see
# https://github.com/ArkEcosystem/ledger/blob/a86dd31/Makefile#L24
bip32path = bytes((
    "05" + # bip32 path length 44'/1'/0'/0/0
    "8000002c" + # 44' -> 44 | 0x80000000
    "80000001" + #  1' ->  1 | 0x80000000
    "80000000" + #  0' ->  0 | 0x80000000
    "00000000" + #  0  ->  0
    "00000000"   #  0  ->  0
    ).decode("hex"))

# The maximum length of a chunk to sign is 255 - len(bip32path). This is because
# the length of "bip32path + chunk to sign" must fit in 1 byte (max 255).
maxChunkLen = 255 - len(bip32path)

messageToSign = "aaaaaaa010aaaaaaa020aaaaaaa030aaaaaaa040aaaaaaa050aaaaaaa060aaaaaaa070aaaaaaa080aaaaaaa090aaaaaaa100aaaaaaa110aaaaaaa120aaaaaaa130aaaaaaa140aaaaaaa150aaaaaaa160aaaaaaa170aaaaaaa180aaaaaaa190aaaaaaa200aaaaaaa210aaaaaaa220aaaaaaa230aaaaaaa240aaaaaaa250aaaaaaa260aaaaaaa270aaaaaaa280aaaaaaa290aaaaaaa300"

try:
    dongle = getDongle(True)

    publicKeyResponse = dongle.exchange(
            cla +
            ins_get_public_key +
            chr(p1_non_confirm) +
            chr(p2_no_chaincode | p2_secp256k1) +
            chr(len(bip32path)) +
            bip32path
            )

    publicKeyLen = publicKeyResponse[0]
    publicKeyBytes = publicKeyResponse[1 : 1 + publicKeyLen]

    print "public key: " + str(publicKeyBytes).encode('hex') + " (" + str(publicKeyLen) + " bytes)"

    addressLen = publicKeyResponse[1 + publicKeyLen]
    addressBytes = publicKeyResponse[1 + publicKeyLen + 1 : 1 + publicKeyLen + 1 + addressLen]

    print "address: " + str(addressBytes)

    offset = 0
    while offset <> len(messageToSign):
        if len(messageToSign) - offset > maxChunkLen:
            chunk = messageToSign[offset : offset + maxChunkLen]
        else:
            chunk = messageToSign[offset :]

        if offset + len(chunk) == len(messageToSign):
            p1 = p1_last
        else:
            p1 = p1_first

        offset += len(chunk)

        signature = dongle.exchange(
                cla +
                ins_sign_message +
                chr(p1) +
                chr(p2_no_chaincode | p2_secp256k1) +
                chr(len(bip32path) + len(chunk)) +
                bip32path +
                bytes(chunk)
                )

    print "signature: " + str(signature).encode('hex')
    publicKey = PublicKey(bytes(publicKeyBytes), raw=True)
    signature = publicKey.ecdsa_deserialize(bytes(signature))
    #if publicKey.ecdsa_verify(bytes(chunk), signature):
    if publicKey.ecdsa_verify(bytes(messageToSign), signature):
        print "verified successfully"
    else:
        print "the returned signature failed verification"
except CommException as comm:
    if comm.sw == 0x6985:
        print "Aborted by user"
    else:
        print "Invalid status " + hex(comm.sw)

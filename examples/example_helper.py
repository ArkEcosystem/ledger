#!/usr/bin/env python3

# /*****************************************************************************
#  * This file is part of the ARK Ledger App.
#  *
#  * Copyright (c) ARK Ecosystem <info@ark.io>
#  *
#  * The MIT License (MIT)
#  *
#  * Permission is hereby granted, free of charge, to any person obtaining a
#  * copy of this software and associated documentation files (the "Software"),
#  * to deal in the Software without restriction, including without limitation
#  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  * and/or sell copies of the Software, and to permit persons to whom the
#  * Software is furnished to do so, subject to the following conditions:
#  *
#  * The above copyright notice and this permission notice shall be included in
#  * all copies or substantial portions of the Software.
#  *
#  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
#  * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
#  * OR OTHER DEALINGS IN THE SOFTWARE.
#  ****************************************************************************/

###############################################################################
# Imports
###############################################################################

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException

import math
import argparse
import binascii
import struct
import sys

###############################################################################
# Limits
###############################################################################

chunkSize = 255
chunkMax = 10
payloadMax = chunkMax * chunkSize

###############################################################################
# Globals
###############################################################################

# bip32 Path: default ARK Mainnet
default_path = "44'/111'/0'/0/0"

# Instruction Class
cla = "e0"

# Instructions
op_publickey = "02"
op_sign_tx = "04"
op_sign_message = "08"

# PublicKey APDU P1 & P2
p1_non_confirm = "00"
p2_no_chaincode = "00"

# Signing APDU P1
p1_single = "80"
p1_first = "00"
p1_more = "01"
p1_last = "81"

# Signing Flags P2
p2_ecdsa = "40"

###############################################################################
# Parser
###############################################################################

# Parse Arguments
parser = argparse.ArgumentParser()
parser.add_argument(
    '--message',
    help="Message to sign, hex encoded"
)
parser.add_argument(
    '--path',
    help="BIP 32 path to sign with"
)
parser.add_argument(
    '--publickey',
    help="Get the device publicKey (may be used with '--path'",
    action='store_true'
)
parser.add_argument(
    '--tx',
    help="TX to sign, hex encoded"
)
args = parser.parse_args()

###############################################################################
# Functions
###############################################################################


# Packs the BIP32 Path
def parse_bip32_path(path):
    if len(path) == 0:
        return b""
    result = b""
    elements = path.split('/')
    for pathElement in elements:
        element = pathElement.split('\'')
        if len(element) == 1:
            result = result + struct.pack(">I", int(element[0]))
        else:
            result = result + struct.pack(">I", 0x80000000 | int(element[0]))
    return result


# Get a device PublicKey on a given bip32 path
def get_publickey(pathStr_, path_, pathLength_):
    try:
        apdu = bytearray.fromhex(
            cla + op_publickey + p1_non_confirm + p2_no_chaincode)
        apdu.append(pathLength)
        apdu.append(pathLength // 4)
        apdu += path_
        dongle = getDongle(True)
        result = dongle.exchange(bytes(apdu))
        print(
            "\nPublicKey:",
            binascii.hexlify(result[1:]).decode("utf-8"), "\n"
        )
        print("On Path:", pathStr_, "\n")
    except CommException as comm:
        print("\nError:", hex(comm.sw))
        if comm.sw == 0x6D00 or comm.sw == 0x6F00 or comm.sw == 0x6700:
            print(
                "\nMake sure your Ledger is connected and unlocked",
                "with the ARK app opened.\n")
    sys.exit()


# Splits a payload into chunks
#
# - payload_:       the payload to be split
# - payloadLen_:    the len of the payload (minus the pathLength)
# - chunks_:        the destination for the split payload
# - chunkCount_:    how many chunks the payload should be split into
# - chunkSize_:     max chunk size
# - pathLength_:    the length of the bip32 path
#
# ---
def split_apdu_payload(payload_, payloadLen_,
                       chunks_, chunkCount_,
                       chunkSize_, pathLength_):
    for i in range(chunkCount_):
        pos = 0 if i == 0 else (i * chunkSize_) - pathLength_
        end = ((i + 1) * chunkSize_) - pathLength_
        if i < chunkCount_:
            chunks_[i] = payload_[pos:end]
        else:
            chunks_[i] = payload_[pos:]

###############################################################################
# Application Flow
###############################################################################

# Use default (testnet) path if not provided
if args.path is None:
    args.path = default_path

# Set the BIP32 Path
donglePath = parse_bip32_path(args.path)

# Set the full paths length
pathLength = len(donglePath) + 1

# Get the PublicKey
if args.publickey is True:
    get_publickey(args.path, donglePath, pathLength)

# Set the payload
if args.tx is not None:
    payload = bytearray.fromhex(args.tx)
    operation = op_sign_tx
elif args.message is not None:
    payload = binascii.unhexlify(args.message)
    operation = op_sign_message
else:
    raise CommException('Invalid Instruction')

# Check that the payload is not larger than the current max
if len(payload) > payloadMax:
    raise CommException('Payload size:', len(payload),
                        'exceeds max length:', payloadMax)

# Determine the length of the payload and the number of chunks needed
payloadLen = len(payload)
chunkCount = math.floor(payloadLen / chunkSize) + 1

# Check that the chunkCount doesn't exceed the max
if chunkCount > chunkMax:
    raise CommException('Payload exceeds maximum number of chunks.')

# Chunk buffer
chunks = [None] * (chunkMax + 1)

# Split the payload
split_apdu_payload(payload, payloadLen,
                   chunks, chunkCount,
                   chunkSize, pathLength)

# Set p1
p1 = p1_single if chunkCount == 1 else p1_first

# Signing Algorithm, (Only Ecdsa is supported)
p2 = p2_ecdsa

###############################################################################
# APDU
###############################################################################

try:
    # Send the APDU Payloads in (N)Chunks
    for i in range(chunkCount):
        if chunks[i] is not None:
            hasMoreChunks = chunks[i + 1] is not None
            p1 = p1_single if chunkCount == 1               \
                else p1_first if i == 0 and hasMoreChunks   \
                else p1_more if hasMoreChunks else p1_last
            apdu = bytearray.fromhex(cla + operation + p1 + p2)

            if i == 0:
                apdu.append(pathLength + len(chunks[0]))
                apdu.append(pathLength // 4)
                apdu += donglePath + chunks[0]
                dongle = getDongle(True)
            else:
                apdu.append(len(chunks[i]))
                apdu += chunks[i]

            result = dongle.exchange(bytes(apdu))

            if len(result) == 0:
                print("Sending next APDU Chunk..")
            else:
                print(
                    "\nPayload:",
                    binascii.hexlify(payload).decode("utf-8")
                )
                print("\nApproved by user on Path:", args.path, "\n")
                print(
                    "Signature:",
                    binascii.hexlify(result).decode("utf-8"), "\n"
                )
except CommException as comm:
    print("\nError:", hex(comm.sw))
    if comm.sw == 0x6D00 or comm.sw == 0x6F00 or comm.sw == 0x6700:
        print(
            "\nMake sure your Ledger is connected and unlocked",
            "with the ARK app opened.\n"
        )
    if comm.sw == 0x6985:
        print("\nRejected by user.\n")

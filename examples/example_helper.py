#!/usr/bin/env python3

# /*******************************************************************************
#  * This file is part of the ARK Ledger App.
#  *
#  * Copyright (c) ARK Ecosystem <info@ark.io>
#  *
#  * The MIT License (MIT)
#  *
#  * Permission is hereby granted, free of charge, to any person obtaining a copy
#  * of this software and associated documentation files (the "Software"), to
#  * deal in the Software without restriction, including without limitation the
#  * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#  * sell copies of the Software, and to permit persons to whom the Software is
#  * furnished to do so, subject to the following conditions:
#  *
#  * The above copyright notice and this permission notice shall be included in
#  * all copies or substantial portions of the Software.
#  *
#  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  * SOFTWARE.
#  * 
#  * -----
#  * 
#  * Parts of this software are based on Ledger Nano SDK
#  * 
#  * (c) 2017 Ledger
#  *
#  * Licensed under the Apache License, Version 2.0 (the "License");
#  * you may not use this file except in compliance with the License.
#  * You may obtain a copy of the License at
#  *
#  *    http://www.apache.org/licenses/LICENSE-2.0
#  *
#  * Unless required by applicable law or agreed to in writing, software
#  * distributed under the License is distributed on an "AS IS" BASIS,
#  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  * See the License for the specific language governing permissions and
#  * limitations under the License.
#  ******************************************************************************/

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
import argparse
import binascii
import struct

chunkSize   = 255
payloadMax  = 2 * chunkSize


# Packs the BIP32 Path.
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


# Parse Helper Arguments.
parser = argparse.ArgumentParser()
parser.add_argument('--path', help="BIP 32 path to sign with")
parser.add_argument('--message', help="Message to sign, hex encoded")
parser.add_argument('--tx', help="TX to sign, hex encoded")
args = parser.parse_args()


# Use default path if not provided.
if args.path is None:
    args.path = "44'/1'/0'/0/0"


# Check that one and only one payload operation is called.
if args.tx is None and args.message is None or          \
   args.tx is not None and args.message is not None:
    raise Exception("Missing or Invalid Payload")


# Set the payload
if args.message is not None:
    payload = binascii.unhexlify(args.message)
    operation = "08"
elif args.tx is not None:
    payload = bytearray.fromhex(args.tx)
    operation = "04"


# Check that the payload is not larger than the current max.
if len(payload) > payloadMax:
    raise Exception('Payload size:', len(payload),
                    'exceeds max length:', payloadMax)


# Set the BIP32 Path.
donglePath = parse_bip32_path(args.path)


# Set the full paths length.
pathLength = len(donglePath) + 1


# Pack the payload.
if len(payload) > chunkSize - pathLength:
    chunk1 = payload[0 : chunkSize - pathLength]
    chunk2 = payload[chunkSize - pathLength:]
    p1 = "00"
else:
    chunk1 = payload
    chunk2 = None
    p1 = "80"


# Build the APDU Payload
apdu = bytearray.fromhex("e0" + operation + p1 + "40")
apdu.append(pathLength + len(chunk1))
apdu.append(pathLength // 4)
apdu += donglePath + chunk1

dongle = getDongle(True)
result = dongle.exchange(bytes(apdu))


# Send second data chunk if present
if chunk2 is not None:
    apdu = bytearray.fromhex("e0" + operation + "8140")
    apdu.append(len(chunk2))
    apdu += chunk2
    result = dongle.exchange(bytes(apdu))

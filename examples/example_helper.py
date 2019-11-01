# /*******************************************************************************
# *   Ark Wallet
# *   (c) 2017 Ledger
# *   (c) ARK Ecosystem
# *
# *  Licensed under the Apache License, Version 2.0 (the "License");
# *  you may not use this file except in compliance with the License.
# *  You may obtain a copy of the License at
# *
# *      http://www.apache.org/licenses/LICENSE-2.0
# *
# *  Unless required by applicable law or agreed to in writing, software
# *  distributed under the License is distributed on an "AS IS" BASIS,
# *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# *  See the License for the specific language governing permissions and
# *  limitations under the License.
# ********************************************************************************/

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
import argparse
import struct


# Packs the BIP32 Path.
def parse_bip32_path(path):
    if len(path) == 0:
        return ""

    result = ""
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
    args.path="44'/111'/0'/0/0"


# Check that one and only one payload operation is called.
if args.tx is None and args.message is None or          \
   args.tx is not None and args.message is not None:
    raise Exception("Missing or Invalid Payload")


# Set the payload
if args.message is not None:
    payload = args.message.decode('hex')
    operation = "08"
elif args.tx is not None:
    payload = args.tx.decode('hex') 
    operation = "04"


# Set the BIP32 Path.
donglePath = parse_bip32_path(args.path)


# Set the full paths length.
pathLength = len(donglePath) + 1

# Pack the payload.
if len(payload) > 255 - pathLength:
    chunk1 = args.tx[0 : 255 - pathLength]
    chunk2 = args.tx[255 - pathLength:]
    p1 = "00"
else:
    chunk1 = payload
    chunk2 = None
    p1 = "80"


# Build the APDU Payload
apdu = ("e0" + operation + p1 + "40").decode('hex')                 \
        + chr(pathLength + len(chunk1)) + chr(len(donglePath) / 4)  \
        + donglePath                                                \
        + chunk1

dongle = getDongle(True)
result = dongle.exchange(bytes(apdu))


# Send second data chunk if present
if chunk2 is not None:
    apdu = "e0048140".decode('hex') + chr(len(chunk2)) + chunk2
    result = dongle.exchange(bytes(apdu))

from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
import argparse
import struct

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

parser = argparse.ArgumentParser()
parser.add_argument('--path', help="BIP 32 path to sign with")
parser.add_argument('--tx', help="TX to sign, hex encoded")
args = parser.parse_args()

if args.path == None:
	args.path="44'/111'/0'/0/0"
if args.tx == None:
	raise Exception("Missing TX")

args.tx = args.tx.decode('hex')

donglePath = parse_bip32_path(args.path)

pathLength = len(donglePath) + 1
if len(args.tx) > 255 - pathLength:
    data1 = args.tx[0 : 255 - pathLength]
    data2 = args.tx[255 - pathLength:]
    p1 = "00"
else:
    data1 = args.tx
    data2 = None
    p1 = "80"

apdu = ("e004" + p1 + "40").decode('hex') + chr(pathLength + len(data1)) + chr(len(donglePath) / 4) + donglePath + data1

dongle = getDongle(True)
result = dongle.exchange(bytes(apdu))

if data2 != None:
    apdu = "e0048140".decode('hex') + chr(len(data2)) + data2
    result = dongle.exchange(bytes(apdu))

#result = dongle.exchange(bytes("e0040040a0058000002c80000090800000008000000000000000000b207900039f97d5a32fa1a1a0e12920f5ed21fde918a11ebf14cd42b70b4cc3633b027fcf175fa2a273f5a34d65710e67fc2b4bfe50d0d8140a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e9a435000000008096980000000000".decode('hex')))
# result = dongle.exchange(bytes("e002004096058000002c80000090800000008000000000000000".decode('hex')))


#print "publicKey " + str(result).encode('hex')

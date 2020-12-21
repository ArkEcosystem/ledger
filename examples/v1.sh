#!/usr/bin/env bash

printf "\n\n%s\n" "v1 (DEPRECATED) Examples"

printf "\n%s\n"  "Testing a v1 (DEPRECATED) Transfer Transaction."
python3 "$(dirname "$0")"/example_helper.py --tx ff011e0007627802034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19280969800000000000000c2eb0b00000000000000001e0995750207ecaf0ccf251c1265b92ad84f553662 --ecdsa

printf "\n%s\n"  "Testing a v1 (DEPRECATED) Transfer Transaction with a VendorField."
python3 "$(dirname "$0")"/example_helper.py --tx ff011e0007627802034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19280969800000000000b48656c6c6f20576f726c6400c2eb0b00000000000000001e0995750207ecaf0ccf251c1265b92ad84f553662 --ecdsa

printf "\n%s\n"  "Testing a v1 (DEPRECATED) Vote Transaction."
python3 "$(dirname "$0")"/example_helper.py --tx ff011e0376b87502034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19200e1f50500000000000101022cca9529ec97a772156c152a00aad155ee6708243e65c9d211a589cb5d43234d --ecdsa

#!/usr/bin/env bash

# Transaction Transfer Test
#
# {
#     "type":0,
#     "amount":5523490234,
#     "fee":10000000,
#     "recipientId":"AUexKjGtgsSpVzPLs6jNMM6vJ6znEVTQWK",
#     "timestamp":13893522,
#     "vendorField":"this is a test",
#     "senderPublicKey":"02e012f0a7cac12a74bdc17d844cbc9f637177b470019c32a53cef94c7a56e2ea9"
# }
#
# Serialized: 0092ffd30002e012f0a7cac12a74bdc17d844cbc9f637177b470019c32a53cef94c7a56e2ea9178d4aa08afea30cfc61acd261306c67a87c3e1c2e74686973206973206120746573740000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000bac53949010000008096980000000000
#
# note: The VendorField can safely be ignored
#
# display:
# -   Confirm Transfer
# -   Recipient = AUexKjGtgsSpVzPLs6jNMM6vJ6znEVTQWK
# -   Amount    = 55.23490234
# -   Fee       = 0.1
#
echo ""
echo Testing a Transfer Transaction.
python $(dirname $0)/test_helper.py --tx 0092ffd30002e012f0a7cac12a74bdc17d844cbc9f637177b470019c32a53cef94c7a56e2ea9178d4aa08afea30cfc61acd261306c67a87c3e1c2e74686973206973206120746573740000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000bac53949010000008096980000000000
echo ""

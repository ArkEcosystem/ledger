
# Branding Guide

Welcome to the ARK Ledger App Branding Guide. This document aims to assist ARK-based networks in rebranding, testing, and preparing the ARK Ledger App for production. Should any questions not be answered, feel free to [open an issue](https://github.com/ArkEcosystem/ledger/issues/new/choose) in our official ARK Ledger App repo.

**"What's covered?"**:

* Rebranding the ARK Ledger App
* Building for development purposes
* Flashing a development build to a Nano S, and
* Preparing your forked App for production

**"Why can't we just use the ARK version?"**

LedgerHQ requires that all Ledger Nano X/S Apps be locked to a particular [BIP-32/BIP-44](https://developers.ledger.com/docs/nano-app/secure-app/#restrict-apps-to-coin-specific-bip32-prefix) signing path. Since your network should use a wholly distinct and unique signing path, signing on a path other than those registered by [ARK](https://github.com/satoshilabs/slips/blob/master/slip-0044.md) will be rejected by the ARK Ledger App.

This is an important security measure to mitigate the risks associated with using the same keys across multiple networks.

<div style="width: 100%; float: inherit; margin: auto; text-align: center">

<b>This should NOT be ignored!</b><br>
<b>!! DO NOT REUSE ARK's MAINNET SIGNING PATH (<code>"44'/111'"</code>) !!</b>

</div>

NOTE: If your ARK-based network uses the standard Devnet/Testnet path of `"44'/1'"`, the ARK Ledger App can _still_ be used for testing with your network's Devnet.

## Rebranding

## Building

## Flashing

## Preparing for Production

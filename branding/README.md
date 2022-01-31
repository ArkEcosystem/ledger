
# Branding Guide

Welcome to the ARK Ledger App Branding Guide. This document aims to assist ARK-based networks in rebranding, testing, and preparing the ARK Ledger App for production. Should any questions not be answered, feel free to [open an issue](https://github.com/ArkEcosystem/ledger/issues/new/choose) in our official ARK Ledger App repo.

**"What's covered?"**:

* Building and flashing to a NanoS for development purposes, and
* Preparing your forked App for production

**"Why can't we just use the ARK version?"**

LedgerHQ requires that all Ledger Nano X/S Apps be locked to a particular [BIP-32/BIP-44](https://developers.ledger.com/docs/nano-app/secure-app/#restrict-apps-to-coin-specific-bip32-prefix) signing path. Since your network should use a wholly distinct and unique signing path, signing on a path other than those registered by [ARK](https://github.com/satoshilabs/slips/blob/master/slip-0044.md) will be rejected by the ARK Ledger App.

This is an important security measure to mitigate the risks associated with using the same keys across multiple networks.

<div style="width: 100%; float: inherit; margin: auto; text-align: center">

<b>This should NOT be ignored!</b><br>
<b>!! DO NOT REUSE ARK's MAINNET SIGNING PATH (<code>"44'/111'"</code>) !!</b>

</div>

NOTE: If your ARK-based network uses the standard Devnet/Testnet path of `"44'/1'"`, the ARK Ledger App can _still_ be used for testing with your network's Devnet.

**"What does the brand_build bash script do!"**

In short, this lets you quickly deploy the App for testing using your network's variables. These variables also indicate what needs to be hardcoded for your App before submission to Ledger.

The `brand_build` script will also move the official ARK Makefile to the `branding` directory and the brand-script-compatible Makefile to the project's root directory.

## Development

Not much needs to be changed if you'd like to build and flash your App for testing and development purposes. Make any desired code changes, then run the following commands using your own variables as defined below:

```shell
# from project root:
bash branding/brand_build.sh -a MyApp -s "44'/123'" -t "MAPP" -l 4 -d 8
```

* `-a` - APPNAME - The name of your app displayed on a Ledger Nano's dashboard
* `-s` - SIGN_PATH - Your network’s BIP-32 signing path
* `-t` - TOKEN_NAME - Your network token's in-app name
* `-l` - TOKEN_NAME_LEN - The char count of your token name
* `-d` - TOKEN_DECIMALS - Your tokens decimal count (e.g., 0.00000001 = 8)

```shell
# from project root:
bash branding/brand_flash.sh -a MyApp -s "44'/123'"
```

## Production

If you'd like to prepare your App for production and submit it to LedgerHQ for review, you should first read their [Submission](https://developers.ledger.com/docs/nano-app/deliverables-checklist/) documentation and also make yourself familiar with their [Security Guidelines](https://developers.ledger.com/docs/nano-app/secure-app/).

As for rebranding the ARK Ledger App itself, you'll specifically need to update the following files:

* **Makefile**
    * This needs to be correctly formatted
    * Specifically, all values must be hardcoded; the bash script version is                     **not** acceptable for a production release
    * Refer to the ARK Makefile for proper formatting
* **Icons**
    * These need to be updated to match your network's brand/logo
    * Refer to Ledger's [Design Guidelines](https://developers.ledger.com/docs/nano-app/design-requirements/) for image requirements

---

If any questions were not answered here, or you'd like to know more, feel free to [open an issue](https://github.com/ArkEcosystem/ledger/issues/new/choose) in our official ARK Ledger App repo.
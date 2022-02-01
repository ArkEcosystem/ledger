
# Branding Guide

Welcome. This document aims to assist ARK-based networks in rebranding the ARK Ledger App. Should any questions not be answered, feel free to [open an issue](https://github.com/ArkEcosystem/ledger/issues/new/choose) in our official ARK Ledger App repo.

## Requirements

* Building - [Docker](https://www.docker.com/products/docker-desktop)
* Flashing - [Python3](https://www.python.org/downloads/) & [LedgerBlue](https://github.com/LedgerHQ/blue-loader-python)

## FAQ

### "What's covered?"

* Building and flashing to a NanoS for development purposes, and
* Preparing your forked App for production

### "Why can't we just use the ARK version?"

LedgerHQ requires that all Ledger Nano X/S Apps be locked to a particular [BIP-32/BIP-44](https://developers.ledger.com/docs/nano-app/secure-app/#restrict-apps-to-coin-specific-bip32-prefix) signing path. Since your network should use a wholly distinct and unique signing path, signing on a path other than those registered by [ARK](https://github.com/satoshilabs/slips/blob/master/slip-0044.md) will be rejected by the ARK Ledger App.

This is an important security measure to mitigate the risks associated with using the same keys across multiple networks.

<div style="width: 100%; float: inherit; margin: auto; text-align: center">

<b>This should NOT be ignored!</b><br>
<b>!! DO NOT REUSE ARK's MAINNET SIGNING PATH (<code>"44'/111'"</code>) !!</b>

</div>

NOTE: If your ARK-based network uses the standard Devnet/Testnet path of `"44'/1'"`, the ARK Ledger App can _still_ be used for testing with your network's Devnet.

### "What do the `brand_` bash scripts do?"

In short, these scripts let you quickly build and flash the App to a NanoS for testing using your network's custom variables.

The `brand_build` script will back up the official ARK Makefile to the `branding` directory and print your chosen variables to a new Makefile in your project's root directory.

The `brand_flash` script flashes your customized App to a Ledger NanoS using your your network's App Name and Signing Path parameters. Signing will fail if these variables are not passed as instructed in the flashing script.

## Development

Not much needs to be changed if you'd like to build and flash your App for testing and development purposes. Make any desired code changes, then run the following commands using your own variables as defined below:

```shell
# from project root:
bash branding/brand_build.sh -n MyApp -p "44'/123'" -t "MAPP" -l 4 -d 8
```

* `-n` - APP_NAME - The name of your app displayed on a Ledger Nano's dashboard
* `-p` - SIGN_PATH - Your network’s BIP-32 signing path
* `-t` - TOKEN_NAME - Your network token's in-app name
* `-l` - TOKEN_NAME_LEN - The char count of your token name
* `-d` - TOKEN_DECIMALS - Your tokens decimal count (e.g., 0.00000001 = 8)

```shell
# from project root:
bash branding/brand_flash.sh -n MyApp -p "44'/123'"
```

## Production

If you'd like to prepare your App for production and submit it to LedgerHQ for review, you should first familiarize yourself with their:

* [Submission](https://developers.ledger.com/docs/nano-app/deliverables-checklist/) documentation, and
* [Security Guidelines](https://developers.ledger.com/docs/nano-app/secure-app/).

Otherwise, the only other thing to consider before submitting to LedgerHQ is that you'll need to update/replace icons in the:

* `icons`
* `glyphs`, and
* 'root' directories to match your network's brand/logo.

**\*Refer to Ledger's [Design Guidelines](https://developers.ledger.com/docs/nano-app/design-requirements/) for image requirements**

---

If any questions were not answered here, or you'd like to know more, feel free to [open an issue](https://github.com/ArkEcosystem/ledger/issues/new/choose) in our official ARK Ledger App repo.

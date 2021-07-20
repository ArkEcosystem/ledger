
# ARK-Based Chain Support

Ledger devices make use of [BIP32 / Hierarchical Deterministic (HD) Wallets](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki) and require that all [Slip44](https://github.com/satoshilabs/slips/blob/master/slip-0044.md) paths be explicitly-defined in an App's `Makefile`.

## What Does This Mean?

This means that while using the ARK App, attempting any PublicKey or Signing operations who's BIP32 path makes use of a Slip44 other than `1'` (ARK Devnet) or `111'` (ARK Mainnet) will be rejected.

## Why is This Enforced?

While not ideal for ARK-based chains, Ledger has opted to enforce this rule as a security measure to mitigate cross-chain attacks.

> more info: [Ledger Security Bulletin 014: Path derivation too permissive in Bitcoin derivative apps](https://donjon.ledger.com/lsb/014/#4)

## What Options Do ARK-Based Chains Have?

As a result, ARK-based chains have 2 options for being supported by Ledger:

1. *Open a PR in the ARK App's Repo adding your network's Slip44.
2. Fork the ARK App, customize it using your network's Slip44, and submit it to Ledger for [approval](https://developers.ledger.com/docs/nano-app/introduction/#submit-your-app-for-the-ledger-live-manager).

While an ARK-based chain _could_ technically reuse `111'` for their Slip44, it is not recommended as it's officially registered and reserved for ARK's Mainnet and could result in a loss of user funds.

_*note that while opening a PR is always welcome, there is no guarantee that it will be accepted. All decisions are made on a case-by-case basis and done solely at our discretion._

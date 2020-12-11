# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [2.2.1] - 2020-12-11

### Added
-   merged LedgerHQ downstream changes ([#102])

### Fixed
-   various bounds checking, value handling, and safety improvements ([#cb7f1ef])

## [2.2.0] - 2020-11-06

### Added
-   two-vote transaction support ([#97])

### Changed
-   upgraded `nanos-secure-sdk` version `160` -> `161` ([#94])

## [2.1.0] - 2020-04-06

### Added
-   support large operations ([#72])
-   support schnorr signatures ([#63])

## [2.0.1] - 2020-02-08

### Changed
-   use optimized Base58 implementation from Ledger BTC app ([#66])

## [2.0.0] - 2020-02-05

### Added
-   added message signing features ([#40])
-   implement v2 Transactions ([#27])
-   added VendorField display support ([#29])
-   added build options and documentation ([#32])

### Changed
-   merged updates from LedgerHQ/ledger-app-ark ([#23])
-   cleaned up warnings and refactored implementation ([#25])
-   upgraded `nanos-secure-sdk` version `1553` -> `160` ([#46])
-   refactor transactions classes to decouple Ledger SDK code ([#51])
-   unify Ledger NanoS and NanoX ux display flow ([#53])
-   implement support for large text fields ([#55])

[#23]: https://github.com/ArkEcosystem/ledger/pull/23
[#25]: https://github.com/ArkEcosystem/ledger/pull/25
[#27]: https://github.com/ArkEcosystem/ledger/pull/27
[#29]: https://github.com/ArkEcosystem/ledger/pull/29
[#32]: https://github.com/ArkEcosystem/ledger/pull/32
[#40]: https://github.com/ArkEcosystem/ledger/pull/40
[#46]: https://github.com/ArkEcosystem/ledger/pull/46
[#51]: https://github.com/ArkEcosystem/ledger/pull/51
[#53]: https://github.com/ArkEcosystem/ledger/pull/53
[#55]: https://github.com/ArkEcosystem/ledger/pull/53
[2.0.0]: https://github.com/ArkEcosystem/ledger/compare/master...2.0.0
[#66]: https://github.com/ArkEcosystem/ledger/pull/66
[2.0.1]: https://github.com/ArkEcosystem/ledger/compare/2.0.0...2.0.1
[#63]: https://github.com/ArkEcosystem/ledger/pull/63
[#72]: https://github.com/ArkEcosystem/ledger/pull/72
[2.1.0]: https://github.com/ArkEcosystem/ledger/compare/2.0.1...2.1.0
[#94]: https://github.com/ArkEcosystem/ledger/pull/94
[#97]: https://github.com/ArkEcosystem/ledger/pull/97
[2.2.0]: https://github.com/ArkEcosystem/ledger/compare/2.1.0...2.2.0
[#102]: https://github.com/ArkEcosystem/ledger/pull/102
[#cb7f1ef]: https://github.com/ArkEcosystem/ledger/compare/7ed7885605453bd67af14e62bce17c2ca6f17a26...cb7f1efc5a362fe618731e122afc728da53370ba
[2.2.1]: https://github.com/ArkEcosystem/ledger/compare/2.2.0...2.2.1

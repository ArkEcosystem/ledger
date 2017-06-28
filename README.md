# Ark Ledger
Ark wallet application for Ledger Blue and Nano S

Under development

## Install Development Environment
- Install ledger vagrant https://github.com/fix/ledger-vagrant
- Clone this repo under `apps/`
- Connect to the vagrant machine `vagrant ssh`
- Move to the app `cd apps/ark-ledger`
- Connect your ledger Nano S
- Install the app `sh ./rebuild.sh` (build, delete and load app on ledger)

## TODO
- [x] Make derivation path working
- [x] Generate correct address
- [ ] Parse serialised transactions
  - [ ] Type 1
  - [ ] Type 2
  - [ ] Type 3
  - [ ] Type 4
  - [ ] Type 5
- [x] Sign transaction

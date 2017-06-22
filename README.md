# Ark Ledger
Ark wallet application for Ledger Blue and Nano S

Under development

## Install dev env
- Install ledger vagrant https://github.com/fix/ledger-vagrant
- Clone this repo under `apps/`
- Connect to the vagrant machine `vagrant ssh`
- Move to the app `cd apps/ark-ledger`
- Connect your ledger Nano S
- Install the app `sh rebuild.sh`

## TODO
- [x] Make derivation path working
- [x] Generate correct address
- [ ] Parse Serialised transactions
- [ ] Sign transaction

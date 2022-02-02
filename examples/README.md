
# ARK Ledger Example Tests

## Requirements

* Ledger Nano S Hardware Wallet
* Python
* [Ledger-Vagrant](https://github.com/sleepdefic1t/ledger-vagrant).

## Running all examples

```shell
sh ./examples/all.sh
```

## Running individual examples

```shell
sh ./examples/v1.sh  # DEPRECATED
sh ./examples/v2.sh
sh ./examples/message.sh
```

---

Note: If you're testing these examples with anything other than ARK's signing path (`"44'/111'"`), you'll need to up date the following variable inside the `example_helper.py` file:

* `default_path = "44'/111'/0'/0/0" # (e.g., "44'/123'/0'/0/0")`

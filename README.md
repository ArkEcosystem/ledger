# ARK Ledger

> A simple Ledger NanoS/NanoX app for the ARK Blockchain.

[![Build](https://github.com/ArkEcosystem/ledger/workflows/Build/badge.svg)](https://github.com/ArkEcosystem/ledger/actions)
[![License: MIT](https://badgen.now.sh/badge/license/MIT/green)](https://opensource.org/licenses/MIT)

<!--ts-->
  * [The Ledger ARK app](#the-ledger-ark-app)
  * [Learning Resources](#learning-resources)
    * [The Ledger Docs](#the-ledger-docs)
    * [The NanoS Secure SDK](#the-nanos-secure-sdk)
    * [More Ledger Apps](#the-nanos-secure-sdk)
    * [Further Reading](#further-reading)
  * [The Vagrant Development Environment](#the-ledger-vagrant-environment)
    * [Requirements](#requirements)
    * [Installing Vagrant and VirtualBox](#installing-vagrant-and-virtualbox)
    * [Initializing the Ledger-Vagrant Machine](#initializing-the-ledger-vagrant-machine)
    * [Connecting to the Vagrant machine](#connecting-to-the-vagrant-machine)
    * [Vagrant Troubleshooting](#vagrant-troubleshooting)
  * [Building and Flashing](#building-and-flashing)
    * [Building the Ledger ARK app](#building-the-ledger-ark-app)
    * [Flashing the Ledger ARK app](#flashing-the-ledger-ark-app)
    * [Rebuilding and Flashing Changes](#rebuilding-and-flashing-changes)
  * [Debugging](#debugging)
  * [Security](#security)
  * [Credits](#credits)
  * [License](#license)

<!--te-->

## The Ledger ARK app

This app is written in C for the Ledger NanoS and NanoX devices.

While the Ledger ARK app _is_ compatible with the Ledger NanoX, there is currently no way to load a custom app until LedgerHQ releases the nanox-sdk's.  
The Ledger NanoS—however—is fully supported for development and flashing.


## Learning Resources

This section contains some useful resources for learning about Ledger and embedded development—which is especially nuanced and requires additional considerations beyond typical crypto development.

This is by no means an exhaustive list, but aims to provide some key materials.

---

### The Ledger Docs

The Ledger Docs are by far the best place to start.

They cover anything from how a Ledger works, to publishing a Ledger Application, to secure app development.

***Be sure to read the Ledger Security Guidelines.**  
It covers many concerns that would typically not be obvious, and is something that should be read more than once.

-   [Ledger Documentation Hub](https://ledger.readthedocs.io/en/latest/index.html)
-   [Ledger Documentation Hub: BOLOS](https://ledger.readthedocs.io/en/latest/bolos/features.html)
-   [***Ledger Documentation Hub: Security Guidelines**](https://ledger.readthedocs.io/en/latest/additional/security_guidelines.html)

---

### The NanoS Secure SDK

-   [GitHub: Ledger NanoS Secure SDK](https://github.com/LedgerHQ/nanos-secure-sdk)

---

### More Ledger Apps

-   [Github: LedgerHQ Ledger Apps](https://github.com/LedgerHQ?q=ledger-app)

---

### Further Reading

-   [ARM Developer Docs: The Arm C and C++ Libraries](https://developer.arm.com/docs/dui0475/i/the-arm-c-and-c-libraries)
-   [C Library ABI for the ARM (PDF)](http://infocenter.arm.com/help/topic/com.arm.doc.ihi0039d/IHI0039D_clibabi.pdf)
-   [Stanford Wiki: AVR Programming](https://ccrma.stanford.edu/wiki/AVR_Programming)

## The Vagrant Development Environment

Although the Vagrant environment is optional, it is highly recommended for development.  
i.e. we _can_ use Python & the [ledgerblue CLI](https://github.com/LedgerHQ/blue-loader-python), but using Vagrant keeps the host system safe from unwanted modification and ensures code portability.

The projects `src/`, `example/`, and `glyphs/` folders are configured to sync with the Vagrant shell, so any changes to files in those directories will be updated to the development environment accordingly.  
At this time, the rebuild script and the Makefile do not sync. Modifying those files requires calling `vagrant halt && vagrant up --provision` from outside the shell.

---

### Requirements
-   Ledger Nano S
-   Vagrant
-   VirtualBox

---

### Installing Vagrant and VirtualBox

**Linux**:
```shell
sudo apt install virtualbox
sudo apt install vagrant
```

**macOS**:
```shell
brew cask install virtualbox
brew cask install vagrant
```

**Windows**:

1) Download and run the installer:
  `https://download.virtualbox.org/virtualbox/6.0.10/VirtualBox-6.0.10-132072-Win.exe`
2) Download and install the Oracle VM VirtualBox Extension Pack:
  `https://download.virtualbox.org/virtualbox/6.0.10/Oracle_VM_VirtualBox_Extension_Pack-6.0.10.vbox-extpack`
3) Download and run the Vagrant installer:
  `https://releases.hashicorp.com/vagrant/2.2.5/vagrant_2.2.5_x86_64.msi`

---

### Initializing the Ledger-Vagrant Machine

```shell
vagrant up
```

_note: setup will take a few minutes to provision._

---

### Connecting to the Vagrant machine

```shell
vagrant ssh
```

---

### Vagrant Troubleshooting

**USB Port locked out of the host machine**:

-   Try restarting the Vagrant Box from outside the shell.
    -   `vagrant halt && vagrant up`

**Ledger not found in the Vagrant Box**:
-   On Ubuntu, if the Ledger is not found in the vagrant box, ensure that the **host** user belongs to the vboxusers group.
    -   `sudo usermod -aG vboxusers <username>`  
(_see [askubuntu.com/How to set up USB for VirtualBox](https://askubuntu.com/questions/25596/how-to-set-up-usb-for-virtualbox/25600#25600)_)
-   Try another USB Cable. Prefer the Ledger-supplied cable.  
    Not all USB cables are created equal; and though rare, compromised cables have been found in the wild.

## Building and Flashing

Connect the Ledger device in before flashing.

The Build and Flash commands are executed from project root:

**Vagrant Shell**:
-   `apps/ledger-app-ark`

**ledgerblue CLI**:
-   `ledger-app-ark`

---

### Building the Ledger ARK app

```shell
make
```

---

### Flashing the Ledger ARK app

```shell
make && make load
```

---

### Rebuilding and Flashing Changes

This is the go-to script to push changes to a Ledger device.  
It cleans, builds, deletes and reflashes the app.

```shell
sh ./rebuild.sh
```

## Debugging

Along the way, debugging can save a lot of headaches when developing for embedded environments like Ledger.

Specifically, Debugging on Ledger allows:
-   Use of **PRINTF** and Console printing.
-   Custom Certificates.
-   Session Pin-bypass.

See the following for more information on setting up debugging:
-   [**Ledger Documentation Hub: Debugging**](https://ledger.readthedocs.io/en/latest/userspace/debugging.html)

***Be sure to also enable Debugging in the projects Makefile**:

```make
# DEBUG = 0  # <-- ln87
DEBUG = 1
```

## Security

If you discover a security vulnerability within this package, please send an e-mail to security@ark.io. All security vulnerabilities will be promptly addressed.

## Credits

This project exists thanks to all the people who [contribute](../../contributors).

## License

[MIT](LICENSE) © [ARK Ecosystem](https://ark.io)

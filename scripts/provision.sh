#!/usr/bin/env bash

printf "\n\n%s\n\n" "Provisioning virtual machine..."

printf "\n%s\n" "Installing Utilities"
dpkg --add-architecture i386
apt-get update  > /dev/null
apt-get install git curl python-dev python3-pip python-setuptools zlib1g-dev libjpeg-dev libudev-dev build-essential libusb-1.0-0-dev -y > /dev/null
apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386 libc6-dev-i386 -y > /dev/null
pip3 install --upgrade setuptools
pip3 install ledgerblue
pip3 install Pillow

if [ ! -d "/opt/bolos" ]; then
    printf "\n%s\n" "Setting up BOLOS environment"
    mkdir /opt/bolos
    mkdir /opt/bolos/CC
    mkdir /opt/bolos/SDK

    printf "\n%s\n" "Installing ARM compilers, this will take a few minutes..."
    cd /opt/bolos/CC
    wget -q https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q1-update/+download/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2
    tar xjf gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2
    ln -s /opt/bolos/gcc-arm-none-eabi-5_3-2016q1/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc

    printf "\n%s\n" "Installing Clang compilers, this will take a few minutes..."
    wget -q http://releases.llvm.org/7.0.0/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
    tar xvf clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
    mv clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04 clang-arm-fropi
    chmod 757 -R clang-arm-fropi/
    chmod +x clang-arm-fropi/bin/clang
    ln -s /opt/bolos/CC/clang-arm-fropi/bin/clang /usr/bin/clang
fi

if [ ! -d "/opt/bolos/SDK/nanos-secure-sdk" ]; then
    printf "\n%s\n" "Fetching the Nano S SDK"
    cd /opt/bolos/SDK
    git clone https://github.com/LedgerHQ/nanos-secure-sdk.git
    cd nanos-secure-sdk/
    git -c advice.detachedHead=false checkout tags/nanos-160
fi

# The NanoX SDK is not currently available to the public.
# Contact LedgerHQ for more information about obtaining a copy.
# - https://ledger-dev.slack.com/
# if [ ! -d "/opt/bolos/SDK/nanox-secure-sdk" ]; then
#     printf "\n%s\n" "Fetching the Nano X SDK"
#     cd /opt/bolos/
#     # git clone https://github.com/LedgerHQ/nanox-secure-sdk.git
# fi

printf "\n\n%s\n\n" "Finetuning rights for USB access"
sudo bash /vagrant/scripts/udev.sh
usermod -a -G plugdev vagrant

printf "\n\n%s\n" "Setting up bash profile"
echo "" >> /home/vagrant/.bashrc
echo "# Custom variables for Ledger Development" >> /home/vagrant/.bashrc
echo "export BOLOS_ENV=/opt/bolos/CC" >> /home/vagrant/.bashrc
echo "export BOLOS_SDK=/opt/bolos/SDK/nanos-secure-sdk" >> /home/vagrant/.bashrc
echo "export ARM_HOME=/opt/bolos/CC/gcc-arm-none-eabi-5_3-2016q1" >> /home/vagrant/.bashrc
echo "" >> /home/vagrant/.bashrc
echo "export PATH=\$PATH:\$ARM_HOME/bin" >> /home/vagrant/.bashrc
echo "" >> /home/vagrant/.bashrc

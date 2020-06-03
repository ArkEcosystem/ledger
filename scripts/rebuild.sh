#!/usr/bin/env bash

NANOX='0'
if [ "$1" == "-x" ]; then
  NANOX='1'
fi

if [[ "$NANOX" == '0' ]]; then
  export BOLOS_SDK=/opt/bolos/SDK/nanos-secure-sdk
  exec /usr/bin/env bash "$(dirname "$0")"/rebuild_nanos.sh
else
  export BOLOS_SDK=/opt/bolos/SDK/nanox-secure-sdk
  exec /usr/bin/env bash "$(dirname "$0")"/rebuild_nanox.sh
fi

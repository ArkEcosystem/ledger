#!/usr/bin/env bash

NANOX='0'
if [ "$1" == "-x" ]; then
  NANOX='1'
fi

TEST_DIR="$(dirname "$0")"

printf "\n\n%s\n\n" "Running all ARK Ledger Examples.."
bash ./"${TEST_DIR}"/legacy.sh
bash ./"${TEST_DIR}"/v2.sh "${NANOX}"
bash ./"${TEST_DIR}"/magistrate.sh
bash ./"${TEST_DIR}"/message.sh
printf "\n"

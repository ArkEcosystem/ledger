#!/usr/bin/env bash

NANOX='0'
if [ "$1" == "-x" ]; then
  NANOX='1'
fi

TEST_DIR="$(dirname "$0")"

printf "\n\n%s\n\n" "Running all ARK Ledger Examples.."
bash ./"${TEST_DIR}"/v1.sh  # (DEPRECATED)
bash ./"${TEST_DIR}"/v2.sh
if [[ "$NANOX" == '1' ]]; then
  bash ./"${TEST_DIR}"/multi_signature.sh
fi
bash ./"${TEST_DIR}"/message.sh
printf "\n"

#!/usr/bin/env bash

TEST_DIR="$(dirname "$0")"

printf "\n\n%s\n" "Running all ARK Ledger Examples.."
sh "${TEST_DIR}"/legacy.sh
sh "${TEST_DIR}"/v2.sh
sh "${TEST_DIR}"/message.sh
printf "\n"

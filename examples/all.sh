#!/usr/bin/env bash

TEST_DIR=$(dirname $0)

echo ""
echo Running all ARK Ledger Tests..
sh ${TEST_DIR}/transfer.sh
sh ${TEST_DIR}/vote.sh
sh ${TEST_DIR}/vote2.sh
echo ""

#!/usr/bin/env bash

TEST_DIR=$(dirname $0)

echo ""
echo Running all ARK Ledger Examples..
echo ""
sh ${TEST_DIR}/legacy.sh
sh ${TEST_DIR}/v2.sh
sh ${TEST_DIR}/message.sh
echo ""

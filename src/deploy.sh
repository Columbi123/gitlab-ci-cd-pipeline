#!/bin/bash

set -euo pipefail

PROD_USER="columbi"
PROD_HOST="192.168.152.144"
REMOTE_TMP="/tmp/do6_deploy"

CAT_BIN="$CI_PROJECT_DIR/src/artifacts/s21_cat"
GREP_BIN="$CI_PROJECT_DIR/src/artifacts/s21_grep"

echo "=== Starting deployment ==="

echo "Checking artifacts..."
test -f "$CAT_BIN"
test -f "$GREP_BIN"

echo "Creating temporary directory on production server..."
ssh -o BatchMode=yes \
    "$PROD_USER@$PROD_HOST" \
    "mkdir -p $REMOTE_TMP"

echo "Copying binaries to production server..."
scp -o BatchMode=yes \
    "$CAT_BIN" \
    "$GREP_BIN" \
    "$PROD_USER@$PROD_HOST:$REMOTE_TMP/"

echo "Installing binaries into /usr/local/bin..."
ssh -o BatchMode=yes \
    "$PROD_USER@$PROD_HOST" \
    "sudo -n /usr/bin/install -m 755 $REMOTE_TMP/s21_cat /usr/local/bin/s21_cat &&
     sudo -n /usr/bin/install -m 755 $REMOTE_TMP/s21_grep /usr/local/bin/s21_grep &&
     rm -rf $REMOTE_TMP"

echo "Checking deployed files..."
ssh -o BatchMode=yes \
    "$PROD_USER@$PROD_HOST" \
    "ls -lh /usr/local/bin/s21_cat /usr/local/bin/s21_grep"

echo "=== DEPLOYMENT SUCCESSFUL ==="
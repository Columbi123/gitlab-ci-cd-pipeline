#!/bin/bash

set -u

BASE="$CI_PROJECT_DIR/src/SimpleBashUtils/src"
CAT_BIN="$CI_PROJECT_DIR/src/artifacts/s21_cat"
GREP_BIN="$CI_PROJECT_DIR/src/artifacts/s21_grep"

CAT_TESTS="$BASE/cat/tests"
GREP_TESTS="$BASE/grep/tests"

FAILED=0

echo "=== Integration tests: s21_cat ==="

for file in "$CAT_TESTS"/test*.txt; do
    for flag in "" "-b" "-n" "-s" "-e" "-t"; do
        if diff <("$CAT_BIN" $flag "$file") <(cat $flag "$file") >/dev/null; then
            echo "PASS: s21_cat $flag $(basename "$file")"
        else
            echo "FAIL: s21_cat $flag $(basename "$file")"
            FAILED=1
        fi
    done
done

echo
echo "=== Integration tests: s21_grep ==="

for file in "$GREP_TESTS"/test*.txt; do
    for flag in "" "-e" "-i" "-v" "-c" "-l" "-n" "-h" "-o"; do
        if [ "$flag" = "-e" ]; then
            if diff <("$GREP_BIN" -e hello "$file") <(grep -e hello "$file") >/dev/null; then
                echo "PASS: s21_grep -e hello $(basename "$file")"
            else
                echo "FAIL: s21_grep -e hello $(basename "$file")"
                FAILED=1
            fi
        else
            if diff <("$GREP_BIN" $flag hello "$file") <(grep $flag hello "$file") >/dev/null; then
                echo "PASS: s21_grep $flag hello $(basename "$file")"
            else
                echo "FAIL: s21_grep $flag hello $(basename "$file")"
                FAILED=1
            fi
        fi
    done
done

echo

if [ "$FAILED" -ne 0 ]; then
    echo "=== INTEGRATION TESTS FAILED ==="
    exit 1
fi

echo "=== ALL INTEGRATION TESTS PASSED ==="
exit 0
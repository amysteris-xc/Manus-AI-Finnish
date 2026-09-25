#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-004-arrays-strings-and-bounds.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-004-arrays-strings-and-bounds.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-004-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-004-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-004-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

check_success() {
    input=$1
    expected=$2

    printf '%b' "$input" | "$binary" >"$stdout_file" 2>"$stderr_file"
    printf '%b' "$expected" >"$expected_file"
    diff -u "$expected_file" "$stdout_file"
    test ! -s "$stderr_file"
}

check_failure() {
    input=$1
    expected_stdout=$2
    expected_stderr=$3

    if printf '%b' "$input" | "$binary" >"$stdout_file" 2>"$stderr_file"; then
        printf '%s\n' 'Expected program failure, but it exited successfully.' >&2
        exit 1
    fi

    printf '%b' "$expected_stdout" >"$expected_file"
    diff -u "$expected_file" "$stdout_file"
    printf '%b' "$expected_stderr" >"$expected_file"
    diff -u "$expected_file" "$stderr_file"
}

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"

check_success 'cat\n1\n' 'Label (1-15 characters): Index (0-2): Label: cat\nStored characters: 3\nCharacter at index 1: a\n'
check_success 'manus\n4\n' 'Label (1-15 characters): Index (0-4): Label: manus\nStored characters: 5\nCharacter at index 4: s\n'
check_failure '\n' 'Label (1-15 characters): ' 'Label must not be empty.\n'
check_failure 'cat\n3\n' 'Label (1-15 characters): Index (0-2): ' 'Index must select an existing label character.\n'
check_failure 'cat\n-1\n' 'Label (1-15 characters): Index (0-2): ' 'Index must select an existing label character.\n'
check_failure 'cat\n1x\n' 'Label (1-15 characters): Index (0-2): ' 'Index must select an existing label character.\n'
check_failure 'sixteen-character\n' 'Label (1-15 characters): ' 'Label is too long. Use at most 15 characters.\n'

printf '%s\n' 'c-004-arrays-strings-and-bounds: PASS'

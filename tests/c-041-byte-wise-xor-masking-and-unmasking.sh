#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-041-byte-wise-xor-masking-and-unmasking.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-041-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-041-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-041-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-041-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary"  > "$out_file"
printf '%s' 'Restored: HELLO
' > "$expected_file"
diff -u "$expected_file" "$out_file"

printf '%s\n' 'c-041-byte-wise-xor-masking-and-unmasking: PASS'

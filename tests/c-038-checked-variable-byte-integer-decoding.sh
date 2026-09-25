#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-038-checked-variable-byte-integer-decoding.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-038-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-038-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-038-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-038-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary"  > "$out_file"
printf '%s' 'Decoded: 300
Consumed: 2
' > "$expected_file"
diff -u "$expected_file" "$out_file"

printf '%s\n' 'c-038-checked-variable-byte-integer-decoding: PASS'

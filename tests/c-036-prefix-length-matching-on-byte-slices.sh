#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-036-prefix-length-matching-on-byte-slices.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-036-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-036-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-036-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-036-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary"  > "$out_file"
printf '%s' 'Common prefix length: 2
' > "$expected_file"
diff -u "$expected_file" "$out_file"

printf '%s\n' 'c-036-prefix-length-matching-on-byte-slices: PASS'

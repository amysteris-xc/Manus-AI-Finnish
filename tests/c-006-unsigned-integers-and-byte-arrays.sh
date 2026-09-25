#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-006-unsigned-integers-and-byte-arrays.c"
unit_source="$repo_root/tests/c-006-unsigned-integers-and-byte-arrays-unit.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-006-unsigned-integers-and-byte-arrays.XXXXXX")
unit_binary=$(mktemp "${TMPDIR:-/tmp}/c-006-unsigned-integers-and-byte-arrays-unit.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-006-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-006-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-006-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$unit_binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cc -std=c17 -Wall -Wextra -Wpedantic -o "$unit_binary" "$unit_source"

"$unit_binary"
"$binary" >"$stdout_file" 2>"$stderr_file"
printf '%s' 'Byte count: 6
Bytes: 52 00 41 A7 00 7F
Zero-valued bytes: 2
' >"$expected_file"
diff -u "$expected_file" "$stdout_file"
test ! -s "$stderr_file"

printf '%s\n' 'c-006-unsigned-integers-and-byte-arrays: PASS'

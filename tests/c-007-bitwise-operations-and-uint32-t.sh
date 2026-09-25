#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-007-bitwise-operations-and-uint32-t.c"
unit_source="$repo_root/tests/c-007-bitwise-operations-and-uint32-t-unit.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-007-bitwise-operations-and-uint32-t.XXXXXX")
unit_binary=$(mktemp "${TMPDIR:-/tmp}/c-007-bitwise-operations-and-uint32-t-unit.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-007-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-007-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-007-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$unit_binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cc -std=c17 -Wall -Wextra -Wpedantic -o "$unit_binary" "$unit_source"

"$unit_binary"
"$binary" >"$stdout_file" 2>"$stderr_file"
printf '%s' 'Word count: 2
Word 0: 0x0123052A category=42 flags=5 sequence=291
Word 1: 0xBEEFA010 category=16 flags=160 sequence=48879
' >"$expected_file"
diff -u "$expected_file" "$stdout_file"
test ! -s "$stderr_file"

printf '%s\n' 'c-007-bitwise-operations-and-uint32-t: PASS'

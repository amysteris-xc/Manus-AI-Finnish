#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-045-binary-key-value-frame-serialization.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-045-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-045-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-045-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-045-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary"  > "$out_file"
printf '%s' 'TLV written: 4 bytes
' > "$expected_file"
diff -u "$expected_file" "$out_file"

printf '%s\n' 'c-045-binary-key-value-frame-serialization: PASS'

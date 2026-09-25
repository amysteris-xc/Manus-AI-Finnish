#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-018-bounded-binary-record-writing.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-018-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-018-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-018-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-018-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary" records.bin > "$out_file"
printf '%s' 'Records written: 2
' > "$expected_file"
diff -u "$expected_file" "$out_file"
test "$(wc -c < records.bin)" -eq 16
printf '%s\n' 'c-018-bounded-binary-record-writing: PASS'

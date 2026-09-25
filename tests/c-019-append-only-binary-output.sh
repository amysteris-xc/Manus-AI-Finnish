#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-019-append-only-binary-output.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-019-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-019-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-019-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-019-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary" log.bin > "$out_file"
printf '%s' 'Bytes appended: 5
' > "$expected_file"
diff -u "$expected_file" "$out_file"
"$binary" log.bin >/dev/null
test "$(wc -c < log.bin)" -eq 10
printf '%s\n' 'c-019-append-only-binary-output: PASS'

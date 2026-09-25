#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-065-calloc-count-size-overflow-checks.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-065-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-065-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-065-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-065-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary" > "$out_file"
printf '%s' 'C 065 copied bytes: 0 65 255
' > "$expected_file"
diff -u "$expected_file" "$out_file"
printf '%s\n' 'c-065-calloc-count-size-overflow-checks: PASS'

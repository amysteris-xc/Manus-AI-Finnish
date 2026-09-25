#!/usr/bin/env sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-190-secure-default-configuration.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-190-test.XXXXXX")
work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-190-work.XXXXXX")
out_file=$(mktemp "${TMPDIR:-/tmp}/c-190-out.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-190-expected.XXXXXX")
cleanup() { rm -f "$binary" "$out_file" "$expected_file"; rm -rf "$work_dir"; }
trap cleanup EXIT HUP INT TERM
cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cd "$work_dir"
"$binary" > "$out_file"
printf '%s' 'C 190 validated byte: 190
' > "$expected_file"
diff -u "$expected_file" "$out_file"
printf '%s\n' 'c-190-secure-default-configuration: PASS'

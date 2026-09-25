#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-001-build-types-output.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-001-build-types-output.XXXXXX")

cleanup() {
    rm -f "$binary"
}
trap cleanup EXIT HUP INT TERM

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"

actual_output=$("$binary")
expected_output='Track: C
Lesson: 1
Planned study time: 30 minutes'

if [ "$actual_output" != "$expected_output" ]; then
    printf '%s\n' 'Unexpected output from c-001-build-types-output.' >&2
    printf '%s\n' 'Expected:' >&2
    printf '%s\n' "$expected_output" >&2
    printf '%s\n' 'Actual:' >&2
    printf '%s\n' "$actual_output" >&2
    exit 1
fi

printf '%s\n' 'c-001-build-types-output: PASS'

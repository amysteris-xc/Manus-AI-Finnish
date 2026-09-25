    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-008-byte-order-and-explicit-serialization.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-008-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-008-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-008-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-008-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary"  > "$stdout_file"
    printf '%s' 'Input word: 0x0123052A
Big-endian bytes: 01 23 05 2A
Decoded word: 0x0123052A
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-008-byte-order-and-explicit-serialization: PASS'

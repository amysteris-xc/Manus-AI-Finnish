    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-012-checked-dynamic-byte-buffers.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-012-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-012-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-012-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-012-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary"  > "$stdout_file"
    printf '%s' 'Length: 5
Bytes: 41 00 A7 42 7F
Capacity: 8
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-012-checked-dynamic-byte-buffers: PASS'

    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-013-safe-uint32-rotations-and-test-vectors.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-013-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-013-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-013-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-013-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary"  > "$stdout_file"
    printf '%s' 'rotl32(0x12345678, 8) = 0x34567812
rotl32(0x80000001, 1) = 0x00000003
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-013-safe-uint32-rotations-and-test-vectors: PASS'

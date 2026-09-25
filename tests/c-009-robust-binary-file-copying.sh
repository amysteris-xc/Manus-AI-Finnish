    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-009-robust-binary-file-copying.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-009-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-009-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-009-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-009-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    printf '\x41\x00\x42\xff\x43' > fixture.bin
"$binary" fixture.bin copied.bin > "$stdout_file"
    printf '%s' 'Copied bytes: 5
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"
    cmp fixture.bin copied.bin

    printf '%s\n' 'c-009-robust-binary-file-copying: PASS'

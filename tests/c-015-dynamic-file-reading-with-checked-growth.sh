    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-015-dynamic-file-reading-with-checked-growth.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-015-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-015-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-015-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-015-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    printf '\x41\x00\x42\xff\x43' > fixture.bin
"$binary" fixture.bin > "$stdout_file"
    printf '%s' 'Bytes loaded: 5
First byte: 0x41
Last byte: 0x43
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-015-dynamic-file-reading-with-checked-growth: PASS'

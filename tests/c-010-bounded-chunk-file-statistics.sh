    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-010-bounded-chunk-file-statistics.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-010-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-010-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-010-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-010-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    printf '\x41\x00\xff\x42\x80' > fixture.bin
"$binary" fixture.bin > "$stdout_file"
    printf '%s' 'Bytes: 5
Zero bytes: 1
High bytes: 2
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-010-bounded-chunk-file-statistics: PASS'

    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-016-fixed-header-serialization-round-trips.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-016-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-016-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-016-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-016-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary"  > "$stdout_file"
    printf '%s' 'Header bytes: 00 03 00 05 00 00 01 23
Version: 3 Flags: 5 Length: 291
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-016-fixed-header-serialization-round-trips: PASS'

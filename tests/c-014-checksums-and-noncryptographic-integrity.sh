    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-014-checksums-and-noncryptographic-integrity.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-014-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-014-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-014-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-014-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary"  > "$stdout_file"
    printf '%s' 'Bytes: 5
Toy byte sum: 0x0000000A
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"

    printf '%s\n' 'c-014-checksums-and-noncryptographic-integrity: PASS'

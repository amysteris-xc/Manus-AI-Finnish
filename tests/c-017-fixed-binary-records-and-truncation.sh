    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-017-fixed-binary-records-and-truncation.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-017-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-017-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-017-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-017-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    printf '\x00\x00\x00\x2a\x00\x05\x01\x00\x00\x00\x01\x23\x00\xa0\x02\x01' > fixture.bin
"$binary" fixture.bin > "$stdout_file"
    printf '%s' 'Record 0: id=42 flags=5 kind=1 status=0
Record 1: id=291 flags=160 kind=2 status=1
Records: 2
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"
    printf '' > truncated.bin
if "$binary" truncated.bin >/dev/null 2>&1; then exit 1; fi
    printf '%s\n' 'c-017-fixed-binary-records-and-truncation: PASS'

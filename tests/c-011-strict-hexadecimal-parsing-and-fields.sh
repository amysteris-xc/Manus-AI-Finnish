    #!/usr/bin/env sh
    set -eu
    repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
    source_file="$repo_root/examples/c/c-011-strict-hexadecimal-parsing-and-fields.c"
    binary=$(mktemp "${TMPDIR:-/tmp}/c-011-test.XXXXXX")
    stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-011-stdout.XXXXXX")
    expected_file=$(mktemp "${TMPDIR:-/tmp}/c-011-expected.XXXXXX")
    work_dir=$(mktemp -d "${TMPDIR:-/tmp}/c-011-work.XXXXXX")
    cleanup() { rm -f "$binary" "$stdout_file" "$expected_file"; rm -rf "$work_dir"; }
    trap cleanup EXIT HUP INT TERM
    cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
    cd "$work_dir"
    "$binary" 0xBEEFA010 > "$stdout_file"
    printf '%s' 'Word: 0xBEEFA010
Category: 16
Flags: 160
Sequence: 48879
' > "$expected_file"
    diff -u "$expected_file" "$stdout_file"
    if "$binary" 0x100000000 >/dev/null 2>&1; then exit 1; fi
if "$binary" 0x10junk >/dev/null 2>&1; then exit 1; fi
    printf '%s\n' 'c-011-strict-hexadecimal-parsing-and-fields: PASS'

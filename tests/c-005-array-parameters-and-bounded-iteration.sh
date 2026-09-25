#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-005-array-parameters-and-bounded-iteration.c"
unit_source="$repo_root/tests/c-005-array-parameters-and-bounded-iteration-unit.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-005-array-parameters-and-bounded-iteration.XXXXXX")
unit_binary=$(mktemp "${TMPDIR:-/tmp}/c-005-array-parameters-and-bounded-iteration-unit.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-005-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-005-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-005-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$unit_binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"
cc -std=c17 -Wall -Wextra -Wpedantic -o "$unit_binary" "$unit_source"

"$unit_binary"
"$binary" >"$stdout_file" 2>"$stderr_file"
printf '%s' 'Session count: 5
Session 0: 25 minutes
Session 1: 30 minutes
Session 2: 20 minutes
Session 3: 45 minutes
Session 4: 25 minutes
Total minutes: 145
Average whole minutes: 29
' >"$expected_file"
diff -u "$expected_file" "$stdout_file"
test ! -s "$stderr_file"

printf '%s\n' 'c-005-array-parameters-and-bounded-iteration: PASS'

#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-003-functions-and-error-returns.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-003-functions-and-error-returns.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-003-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-003-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-003-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

check_success() {
    sessions=$1
    minutes=$2
    expected=$3

    "$binary" "$sessions" "$minutes" >"$stdout_file" 2>"$stderr_file"
    printf '%b' "$expected" >"$expected_file"
    diff -u "$expected_file" "$stdout_file"
    test ! -s "$stderr_file"
}

check_failure() {
    expected_stderr=$1
    shift

    if "$binary" "$@" >"$stdout_file" 2>"$stderr_file"; then
        printf '%s\n' 'Expected program failure, but it exited successfully.' >&2
        exit 1
    fi

    test ! -s "$stdout_file"
    printf '%b' "$expected_stderr" >"$expected_file"
    diff -u "$expected_file" "$stderr_file"
}

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"

check_success 4 25 'Sessions: 4\nMinutes per session: 25\nTotal planned minutes: 100\n'
check_success 100 180 'Sessions: 100\nMinutes per session: 180\nTotal planned minutes: 18000\n'
check_failure 'Usage: c-003-functions-and-error-returns SESSIONS MINUTES_PER_SESSION\n'
check_failure 'Invalid session count. Use 1 through 100.\n' 0 25
check_failure 'Invalid session count. Use 1 through 100.\n' 4cats 25
check_failure 'Invalid minutes per session. Use 1 through 180.\n' 4 181
check_failure 'Invalid minutes per session. Use 1 through 180.\n' 4 25minutes

printf '%s\n' 'c-003-functions-and-error-returns: PASS'

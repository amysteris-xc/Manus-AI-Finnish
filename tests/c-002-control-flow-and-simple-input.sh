#!/usr/bin/env sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
source_file="$repo_root/examples/c/c-002-control-flow-and-simple-input.c"
binary=$(mktemp "${TMPDIR:-/tmp}/c-002-control-flow-and-simple-input.XXXXXX")
stdout_file=$(mktemp "${TMPDIR:-/tmp}/c-002-stdout.XXXXXX")
stderr_file=$(mktemp "${TMPDIR:-/tmp}/c-002-stderr.XXXXXX")
expected_file=$(mktemp "${TMPDIR:-/tmp}/c-002-expected.XXXXXX")

cleanup() {
    rm -f "$binary" "$stdout_file" "$stderr_file" "$expected_file"
}
trap cleanup EXIT HUP INT TERM

check_success() {
    input=$1
    expected=$2

    printf '%b' "$input" | "$binary" >"$stdout_file" 2>"$stderr_file"
    printf '%b' "$expected" >"$expected_file"
    diff -u "$expected_file" "$stdout_file"
    test ! -s "$stderr_file"
}

check_failure() {
    input=$1
    expected_stderr=$2

    if printf '%b' "$input" | "$binary" >"$stdout_file" 2>"$stderr_file"; then
        printf '%s\n' 'Expected program failure, but it exited successfully.' >&2
        exit 1
    fi

    printf '%s' 'How many lessons (1-100)? ' >"$expected_file"
    diff -u "$expected_file" "$stdout_file"
    printf '%b' "$expected_stderr" >"$expected_file"
    diff -u "$expected_file" "$stderr_file"
}

cc -std=c17 -Wall -Wextra -Wpedantic -o "$binary" "$source_file"

check_success '1\n' 'How many lessons (1-100)? Plan: 1 lesson\nReview checkpoints: 0\nStart with one focused session.\n'
check_success '12\n' 'How many lessons (1-100)? Plan: 12 lessons\nReview checkpoints: 2\nMake the plan in short, repeatable blocks.\n'
check_failure '12cats\n' 'Enter one whole number from 1 to 100.\n'
check_failure '101\n' 'Enter one whole number from 1 to 100.\n'
check_failure '\n' 'Enter one whole number from 1 to 100.\n'
long_input=$(printf '%064d' 0)
check_failure "$long_input\n" 'Input is too long.\n'

printf '%s\n' 'c-002-control-flow-and-simple-input: PASS'

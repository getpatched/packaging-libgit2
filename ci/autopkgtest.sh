#!/usr/bin/env bash
set -euo pipefail
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
descriptors=(/originals/*.dsc)
test "${#descriptors[@]}" -eq 1
dpkg-source -x "${descriptors[0]}" "$work/source"

# Compile the distro's version-query program against the installed C SDK.
cmake -S "$work/source/debian/tests" -B "$work/build"
cmake --build "$work/build"
"$work/build/libgit2_test"
printf 'libgit2_test PASS\n' > /reports/autopkgtest-summary.txt

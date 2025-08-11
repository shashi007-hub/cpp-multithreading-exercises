#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."
mode="${1:-solution}"
failed=()

for directory in exercises/*/; do
    number="$(basename "$directory" | cut -d- -f1)"
    if make --no-print-directory "${mode}-${number}" > /dev/null 2>&1; then
        echo "ok    ${number}"
    else
        echo "FAIL  ${number}"
        failed+=("$number")
    fi
done

[ "${#failed[@]}" -eq 0 ] || { echo "failed: ${failed[*]}"; exit 1; }

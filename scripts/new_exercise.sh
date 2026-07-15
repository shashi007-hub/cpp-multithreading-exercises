#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "usage: $0 <number> <slug>" >&2
    exit 1
fi

directory="$(dirname "$0")/../exercises/$1-$2"
mkdir -p "$directory"

cat > "$directory/README.md" <<TEMPLATE
# Exercise $1: Title

**Level:** | **Paradigm:** | **Concepts:**

## Problem

## Hints

## Solution walkthrough

## Bonus challenges
TEMPLATE

cat > "$directory/starter.cpp" <<'TEMPLATE'
#include "common/check.hpp"

int main() {
    run_test("todo", [] { CHECK(false); });
}
TEMPLATE

cp "$directory/starter.cpp" "$directory/solution.cpp"
echo "created $directory"

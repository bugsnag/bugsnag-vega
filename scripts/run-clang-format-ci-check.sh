#!/usr/bin/env bash

find packages/kepler-native/kepler -iname *.h -o -iname *.cpp -o -iname *.c | xargs clang-format --dry-run -Werror

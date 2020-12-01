#!/bin/sh

# Abort on errors
set -e

git clone "$REPO" "$BUILD_DIR/"
cd "$BUILD_DIR/"
git submodule update --init
make -e

exec $(eval echo "${@}")
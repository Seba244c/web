#!/bin/bash
# Make sure it is run in the Base dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

if [ -f ./scripts/debug.mode ]; then
    cmake --build --preset build-debug-unix --target web
    out/build/x64-debug-unix/web "$@"
    echo Program returned with $?
elif [[ -f ./scripts/rel.mode ]]; then
    cmake --build --preset build-release-unix --target web
    out/build/x64-release-unix/web "$@"
    echo Program returned with $?
else
    echo "No build mode set, please run scripts/configure-cmake.sh"
fi

#!/bin/bash
# Make sure it is run in the Base dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

# Get Os
os="unnamed"
if [ "$OSTYPE" == "win32" ] || [ "$OSTYPE" == "msys" ]; then
    os="win"
else
    os="unix"
fi

if [ -f ./scripts/debug.mode ]; then
    cmake --build --preset build-debug-${os} --target web
    "out/build/x64-debug-${os}/web" "$@"
    echo Program returned with $?
elif [[ -f ./scripts/rel.mode ]]; then
    cmake --build --preset build-release-${os} --target web
    "out/build/x64-release-${os}/web" "$@"
    echo Program returned with $?
else
    echo "No build mode set, please run scripts/configure-cmake.sh"
fi

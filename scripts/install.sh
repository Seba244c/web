#!/bin/bash
# Make sure it is run in the Base dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

cmake --build --preset build-release-unix --target web
cmake --install out/build/x64-release-unix/

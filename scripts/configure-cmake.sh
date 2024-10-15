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

# Get release mode
mode="unset"
echo "Configure for Debug or Release mode?"
select dr in "Debug" "Release"; do
    case $dr in
        Debug ) mode="debug"; rm -f scripts/rel.mode; touch scripts/debug.mode; break;;
        Release ) mode="release"; rm -f scripts/debug.mode; touch scripts/rel.mode; break;;
    esac
done

# Configure cmake
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset x64-${mode}-${os}

# For clangd
if [ -f ./compile_commands.json ]; then
    rm ./compile_commands.json
fi

ln -s out/build/x64-${mode}-${os}/compile_commands.json .


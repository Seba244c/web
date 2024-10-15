## For Developers
Dependencies UNIX: `git, ninja, cmake, gcc`

Depedencies Windows: `git, ninja, Visual Studio C++ Desktop Development Tools` *Also, you must use the developer commandprompt or powershell application when running build scripts, or else CMake cant find your compiler*

**Setup**
```bash
git clone git@github.com:Seba244c/web.git # or git clone https://github.com/Seba244c/web.git
cd web
git submodule update --init --recursive

# Make sure that cmake is configured and ready to build
# Unix
scripts/configure-cmake.sh # Choose either release og debug mode
# Windows
cmake --preset x64-debug-win # Or x64-release-win
```

**Build & Run**
```bash
# Unix
./scripts/build-run.sh # add arguments here
# Windows
cmake --build --preset build-debug-win --target web # Or use build-release-win for relase mode
./out/build/x64-debug-win/web.exe # add arguments here
```

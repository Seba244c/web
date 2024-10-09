## For Developers
Dependencies `git, ninja`

```bash
git clone git@github.com:Seba244c/web.git
# or git clone https://github.com/Seba244c/web.git

cd web

git submodule update --init --recursive

scripts/configure-cmake.sh # Choose either release og debug mode
scripts/build-run.sh
```

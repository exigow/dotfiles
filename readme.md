dotfiles
========

## Install

```bash
# Create empty repository
cd ~
git init
# Add remote
git remote add origin https://github.com/exigow/dotfiles
# Get the stuff
git fetch
# Be careful! This will overwrite any local files existing on remote!
git reset --hard origin/master
```

## Add more dotfiles

Everything is **ignored** by default except `.gitignore` and `.gitconfig`.

To add file use:
```bash
git add -f path_to_file
```

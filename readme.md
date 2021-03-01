#### Installation

Default clone command will refuse to run because home directory exists. Create empty repository and add remote manually:

```shell
git init
git remote add origin https://github.com/exigow/dotfiles
git fetch
git reset origin/master
```

## About

* Manjaro + i3
* keyboard shortcuts mimics Windows shortcuts
  * this allows for easy switching between OS-es
  * examples: <kbd>Win</kbd>+<kbd>R</kbd> opens dmenu, <kbd>Win</kbd>+<kbd>L</kbd> locks, <kbd>Alt</kbd>+<kbd>Tab</kbd> cycles workspaces and so on
* visuals: no gaps, black & white, high contrast
* sound & wi-fi applets in tray

## Installation

Default clone command will fail because home directory exists. 

To fix that, init empty repo, add remote and reset:

```shell
cd ~
git init
git remote add origin git@github.com:exigow/dotfiles.git
git fetch
git reset origin/master
```

## Applications (optional)

```shell
pacman -Syu fish firefox keepassxc thunar intellij-idea-community-edition pa-applet steam ncspot
```

Set `fish` as default shell:

```shell
chsh -s /usr/bin/fish
```

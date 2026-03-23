# Dotfiles

* compatible with Manjaro and Ubuntu
* i3wm
  * style: no gaps, black & white, high contrast
* Windows-like keyboard shortcuts
  * examples: use Win+R to start `dmenu`, Win+L to `i3lock`, Alt+Tab to cycle workspaces, etc
* system font: Source Code Pro

## Requirements

* apps:
  * `fish`
  * `dmenu`
  * `pa-applet` (on Manjaro) or `pasystray` (on Ubuntu)
  * `nm-applet`
* font: `Source Code Pro`
  * on Ubuntu: https://gist.github.com/jacksonpradolima/840b4a20c617204a65b15da61e124bf6
  * on Manjaro: https://archlinux.org/packages/extra/any/adobe-source-code-pro-fonts/

### Make fish shell as default shell

```shell
chsh -s /usr/bin/fish
```

## Installation

```shell
cd ~
git init
git remote add origin git@github.com:exigow/dotfiles.git
git fetch
git reset origin/master
```

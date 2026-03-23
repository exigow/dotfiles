#!/usr/bin/env bash

launch_first_installed() {
    for app in "$@"; do
        if command -v "$app" &> /dev/null; then
            "$app" &
            return 0
        fi
    done
    notify-send ".i3/autostart.sh" "No apps installed: $*"
}

launch_first_installed "nm-applet" # network manager
launch_first_installed "pa-applet" "pasystray" # volume; pa-applet on Arch, pasystray on Ubuntu

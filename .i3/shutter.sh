#!/usr/bin/env bash

set -e
file="$(mktemp --suffix=.png)"
shutter --select --exit_after_capture --output="$file"
xclip -selection clipboard -t image/png -i "$file"


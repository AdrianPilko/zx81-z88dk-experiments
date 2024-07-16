#!/bin/bash
echo "building roids"
zcc +zx81 -O3  -L/usr/local/share/z88dk/lib  -create-app roids.c -o roids -lm
## for some reason the P is always uppercase?!
mv roids.P roids.p
rm roids


#!/bin/bash


## more info avialable from https://www.z88dk.org/wiki/doku.php?id=platform:zx81
#echo "building hello"
#zcc +zx81 -L/usr/local/share/z88dk/lib  -create-app hello.c -o hello
## for some reason the P is always uppercase?!
#mv hello.P hello.p

echo "building breakout"
#zcc +zx81 -subtype=wrx -lgfx81hr192 -L/usr/local/share/z88dk/lib  -create-app break.c -o break
zcc +zx81 +lamda -L/usr/local/share/z88dk/lib  -create-app break.c -o break
## for some reason the P is always uppercase?!
mv break.P break.p
#echo "building graphics" 
#zcc +zx81 -subtype=wrx -lgfx81hr192 -L/usr/local/share/z88dk/lib  -create-app hrg.c -o hrg
#mv hrg.P hrg.p

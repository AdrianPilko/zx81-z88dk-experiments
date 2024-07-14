#!/bin/bash


## more info avialable from https://www.z88dk.org/wiki/doku.php?id=platform:zx81
#echo "building hello"
#zcc +zx81 -L/usr/local/share/z88dk/lib  -create-app hello.c -o hello
## for some reason the P is always uppercase?!
#mv hello.P hello.p

echo "building scroller"
#zcc +zx81 -subtype=wrx -lgfx81hr192 -L/usr/local/share/z88dk/lib  -create-app break.c -o break
zcc +zx81 -O3  -L/usr/local/share/z88dk/lib  -create-app scroller.c -o scroller
## for some reason the P is always uppercase?!
mv scroller.P scroller.p
#echo "building graphics" 
#zcc +zx81 -subtype=wrx -lgfx81hr192 -L/usr/local/share/z88dk/lib  -create-app hrg.c -o hrg
#mv hrg.P hrg.p

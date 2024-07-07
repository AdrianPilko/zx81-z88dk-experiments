set PATH=PATH;e:\z88dk\;e:\z88dk\bin;e:\z88dk\lib\config\
cd C:\z88dk\ZXSpectrum_SpaceWar\
del main.tap

REM zcc +zx -vn -m -startup=31 -clib=sdcc_iy main.c spaceship1.asm spaceship2.asm bonusship.asm -o main -create-app -lm
echo "building hires graphics" 
zcc +zx81 -subtype=wrx -lgfx81hr192 -create-app hrg.c -o hrg


if exist main.tap (
  call main.tap
  exit
) else (
  pause
)

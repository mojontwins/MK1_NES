@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Exporting chr
cd ..\gfx
..\utils\nescnv tileset-import.png
copy tileset.chr ..\dev
del tileset.chr
echo Copying enems
cd ..\enems
copy enems0.h ..\dev
copy enems1.h ..\dev
echo Making map
cd ..\map
..\utils\mapcnvnes mapa0.map 4 6 16 12 99 mapa0 packed
..\utils\mapcnvnes mapa1.map 4 6 16 12 99 mapa1 packed
copy mapa0.h ..\dev
copy mapa1.h ..\dev
del mapa?.h
cd ..\script
..\utils\mscnes script.spt 24
copy mscnes.h ..\dev
copy scripts.s ..\dev
cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0.s
ca65 game.s
ca65 scripts.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
pause
del *.o
del game.s

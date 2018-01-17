@echo off
echo Exporting chr
cd ..\gfx
..\..\..\src\utils\legacy\nescnv.exe tileset-import2.png
copy tileset.chr ..\dev > nul
echo Making map
cd ..\map
..\..\..\src\utils\legacy\mapcnvnes_old.exe mapa.map 7 5 16 12 99 mapa0 packed
..\..\..\src\utils\legacy\mapcnvnes_old.exe mapa2.map 7 5 16 12 99 mapa1 packed
..\..\..\src\utils\legacy\mapcnvnes_old.exe mapa3.map 7 5 16 12 99 mapa2 packed
copy mapa0.h ..\dev > nul
copy mapa1.h ..\dev > nul
copy mapa2.h ..\dev > nul
rem echo Making script
rem cd ..\script
rem ..\..\..\src\utils\mscnes.exe script.spt 35
rem copy mscnes.h ..\dev > nul
rem copy scripts.s ..\dev > nul
cd ..\dev
echo Making enems
..\..\..\src\utils\legacy\eneexp.exe ..\enems\enems0.ene enems0.h ROM0
..\..\..\src\utils\legacy\eneexp.exe ..\enems\enems1.ene enems1.h ROM1
..\..\..\src\utils\legacy\eneexp.exe ..\enems\enems2.ene enems2.h ROM2
echo Compiling
cc65 -Oi game.c --add-source
ca65 crt0.s
ca65 game.s
ca65 scripts.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
del *.o
del game.s
copy cart.nes ..\jet_paco.nes > nul
echo Done!

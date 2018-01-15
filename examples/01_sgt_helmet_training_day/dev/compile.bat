@echo off
echo Exporting chr
cd ..\gfx
..\..\..\src\utils\nescnv.exe tileset-import.png
copy tileset.chr ..\dev > nul
echo Making map
cd ..\map
..\..\..\src\utils\mapcnvnes_old.exe mapa.map 1 24 16 12 15 mapa0 packed
..\..\..\src\utils\mapcnvnes_old.exe night.map 1 24 16 12 15 mapa1 packed
copy mapa0.h ..\dev > nul
copy mapa1.h ..\dev > nul
echo Making script
cd ..\script
..\..\..\src\utils\mscnes.exe comando-eng.spt 24
copy mscnes.h ..\dev > nul
copy scripts.s ..\dev > nul
cd ..\dev
echo Making enems
..\..\..\src\utils\eneexp.exe ..\enems\enems.ene enems0.h ROM0
..\..\..\src\utils\eneexp.exe ..\enems\night.ene enems1.h ROM1
echo Compiling
cc65 -Oi game.c --add-source
ca65 crt0.s
ca65 game.s
ca65 scripts.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
del *.o > nul
del game.s > nul
copy cart.nes ..\sgt_helmet_training_day.nes > nul
echo Done!

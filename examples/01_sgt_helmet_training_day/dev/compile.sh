#!/bin/bash 
echo Exporting chr
cd ../gfx
../../../src/utils/legacy/nescnv tileset-import.png
cp tileset.chr ../dev 
echo Making map
cd ../map
../../../src/utils/legacy/mapcnvnes_old mapa.map 1 24 16 12 15 mapa0 packed
../../../src/utils/legacy/mapcnvnes_old night.map 1 24 16 12 15 mapa1 packed
cp mapa0.h ../dev 
cp mapa1.h ../dev 
echo Making script
cd ../script
../../../src/utils/legacy/mscnes comando-eng.spt 24
cp mscnes.h ../dev 
cp scripts.s ../dev 
cd ../dev
echo Making enems
../../../src/utils/legacy/eneexp ../enems/enems.ene enems0.h ROM0
../../../src/utils/legacy/eneexp ../enems/night.ene enems1.h ROM1
echo Compiling
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ca65.exe scripts.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
rm*.o 
rm game.s 
cp cart.nes ../../sgt_helmet_training_day_R0.1.nes 
echo Done!

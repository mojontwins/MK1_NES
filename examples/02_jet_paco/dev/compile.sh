#!/bin/sh
echo Exporting chr
cd ../gfx
../../../src/utils/legacy/nescnv tileset-import2.png
cp tileset.chr ../dev 
echo Making map
cd ../map
../../../src/utils/legacy/mapcnvnes_old mapa.map 7 5 16 12 99 mapa0 packed
../../../src/utils/legacy/mapcnvnes_old mapa2.map 7 5 16 12 99 mapa1 packed
../../../src/utils/legacy/mapcnvnes_old mapa3.map 7 5 16 12 99 mapa2 packed
cp mapa0.h ../dev 
cp mapa1.h ../dev 
cp mapa2.h ../dev 
rem echo Making script
rem cd ../script
rem ../../../src/utils/mscnes script.spt 35
rem cp mscnes.h ../dev 
rem cp scripts.s ../dev 
cd ../dev
echo Making enems
../../../src/utils/legacy/eneexp ../enems/enems0.ene enems0.h ROM0
../../../src/utils/legacy/eneexp ../enems/enems1.ene enems1.h ROM1
../../../src/utils/legacy/eneexp ../enems/enems2.ene enems2.h ROM2
echo Compiling
cc65 -Oi game.c --add-source
ca65 crt0.s
ca65 game.s
ca65 scripts.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
rm *.o
rm game.s
cp cart.nes ../../jet_paco.nes 
echo Done!

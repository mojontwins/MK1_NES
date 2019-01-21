#!/bin/bash

echo Generating pals
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts.png out=work/palbg0.h label=mypal_game_bg0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts2.png out=work/palbg1.h label=mypal_game_bg1 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts3.png out=work/palbg2.h label=mypal_game_bg2 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts4.png out=work/palbg3.h label=mypal_game_bg3 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss.png out=work/palfg0.h label=mypal_game_fg0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palssp.png out=work/palfg1.h label=mypal_game_fg1 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/paltsl.png out=work/paltsl.h label=mypal_light_bg silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palssl.png out=work/palssl.h label=mypal_light_fg silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palcuts.png out=work/palcuts.h label=mypal_cuts silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palredbg.png out=work/palred.h label=mypal_reds silent
cat work/palbg0.h work/palbg1.h work/palbg2.h work/palbg3.h work/palfg0.h work/palfg1.h work/paltsl.h work/palssl.h work/palcuts.h work/palred.h > assets/palettes.h

echo Exporting enems
cd ../enems
../../../src/utils/legacy/eneexp2 enems.ene ../dev/assets/enems.h 0
echo Making map
cd ../map
../../../src/utils/legacy/rlemap mapa.map ../dev/assets/mapa.h 20 4 99 0
#rem cd ../script
#rem ../../../src/utils/mscnes script.spt 24
#rem cp mscnes.h ../dev/assets/ 
#rem cp scripts.s ../dev 

echo Exporting chr
cd ../gfx

echo BANKS 0,1
../../../src/utils/mkts mode=scripted in=$(pwd)/cut_tileset0.spt out=../dev/tileset0.chr silent

echo BANKS 2, 3
../../../src/utils/mkts mode=scripted in=$(pwd)/cut_tileset1.spt out=../dev/tileset1.chr silent

echo BANKS 4,5
../../../src/utils/mkts mode=scripted in=$(pwd)/cut_tileset2.spt out=../dev/tileset2.chr silent

echo dummy banks 6, 7
../../../src/utils/fillto empty.bin 8192
cp empty.bin ../dev/tileset3.chr 
#rm empty.bin 

cd ../dev
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0-CNROM.s -o crt0.o
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
#rem ca65 scripts.s
#rem ld65 -v -C ../../nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib

#rm *.o
#rm game.s

cp cart.nes ../../cheril_the_goddess.nes 

#rm work/*.h 

echo DONE

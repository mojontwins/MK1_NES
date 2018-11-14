#!/bin/bash 

#if [%1]==[justcompile] goto :justcompile

echo Generating pals
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts0.png out=work/palts0.h label=palts0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss0.png out=work/palss0.h label=palss0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts1.png out=work/palts1.h label=palts1 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts2.png out=work/palts2.h label=palts2 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/paltsl.png out=work/paltsl.h label=paltsl silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palssl.png out=work/palssl.h label=palssl silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palcuts.png out=work/palcuts.h label=palcuts silent
cat work/palts0.h work/palss0.h work/palts1.h work/palts2.h work/paltsl.h work/palssl.h work/palcuts.h > assets/palettes.h

echo Exporting enems
cd ../enems
../../../src/utils/eneexp3 level0.ene ../dev/assets/enems0.h 0 1 gencounter
../../../src/utils/eneexp3 level1.ene ../dev/assets/enems1.h 1 1 gencounter
../../../src/utils/eneexp3 level2.ene ../dev/assets/enems2.h 2 1 gencounter

echo Compiling enembehs
cd ../script
../../../src/utils/pencompiler enembehs.spt ../dev/assets/compiled_enems.h

echo Making map
cd ../map
../../../src/utils/rle53mapMK1 ../map/level0.map ../dev/assets/map0.h 6 5 15 0 1 scrsizes nodecos
../../../src/utils/rle53mapMK1 ../map/level1.map ../dev/assets/map1.h 4 6 15 1 1 scrsizes nodecos
../../../src/utils/rle53mapMK1 ../map/level2.map ../dev/assets/map2.h 4 6 15 2 1 scrsizes nodecos

echo Exporting music and sound
cd ../dev
wine ../../../src/utils/text2data.exe ../ogt/music.txt -ca65 -ch1..4
wine ../../../src/utils/nsf2data.exe ../ogt/sounds.nsf -ca65 -ntsc
cp  ../ogt/music.s .
cp  ../ogt/sounds.s .

cd ../dev

#:justcompile
#if [%2]==[noscript] goto :noscript

echo Building script
cd ../script
../../../src/utils/mscmk1 script.spt ../dev/assets/mscnes.h 25
cd ../dev

echo Exporting chr
cd ../gfx
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns.spt out=../dev/tileset.chr silent
cd ../dev

#:noscript
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o -D CNROM=0
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart-omv.nes crt0.o game.o runtime.lib -m labels.txt

echo Exporting chr
cd ../gfx
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns-sfw.spt out=../dev/tileset.chr silent
cd ../dev

#:noscript
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o -D CNROM=0
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt


rm *.o
rm game.s

cp  cart.nes ../../bootee.nes
rm work/*.h
rm work/*.bin.*

echo DONE!

#!/bin/bash

#if [%1]==[justcompile] goto :justcompile

echo Generating pals
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts0.png out=work/palts0.h label=palts0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss0.png out=work/palss0.h label=palss0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/paltscuts.png out=work/paltscuts.h label=paltscuts silent
cat work/palts0.h work/palss0.h work/paltscuts.h > assets/palettes.h 

echo Exporting chr
cd ../gfx
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns.spt out=../dev/tileset.chr silent

echo Exporting enems
cd ../enems
../../../src/utils/eneexp3 level0.ene ../dev/assets/enems0.h 0 1 gencounter

echo Compiling enembehs
cd ../script
../../../src/utils/pencompiler enembehs.spt ../dev/assets/compiled_enems.h

echo Making map
cd ../map
../../../src/utils/rle53mapMK1 ../map/level0.map ../dev/assets/map0.h 5 4 15 0 1 scrsizes

echo Exporting music and sound
cd ../dev
wine ../../../src/utils/text2data.exe ../ogt/music.txt -ca65 -ch1..4
wine ../../../src/utils/nsf2data.exe  ../ogt/sounds.nsf -ca65 -ntsc
cp  ../ogt/music.s .
cp  ../ogt/sounds.sv.

cd ../dev

#:justcompile
#if [%2]==[noscript] goto :noscript

echo Building script
cd ../script
../../../src/utils/mscmk1 script.spt ../dev/assets/mscnes.h 20 debug
cd ../dev

#:noscript
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o -D CNROM=0
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

rm *.o
rm game.s

cp  cart.nes ../../cadaverion.nes > nul
rm work/*.h 
rm work/*.bin.* 

echo DONE!

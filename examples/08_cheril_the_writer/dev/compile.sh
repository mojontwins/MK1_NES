#!/bin/bash 

#if [%1]==[justcompile] goto :justcompile

echo Generating pals
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts0.png out=work/palts0.h label=palts0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss0.png out=work/palss0.h label=palss0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts1.png out=work/palts1.h label=palts1 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss1.png out=work/palss1.h label=palss1 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts2.png out=work/palts2.h label=palts2 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss2.png out=work/palss2.h label=palss2 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palts3.png out=work/palts3.h label=palts3 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palss3.png out=work/palss3.h label=palss3 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palssgrey.png out=work/palssgrey.h label=palssgrey silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/palcuts0.png out=work/palcuts0.h label=palcuts0 silent
../../../src/utils/mkts mode=pals pals=$(pwd)/../gfx/paltitle.png out=work/paltitle.h label=paltitle silent

cat work/palts0.h work/palss0.h work/palts1.h work/palss1.h work/palts2.h work/palss2.h work/palts3.h work/palss3.h work/palssgrey.h work/palcuts0.h work/paltitle.h > assets/palettes.h 

echo Exporting chr
cd ../gfx
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns0.spt out=../dev/tileset0.chr silent
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns1.spt out=../dev/tileset1.chr silent
../../../src/utils/mkts mode=scripted in=$(pwd)/import_patterns2.spt out=../dev/tileset2.bin silent

echo Exporting enems
cd ../enems
../../../src/utils/eneexp3 level0.ene ../dev/work/enems0.h 0 1 gencounter bin
../../../src/utils/eneexp3 level1.ene ../dev/work/enems1.h 1 1 gencounter bin
../../../src/utils/eneexp3 level2.ene ../dev/work/enems2.h 2 1 gencounter bin
../../../src/utils/eneexp3 level3.ene ../dev/work/enems3.h 3 1 gencounter bin
../../../src/utils/eneexp3 level4.ene ../dev/work/enems4.h 4 1 gencounter bin
cd ../dev/work
cat enems0.h enems1.h enems2.h enems3.h enems4.h > ../assets/enem_constants.h 
../../../../src/utils/binpaster index=../assets/enem_index.h out=../enems.bin files=enems0.h.bin,enems1.h.bin,enems2.h.bin,enems3.h.bin,enems4.h.bin
cd ..
cat tileset2.bin enems.bin > tileset2.chr 
rm tileset2.bin 
rm enems.bin 

echo Compiling enembehs
cd ../script
../../../src/utils/pencompiler enembehs.spt ../dev/assets/compiled_enems.h

echo Making map
cd ../map
../../../src/utils/rle53mapchrrom in=$(pwd)/maplist.txt bin=$(pwd)/../dev/work/mapchr.bin out=../dev/assets/chr_rom_maps.h chr=3
cd ../dev
cp  work/mapchr.bin.3 tileset3.chr 
../../../src/utils/mapcnvnes2 ../map/level0_bg.map ../dev/assets/bg0.h 1 1 99 bg0
../../../src/utils/mapcnvnes2 ../map/level3_bg.map ../dev/assets/bg3.h 2 1 99 bg3

echo Exporting music and sound
cd ../dev
wine ../../../src/utils/text2data.exe ../ogt/music.txt -ca65 -ch1..4
wine ../../../src/utils/nsf2data.exe ../ogt/sounds.nsf -ca65 -ntsc
cp  ../ogt/music.s . 
cp  ../ogt/sounds.s .

cd ../dev

#:justcompile
#if [%2]==[noscript] goto :noscript

#rem echo Building script
#rem cd ../script
#rem ../../../src/utils/mscmk1 script.spt ../dev/assets/mscnes.h 5
#rem cd ../dev

#:noscript
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o -D CNROM=1

wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source -D CNROM
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source -D CNROM -D SPANISH
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes-CNROM.cfg -o cart-sp.nes crt0.o game.o runtime.lib -m labels.txt


rm *.o 
rm game.s 
rm work/*.h 
rm work/*.bin.*

cp  cart.nes ../../cheril_the_writer.nes

echo DONE!

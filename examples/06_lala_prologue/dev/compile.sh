#!/bin/bash


echo Generating pals
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palts1.png out=work/palbg0.h label=mypal_game_bg0 silent

../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palss1.png out=work/palfg0.h label=mypal_game_fg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/paltsl.png out=work/paltsl.h label=mypal_lighting_bg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palssl.png out=work/palssl.h label=mypal_lighting_fg0 silent
#rem ../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palcuts.png out=work/palcuts.h label=mypal_cuts silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palredbg.png out=work/palred.h label=mypal_reds silent
cat work/palbg0.h work/palfg0.h work/paltsl.h work/palssl.h work/palred.h > assets/palettes.h 

echo Exporting chr
cd ../gfx

#rem charset, tileset, fill to 4096
../../../src/utils/legacy/mkts_old in=$(pwd)/font.png pals=$(pwd)/palts1.png out=tschr1.bin mode=chars size=32,2 silent
../../../src/utils/legacy/mkts_old in=$(pwd)/ts.png pals=$(pwd)/palts1.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=../dev/assets/tiledata1.h label=ts1 tmapoffset=64 silent
#rem ../../../src/utils/legacy/mkts_old in=cuts.png pals=$(pwd)/palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=../dev/assets/cuts1.h label=cuts1 tmapoffset=185 max=32 silent

cat tschr1.bin tschr3.bin > tschr.bin 
../../../src/utils/fillto tschr.bin 4096

rm tschr1.bin 
rm tschr3.bin 
#rem rm tschr4.bin 

../../../src/utils/legacy/mkts_old in=$(pwd)/ss.png pals=$(pwd)/palss1.png out=sschr1.bin mode=sprites tsmap=../dev/assets/spr_pl.h offset=0,0 size=3,1 metasize=2,3 sprorg=0,-8 label=spr_pl genflipped silent
../../../src/utils/legacy/mkts_old in=$(pwd)/ss.png pals=$(pwd)/palss1.png out=sschr2.bin mode=sprites tsmap=../dev/assets/spr_en.h offset=0,3 size=8,1 metasize=2,2 sprorg=0,0 label=spr_en genflipped tmapoffset=18 genflipped silent 
../../../src/utils/legacy/mkts_old in=$(pwd)/ss.png pals=$(pwd)/palss1.png out=sschr3.bin mode=sprites tsmap=../dev/assets/spr_hs.h offset=0,5 size=3,2 metasize=2,2 sprorg=0,0 label=spr_hs tmapoffset=44 silent

cat sschr1.bin sschr2.bin sschr3.bin > sschra.bin 
rm sschr1.bin 
rm sschr2.bin 
rm sschr3.bin 
../../../src/utils/fillto sschra.bin 4096

cp sschra.bin sschr.bin 
rm sschra.bin 

cat tschr.bin sschr.bin > ../dev/tileset.chr 

rm tschr.bin 
rm sschr.bin 

echo Exporting enems
cd ../enems
../../../src/utils/legacy/eneexp2 enems0.ene ../dev/assets/enems0.h 0
echo Making map
cd ../map
../../../src/utils/legacy/rlemap level0.map ../dev/assets/mapa0.h 6 5 15 0


cd ../dev

#rem echo Building script
#rem cd ../script
#rem ../../../src/utils/legacy/msc3nes script.spt 30
#rem cp  mscnes.h ../dev/assets/ 
#rem cd ../dev
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
#rem ca65 scripts.s
#rem ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

rm *.o
rm game.s

cp cart.nes ../../lala_prologue.nes
rm work/*.h  

echo DONE!

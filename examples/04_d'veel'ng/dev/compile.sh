#!/bin/bash

echo Generating pals
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/paltsb.png out=work/palbg0.h label=mypal_game_bg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palssb.png out=work/palfg0.h label=mypal_game_fg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/paltsl.png out=work/palf0.h label=mypal_lighting_bg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palssl.png out=work/palf1.h label=mypal_lighting_fg0 silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palcuts.png out=work/palcuts.h label=mypal_cuts silent
../../../src/utils/legacy/mkts_old mode=pals pals=$(pwd)/../gfx/palredbg.png out=work/palredbg.h label=mypal_redbg silent
cat work/palbg0.h work/palfg0.h work/palf0.h work/palf1.h work/palcuts.h work/palredbg.h > assets/palettes.h 

echo cp ing enems
cd ../enems
../../../src/utils/legacy/eneexp2 enemsd.ene ../dev/assets/enems.h 0 
echo Making map
cd ../map
../../../src/utils/legacy/mapcnvnes dveel.map 5 5 16 12 15 mapa0 packed
cp  mapa0.h ../dev/assets/ 
rm mapa?.h
#rem cd ../script
#rem ../../../src/utils/mscnes script.spt 24
#rem cp  mscnes.h ../dev/assets/ 
#rem cp  scripts.s ../dev 

echo Exporting chr
cd ../gfx

../../../src/utils/legacy/mkts_old in=font.png pals=$(pwd)/paltsb.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
../../../src/utils/legacy/mkts_old in=tsb.png pals=$(pwd)/paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=../dev/assets/tiledata1.h label=ts1 tmapoffset=64 max=24 silent
../../../src/utils/legacy/mkts_old in=cuts.png pals=$(pwd)/palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=../dev/assets/cuts1.h label=cuts1 tmapoffset=144 max=32 silent
../../../src/utils/legacy/mkts_old in=tstitle.png pals=$(pwd)/palcuts.png out=tschr5.bin mode=mapped offset=0,0 size=9,1 tsmap=../dev/assets/tiledata_title.h label=title tmapoffset=216 silent

cat tschr1.bin tschr3.bin tschr4.bin tschr5.bin > tschr.bin 
../../../src/utils/fillto tschr.bin 4096

rm tschr1.bin 
rm tschr3.bin 
rm tschr4.bin 
rm tschr5.bin 

#rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

../../../src/utils/mkts in=ssb-cheril.png pals=$(pwd)/palssb.png out=sschr1.bin mode=sprites tsmap=../dev/assets/spr_pl.h offset=0,0 size=6,5 metasize=2,3 sprorg=0,-8 label=spr_pl max=26 silent
../../../src/utils/mkts in=ssb-enems.png pals=$(pwd)/palssb.png out=sschr2.bin mode=sprites tsmap=../dev/assets/spr_en.h offset=0,0 size=11,2 metasize=2,2 sprorg=0,0 label=spr_en max=15 tmapoffset=102 silent
../../../src/utils/mkts in=hostage.png pals=$(pwd)/palssb.png out=sschr3.bin mode=sprites tsmap=../dev/assets/spr_hs1.h offset=0,0 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_hs1 max=4 tmapoffset=144 silent
../../../src/utils/mkts in=items.png pals=$(pwd)/palssb.png out=sschr4.bin mode=sprites tsmap=../dev/assets/spr_hs2.h offset=0,0 size=3,1 metasize=2,2 sprorg=0,0 label=spr_hs2 max=3 tmapoffset=156 silent
../../../src/utils/mkts in=logo.png pals=$(pwd)/palcuts.png out=sschr5.bin mode=sprites tsmap=../dev/assets/spr_logo.h offset=0,0 size=1,1 metasize=6,2 sprorg=0,0 label=spr_logo tmapoffset=168 silent

cat sschr1.bin sschr2.bin sschr3.bin sschr4.bin sschr5.bin > sschra.bin 
rm sschr1.bin 
rm sschr2.bin 
rm sschr3.bin 
rm sschr4.bin 
rm sschr5.bin 

../../../src/utils/fillto sschra.bin 3968

../../../src/utils/legacy/mkts_old in=proj.png pals=$(pwd)/palssb.png out=sschr5.bin mode=chars offset=0,0 size=8,1 silent

cat sschra.bin sschr5.bin > sschr.bin 

rm sschra.bin 
rm sschr5.bin 
../../../src/utils/fillto sschr.bin 4096

cat tschr.bin sschr.bin > ../dev/tileset.chr 

rm tschr.bin 
rm sschr.bin 

cd ../dev
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
#rem ca65 scripts.s
#rem ld65 -v -C ../../nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

rm *.o
rm game.s

cp  cart.nes ../../d'veel'ng.nes 

if [%1]==[omv] goto :omv
goto :end
:omv

echo Exporting chr
cd ../gfx

../../../src/utils/legacy/mkts_old in=font.png pals=$(pwd)/paltsb.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
../../../src/utils/legacy/mkts_old in=tsb.png pals=$(pwd)/paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=../dev/assets/tiledata1.h label=ts1 tmapoffset=64 max=24 silent
../../../src/utils/legacy/mkts_old in=cuts.omv pals=$(pwd)/palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=../dev/assets/cuts1.h label=cuts1 tmapoffset=144 max=32 silent
../../../src/utils/legacy/mkts_old in=tstitle.png pals=$(pwd)/palcuts.png out=tschr5.bin mode=mapped offset=0,0 size=9,1 tsmap=../dev/assets/tiledata_title.h label=title tmapoffset=216 silent

cat tschr1.bin tschr3.bin tschr4.bin tschr5.bin > tschr.bin 
../../../src/utils/fillto tschr.bin 4096

rm tschr1.bin 
rm tschr3.bin 
rm tschr4.bin 
rm tschr5.bin 

#rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

../../../src/utils/mkts in=ssb-cheril.omv pals=$(pwd)/palssb.png out=sschr1.bin mode=sprites tsmap=../dev/assets/spr_pl.h offset=0,0 size=6,5 metasize=2,3 sprorg=0,-8 label=spr_pl max=26 silent
../../../src/utils/mkts in=ssb-enems.png pals=$(pwd)/palssb.png out=sschr2.bin mode=sprites tsmap=../dev/assets/spr_en.h offset=0,0 size=11,2 metasize=2,2 sprorg=0,0 label=spr_en max=15 tmapoffset=101 silent
../../../src/utils/mkts in=hostage.omv pals=$(pwd)/palssb.png out=sschr3.bin mode=sprites tsmap=../dev/assets/spr_hs1.h offset=0,0 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_hs1 max=4 tmapoffset=143 silent
../../../src/utils/mkts in=items.png pals=$(pwd)/palssb.png out=sschr4.bin mode=sprites tsmap=../dev/assets/spr_hs2.h offset=0,0 size=3,1 metasize=2,2 sprorg=0,0 label=spr_hs2 max=3 tmapoffset=162 silent
../../../src/utils/mkts in=logo.png pals=$(pwd)/palcuts.png out=sschr5.bin mode=sprites tsmap=../dev/assets/spr_logo.h offset=0,0 size=1,1 metasize=6,2 sprorg=0,0 label=spr_logo tmapoffset=168 silent

cat sschr1.bin sschr2.bin sschr3.bin sschr4.bin sschr5.bin > sschra.bin 
rm sschr1.bin 
rm sschr2.bin 
rm sschr3.bin 
rm sschr4.bin 
rm sschr5.bin 

../../../src/utils/fillto sschra.bin 3968

../../../src/utils/legacy/mkts_old in=proj.png pals=$(pwd)/palssb.png out=sschr5.bin mode=chars offset=0,0 size=8,1 silent

cat sschra.bin sschr5.bin > sschr.bin 

rm sschra.bin 
rm sschr5.bin 
../../../src/utils/fillto sschr.bin 4096

cat tschr.bin sschr.bin >../dev/tileset.chr 

rm tschr.bin 
rm sschr.bin 

cd ../dev
wine ../../../cc65_2.13.2/bin/cc65.exe -Oi game.c --add-source
wine ../../../cc65_2.13.2/bin/ca65.exe crt0.s -o crt0.o
wine ../../../cc65_2.13.2/bin/ca65.exe game.s
#rem ca65 scripts.s
#rem ld65 -v -C ../../nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
wine ../../../cc65_2.13.2/bin/ld65.exe -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

rm *.o
rm game.s

cp  cart.nes ../../d'veel'ng-omv.nes 

:end
rm work/*.h 

echo DONE

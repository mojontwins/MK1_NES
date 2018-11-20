@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palts2.h label=palts2 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
copy /b work\*.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr silent

echo Exporting RLE'd screens
..\..\..\src\utils\namgen.exe in=hud.png out=..\dev\assets\hud_rle.h pals=palts0.png chr=..\dev\tileset.chr bank=0 rle=hud_rle
..\..\..\src\utils\namgen.exe in=title.png out=..\dev\assets\title_rle.h pals=palts1.png chr=..\dev\tileset.chr bank=0 rle=title_rle

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1 gencounter
..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\assets\enems1.h 1 1 gencounter
..\..\..\src\utils\eneexp3.exe level2.ene ..\dev\assets\enems2.h 2 1 gencounter

echo Compiling enembehs
cd ..\script
..\..\..\src\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h

echo Making map
cd ..\map
..\..\..\src\utils\rle44mapMK1.exe ..\map\level0.map ..\dev\assets\map0.h 4 6 15 0 1 scrsizes nodecos
..\..\..\src\utils\rle44mapMK1.exe ..\map\level1.map ..\dev\assets\map1.h 4 6 15 1 1 scrsizes
..\..\..\src\utils\rle44mapMK1.exe ..\map\level2.map ..\dev\assets\map2.h 8 3 15 2 0 scrsizes

echo Exporting music and sound
cd ..\dev
..\..\..\src\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\..\..\src\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

cd ..\dev

if [%1]==[nocompile] goto :end

:justcompile
if [%2]==[noscript] goto :noscript

echo Building script
cd ..\script
..\..\..\src\utils\mscmk1.exe script.spt ..\dev\assets\mscnes.h 25
cd ..\dev

:noscript
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o -D CNROM=0
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

copy cart.nes ..\..\cheril_perils_classic.nes

del *.o
del game.s

:end 
del work\*.h /q 2> nul
del work\*.bin /q 2> nul

echo DONE!

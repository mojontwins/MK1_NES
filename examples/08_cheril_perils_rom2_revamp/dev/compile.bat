@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss1.png out=work\palss1.h label=palss1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\paltsgrey.png out=work\paltsgrey.h label=paltsgrey silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palssgrey.png out=work\palssgrey.h label=palssgrey silent

copy /b work\palts0.h + work\palss0.h + work\palts1.h + work\palss1.h + work\paltsgrey.h + work\palssgrey.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns0.spt out=..\dev\tileset0.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns1.spt out=..\dev\tileset1.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns2.spt out=..\dev\tileset2.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns3.spt out=..\dev\tileset3.chr silent

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1 gencounter
..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\assets\enems1.h 1 1 gencounter

echo Compiling enembehs
cd ..\script
..\..\..\src\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h

echo Making map
cd ..\map
..\..\..\src\utils\rle53mapMK1.exe ..\map\level0.map ..\dev\assets\map0.h 6 4 15 0
..\..\..\src\utils\rle53mapMK1.exe ..\map\level1.map ..\dev\assets\map1.h 8 3 99 1 1

echo Exporting music and sound
cd ..\dev
..\..\..\src\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\..\..\src\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

cd ..\dev

:justcompile
if [%2]==[noscript] goto :noscript

echo Building script
cd ..\script
..\..\..\src\utils\mscmk1.exe script.spt 30
copy mscnes.h ..\dev\assets\ > nul
cd ..\dev

:noscript
cc65 -Oi game.c --add-source -D CNROM
ca65 crt0.s -o crt0.o -D CNROM=1
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o
del game.s

copy cart.nes ..\..\cheril_perils_revamp.nes

echo DONE!

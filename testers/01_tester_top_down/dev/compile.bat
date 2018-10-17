@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent aseprite
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent aseprite
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent aseprite
copy /b work\palts0.h + work\palts1.h + work\palss0.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr silent

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1
..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\assets\enems1.h 1 1

echo Making map
cd ..\map
..\..\..\src\utils\mapcnvnes2.exe level0.map ..\dev\assets\map0.h 1 24 15 0 packed
..\..\..\src\utils\mapcnvnes2.exe level1.map ..\dev\assets\map1.h 1 24 15 1 packed

echo Exporting music and sound
cd ..\dev
..\..\..\src\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\..\..\src\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

cd ..\dev

:justcompile
if [%2]==[noscript] goto :noscript
if [%1]==[noscript] goto :noscript

echo Building script
cd ..\script
..\..\..\src\utils\mscmk1.exe script.spt ..\dev\assets\mscnes.h 24
cd ..\dev

:noscript
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o -D CNROM=0
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o
rem del game.s
del work\*.h /q

copy cart.nes ..\..\sgt_helmet_training_day_R1.0.nes > nul

echo DONE!

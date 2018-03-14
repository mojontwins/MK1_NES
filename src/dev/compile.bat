@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
copy /b work\palts0.h + work\palss0.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr silent

echo Exporting enems
cd ..\enems
..\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1 gencounter

echo Compiling enembehs
cd ..\script
..\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h

echo Making map
cd ..\map
..\utils\rlemap2.exe ..\map\level0.map ..\dev\assets\map0.h 5 5 15 0

echo Exporting music and sound
cd ..\dev
..\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

cd ..\dev

:justcompile
if [%2]==[noscript] goto :noscript

echo Building script
cd ..\script
..\utils\mscmk1.exe script.spt 30
copy mscnes.h ..\dev\assets\ > nul
cd ..\dev

:noscript
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o -D CNROM=0
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o
del game.s

echo DONE!

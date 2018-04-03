@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss1.png out=work\palss1.h label=palss1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palts2.h label=palts2 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss2.png out=work\palss2.h label=palss2 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts3.png out=work\palts3.h label=palts3 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss3.png out=work\palss3.h label=palss3 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\paltsgrey.png out=work\paltsgrey.h label=paltsgrey silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palssgrey.png out=work\palssgrey.h label=palssgrey silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palcuts0.png out=work\palcuts0.h label=palcuts0 silent

copy /b work\palts0.h + work\palss0.h + work\palts1.h + work\palss1.h + work\palts2.h + work\palss2.h + work\palts3.h + work\palss3.h + work\palssgrey.h + works\aplcuts0.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns0.spt out=..\dev\tileset0.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns1.spt out=..\dev\tileset1.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns2.spt out=..\dev\tileset2.chr silent

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1 gencounter
..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\assets\enems1.h 1 1 gencounter
..\..\..\src\utils\eneexp3.exe level2.ene ..\dev\assets\enems2.h 2 1 gencounter
..\..\..\src\utils\eneexp3.exe level3.ene ..\dev\assets\enems3.h 3 1 gencounter
..\..\..\src\utils\eneexp3.exe level4.ene ..\dev\assets\enems4.h 4 1 gencounter

echo Compiling enembehs
cd ..\script
..\..\..\src\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h

echo Making map
cd ..\map
..\..\..\src\utils\rle53mapchrrom.exe in=maplist.txt bin=..\dev\work\mapchr.bin out=..\dev\assets\chr_rom_maps.h chr=3
cd ..\dev
copy work\mapchr.bin.3 tileset3.chr >nul
..\..\..\src\utils\mapcnvnes2.exe ..\map\level0_bg.map ..\dev\assets\bg0.h 1 1 99 bg0
..\..\..\src\utils\mapcnvnes2.exe ..\map\level3_bg.map ..\dev\assets\bg3.h 2 1 99 bg3

echo Exporting music and sound
cd ..\dev
..\..\..\src\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\..\..\src\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

cd ..\dev

:justcompile
if [%2]==[noscript] goto :noscript

rem echo Building script
rem cd ..\script
rem ..\..\..\src\utils\mscmk1.exe script.spt ..\dev\assets\mscnes.h 5
rem cd ..\dev

:noscript
ca65 crt0.s -o crt0.o -D CNROM=1

cc65 -Oi game.c --add-source -D CNROM
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

cc65 -Oi game.c --add-source -D CNROM -D SPANISH
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o cart-sp.nes crt0.o game.o runtime.lib -m labels.txt


del *.o
del game.s

copy cart.nes ..\..\cheril_perils_revamp.nes

echo DONE!

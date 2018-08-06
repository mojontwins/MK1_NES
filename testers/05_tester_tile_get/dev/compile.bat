@echo off

if [%1]==[justcompile] goto :justcompile

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palts2.h label=palts2 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts2a.png out=work\palts2a.h label=palts2a silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts3.png out=work\palts3.h label=palts3 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts3a.png out=work\palts3a.h label=palts3a silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\paltstitle.png out=work\paltstitle.h label=paltstitle silent
copy /b work\pal*.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns0.spt out=..\dev\tileset0.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns1.spt out=..\dev\tileset1.chr silent

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level00.ene ..\dev\work\enems00.h 00 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level01.ene ..\dev\work\enems01.h 01 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level02.ene ..\dev\work\enems02.h 02 1 gencounter bin

..\..\..\src\utils\eneexp3.exe level10.ene ..\dev\work\enems10.h 10 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level11.ene ..\dev\work\enems11.h 11 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level12.ene ..\dev\work\enems12.h 12 1 gencounter bin

..\..\..\src\utils\eneexp3.exe level20.ene ..\dev\work\enems20.h 20 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level21.ene ..\dev\work\enems21.h 21 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level22.ene ..\dev\work\enems22.h 22 1 gencounter bin

..\..\..\src\utils\eneexp3.exe level30.ene ..\dev\work\enems30.h 30 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level31.ene ..\dev\work\enems31.h 31 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level32.ene ..\dev\work\enems32.h 32 1 gencounter bin

cd ..\dev
copy /b work\enems*.h assets\enem_constants.h > nul
..\..\..\src\utils\binpaster.exe index=assets\enem_index.h out=work\enems.bin files=work\enems00.h.bin,work\enems01.h.bin,work\enems02.h.bin,work\enems10.h.bin,work\enems11.h.bin,work\enems12.h.bin,work\enems20.h.bin,work\enems21.h.bin,work\enems22.h.bin,work\enems30.h.bin,work\enems31.h.bin,work\enems32.h.bin

echo Compiling enembehs
cd ..\script
..\..\..\src\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h debug

echo Making map
cd ..\map
..\..\..\src\utils\rle44mapchrrom.exe in=maplist.txt bin=..\dev\work\mapchr.bin out=..\dev\assets\chr_rom_maps.h chr=2
cd ..\dev
..\..\..\src\utils\fillto.exe work\mapchr.bin.3 4096
..\..\..\src\utils\fillto.exe work\enems.bin 4096
copy work\mapchr.bin.2 tileset2.chr
copy /b work\mapchr.bin.3 + work\enems.bin tileset3.chr

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
cc65 -Oi game.c --add-source -D CNROM
ca65 crt0.s -o crt0.o -D CNROM=1
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o > nul
del game.s > nul
del work\*.h /q > nul
del work\*.bin.* /q > nul  2> nul

echo DONE!

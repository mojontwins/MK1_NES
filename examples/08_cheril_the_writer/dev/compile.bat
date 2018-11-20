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
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palssgrey.png out=work\palssgrey.h label=palssgrey silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palcuts0.png out=work\palcuts0.h label=palcuts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\paltitle.png out=work\paltitle.h label=paltitle silent

copy /b work\palts0.h + work\palss0.h + work\palts1.h + work\palss1.h + work\palts2.h + work\palss2.h + work\palts3.h + work\palss3.h + work\palssgrey.h + work\palcuts0.h + work\paltitle.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns0.spt out=..\dev\tileset0.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns1.spt out=..\dev\tileset1.chr silent
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns2.spt out=..\dev\tileset2.bin silent

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\work\enems0.h 0 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level1.ene ..\dev\work\enems1.h 1 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level2.ene ..\dev\work\enems2.h 2 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level3.ene ..\dev\work\enems3.h 3 1 gencounter bin
..\..\..\src\utils\eneexp3.exe level4.ene ..\dev\work\enems4.h 4 1 gencounter bin
cd ..\dev\work
copy /b enems0.h + enems1.h + enems2.h + enems3.h + enems4.h ..\assets\enem_constants.h > nul
..\..\..\..\src\utils\binpaster.exe index=..\assets\enem_index.h out=..\enems.bin files=enems0.h.bin,enems1.h.bin,enems2.h.bin,enems3.h.bin,enems4.h.bin
cd ..
copy /b tileset2.bin + enems.bin tileset2.chr 
del tileset2.bin > nul
del enems.bin > nul

echo Exporting RLE'd screens
cd ..\gfx
..\..\..\src\utils\namgen.exe in=hud.png out=..\dev\assets\hud_rle.h pals=palts0.png chr=..\dev\tileset0.chr bank=0 rle=hud_rle
..\..\..\src\utils\namgen.exe in=cuts0.png out=..\dev\assets\cuts0_rle.h pals=palcuts0.png chr=..\dev\tileset2.chr bank=0 rle=cuts0_rle
..\..\..\src\utils\namgen.exe in=cuts1.png out=..\dev\assets\cuts1_rle.h pals=palcuts0.png chr=..\dev\tileset2.chr bank=0 rle=cuts1_rle
..\..\..\src\utils\namgen.exe in=cuts2.png out=..\dev\assets\cuts2_rle.h pals=palcuts0.png chr=..\dev\tileset2.chr bank=0 rle=cuts2_rle
..\..\..\src\utils\namgen.exe in=cuts3.png out=..\dev\assets\cuts3_rle.h pals=palcuts0.png chr=..\dev\tileset2.chr bank=0 rle=cuts3_rle
..\..\..\src\utils\namgen.exe in=title_sp.png out=..\dev\assets\title_sp_rle.h pals=paltitle.png chr=..\dev\tileset2.chr bank=1 rle=title_sp_rle
..\..\..\src\utils\namgen.exe in=title_en.png out=..\dev\assets\title_en_rle.h pals=paltitle.png chr=..\dev\tileset2.chr bank=1 rle=title_en_rle

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


del *.o > nul
del game.s > nul
del work\*.h /q 2> nul
del work\*.bin.* /q 2> nul

copy cart.nes ..\..\cheril_the_writer.nes

echo DONE!

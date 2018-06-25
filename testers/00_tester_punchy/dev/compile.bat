@echo off

echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
copy /b work\palts0.h + work\palss0.h assets\palettes.h > nul

echo Exporting enems
cd ..\enems
..\..\..\src\utils\eneexp3.exe level0.ene ..\dev\assets\enems0.h 0 1 gencounter

echo Compiling enembehs
cd ..\script
..\..\..\src\utils\pencompiler.exe enembehs.spt ..\dev\assets\compiled_enems.h

echo Making map
cd ..\map
..\..\..\src\utils\legacy\rlemap2.exe ..\map\level0.map ..\dev\assets\map0.h 16 1 99 0 1
..\..\..\src\utils\mapcnvnes2.exe ..\map\bg0.map ..\dev\assets\bg0.h 2 1 99 bg0 1

echo Exporting music and sound
cd ..\dev
..\..\..\src\utils\text2data.exe ..\ogt\music.txt -ca65 -ch1..4
..\..\..\src\utils\nsf2data.exe ..\ogt\sounds.nsf -ca65 -ntsc
copy ..\ogt\music.s > nul
copy ..\ogt\sounds.s > nul

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr silent

cd ..\dev

rem echo Building script
rem cd ..\script
rem ..\..\..\src\utils\mscmk1.exe script.spt ..\dev\assets\mscnes.h 5
rem cd ..\dev

cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o -D CNROM=0
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o
del game.s

copy cart.nes ..\..\tester_punchy.nes

if [%1]==[omv] goto :omv
goto :end

:omv

echo Exporting chr
cd ..\gfx
..\..\..\src\utils\mkts.exe mode=scripted in=import_patterns-omv.spt out=..\dev\tileset.chr silent

cd ..\dev

rem echo Building script
rem cd ..\script
rem ..\..\..\src\utils\mscmk1.exe script.spt ..\dev\assets\mscnes.h 5
rem cd ..\dev

cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o -D CNROM=0
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt

del *.o
del game.s

copy cart.nes ..\..\tester_punchy-omv.nes


:end

echo DONE!

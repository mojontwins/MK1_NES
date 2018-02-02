@echo off

if [%1]==[justcompile] goto :justcompile
echo Generating pals
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palts1.png out=work\palbg0.h label=mypal_game_bg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palss1.png out=work\palfg0.h label=mypal_game_fg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\paltsl.png out=work\paltsl.h label=mypal_lighting_bg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palssl.png out=work\palssl.h label=mypal_lighting_fg0 silent
rem ..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palredbg.png out=work\palred.h label=mypal_reds silent
copy /b work\palbg0.h + work\palfg0.h + work\paltsl.h + work\palssl.h + work\palred.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

rem charset, tileset, fill to 4096
..\..\..\src\utils\legacy\mkts_old.exe in=font.png pals=palts1.png out=tschr1.bin mode=chars size=32,2 silent
..\..\..\src\utils\legacy\mkts_old.exe in=ts.png pals=palts1.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=64 silent
rem ..\..\..\src\utils\legacy\mkts_old.exe in=cuts.png pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=185 max=32 silent

copy /b tschr1.bin + tschr3.bin tschr.bin > nul
..\..\..\src\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr3.bin > nul
rem del tschr4.bin > nul

..\..\..\src\utils\legacy\mkts_old.exe in=ss.png pals=palss1.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=3,1 metasize=2,3 sprorg=0,-8 label=spr_pl genflipped silent
..\..\..\src\utils\legacy\mkts_old.exe in=ss.png pals=palss1.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,3 size=8,1 metasize=2,2 sprorg=0,0 label=spr_en genflipped tmapoffset=18 genflipped silent 
..\..\..\src\utils\legacy\mkts_old.exe in=ss.png pals=palss1.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs.h offset=0,5 size=3,2 metasize=2,2 sprorg=0,0 label=spr_hs tmapoffset=44 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
..\..\..\src\utils\fillto.exe sschra.bin 4096

copy /b sschra.bin  sschr.bin > nul
del sschra.bin > nul

copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul

del tschr.bin > nul
del sschr.bin > nul

echo Exporting enems
cd ..\enems
..\..\..\src\utils\legacy\eneexp2.exe enems0.ene ..\dev\assets\enems0.h 0
echo Making map
cd ..\map
..\..\..\src\utils\legacy\rlemap.exe level0.map ..\dev\assets\mapa0.h 6 5 15 0


cd ..\dev
:justcompile
if [%2]==[noscript] goto :noscript
rem echo Building script
rem cd ..\script
rem ..\..\..\src\utils\msc3nes.exe script.spt 30
rem copy mscnes.h ..\dev\assets\ > nul
rem cd ..\dev
:noscript
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
rem ca65 scripts.s
rem ld65 -v -C nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o
del game.s

copy cart.nes ..\..\lala_prologue.nes
echo DONE!
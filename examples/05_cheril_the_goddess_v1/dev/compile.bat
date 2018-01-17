@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts.png out=work\palbg0.h label=mypal_game_bg0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palbg1.h label=mypal_game_bg1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts3.png out=work\palbg2.h label=mypal_game_bg2 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palts4.png out=work\palbg3.h label=mypal_game_bg3 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palss.png out=work\palfg0.h label=mypal_game_fg0 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palssp.png out=work\palfg1.h label=mypal_game_fg1 silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\paltsl.png out=work\paltsl.h label=mypal_light_bg silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palssl.png out=work\palssl.h label=mypal_light_fg silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\..\..\src\utils\mkts.exe mode=pals pals=..\gfx\palredbg.png out=work\palred.h label=mypal_reds silent
copy /b work\palbg0.h + work\palbg1.h + work\palbg2.h + work\palbg3.h + work\palfg0.h + work\palfg1.h + work\paltsl.h + work\palssl.h + work\palcuts.h + work\palred.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

echo BANKS 0,1
..\..\..\src\utils\mkts.exe mode=scripted in=cut_tileset0.spt out=..\dev\tileset0.chr silent

echo BANKS 2, 3
..\..\..\src\utils\mkts.exe mode=scripted in=cut_tileset1.spt out=..\dev\tileset1.chr silent

echo BANKS 4,5
..\..\..\src\utils\mkts.exe mode=scripted in=cut_tileset2.spt out=..\dev\tileset2.chr silent

echo dummy banks 6, 7
..\..\..\src\utils\fillto.exe empty.bin 8192
copy empty.bin ..\dev\tileset3.chr > nul
del empty.bin > nul

echo Exporting enems
cd ..\enems
..\..\..\src\utils\legacy\eneexp2.exe enems.ene ..\dev\assets\enems.h 0
echo Making map
cd ..\map
..\..\..\src\utils\legacy\rlemap.exe mapa.map ..\dev\assets\mapa.h 20 4 99 0
rem cd ..\script
rem ..\..\..\src\utils\mscnes script.spt 24
rem copy mscnes.h ..\dev\assets\ > nul
rem copy scripts.s ..\dev > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0-CNROM.s -o crt0.o
ca65 game.s
rem ca65 scripts.s
rem ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
ld65 -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o
del game.s

copy cart.nes ..\cheril_the_goddess.nes > nul

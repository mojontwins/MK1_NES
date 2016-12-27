@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\palts.png out=work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss.png out=work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\paltsl.png out=work\paltsl.h label=mypal_lighting_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palssl.png out=work\palssl.h label=mypal_lighting_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\utils\mkts.exe mode=pals pals=..\gfx\palredbg.png out=work\palred.h label=mypal_reds silent
copy /b work\palbg0.h + work\palfg0.h + work\paltsl.h + work\palssl.h + work\palcuts.h + work\palred.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

rem charset, tileset, fill to 4096
..\utils\mkts.exe in=font.png pals=palts.png out=tschr1.bin mode=chars size=32,2 silent
..\utils\mkts.exe in=frame.png pals=palts.png out=tschr2.bin mode=chars size=8,1 silent
..\utils\mkts.exe in=ts.png pals=palts.png out=tschr3.bin mode=mapped offset=0,0 size=16,3 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=72 silent
..\utils\mkts.exe in=cuts.png pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=154 max=32 silent

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin tschr.bin > nul
..\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul

..\utils\mkts.exe in=ss.png pals=palss.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=8,2 metasize=2,3 sprorg=0,-8 label=spr_pl max=9 genflipped silent
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,6 size=8,1 metasize=2,2 sprorg=0,0 label=spr_en genflipped tmapoffset=39 genflipped silent 
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs.h offset=0,10 size=8,1 metasize=2,2 sprorg=0,0 label=spr_hs tmapoffset=69 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
..\utils\fillto.exe sschra.bin 3968

..\utils\mkts.exe in=ssbits.png pals=palss.png out=sschrb.bin mode=chars size=8,1 silent
copy /b sschra.bin + sschrb.bin sschr.bin > nul
del sschra.bin > nul
del sschrb.bin > nul

copy /b tschr.bin + sschr.bin ..\dev\tileset0.chr > nul

del tschr.bin > nul
del sschr.bin > nul

echo Exporting enems
cd ..\enems
..\utils\eneexp2.exe enems.ene ..\dev\assets\enems.h 0
echo Making map
cd ..\map
..\utils\rlemap.exe mapa.map ..\dev\assets\mapa.h 6 5 15 0
rem cd ..\script
rem ..\utils\mscnes script.spt 24
rem copy mscnes.h ..\dev\assets\ > nul
rem copy scripts.s ..\dev > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 ..\..\crt0.s -o crt0.o
ca65 game.s
rem ca65 scripts.s
rem ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o
del game.s

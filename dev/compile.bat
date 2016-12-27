@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\palts.png out=work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palbg1.h label=mypal_game_bg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts3.png out=work\palbg2.h label=mypal_game_bg2 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts4.png out=work\palbg3.h label=mypal_game_bg3 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss.png out=work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palssp.png out=work\palfg1.h label=mypal_game_fg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\paltsl.png out=work\paltsl.h label=mypal_light_bg silent
..\utils\mkts.exe mode=pals pals=..\gfx\palssl.png out=work\palssl.h label=mypal_light_fg silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\utils\mkts.exe mode=pals pals=..\gfx\palredbg.png out=work\palred.h label=mypal_reds silent
copy /b work\palbg0.h + work\palbg1.h + work\palbg2.h + work\palbg3.h + work\palfg0.h + work\palfg1.h + work\paltsl.h + work\palssl.h + work\palcuts.h + work\palred.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

rem charset, tileset, fill to 4096
..\utils\mkts.exe in=font.png pals=palts.png out=tschr1.bin mode=chars size=32,2 silent
..\utils\mkts.exe in=frame.png pals=palts.png out=tschr2.bin mode=chars size=8,1 silent
..\utils\mkts.exe in=ts.png pals=palts.png out=tschr3.bin mode=mapped offset=0,0 size=16,4 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=72 max=56 silent
..\utils\mkts.exe in=cuts.png pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=187 max=32 silent

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin tschr.bin > nul
..\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul

..\utils\mkts.exe in=ss.png pals=palss.png out=sschr1a.bin mode=sprites tsmap=..\dev\assets\spr_pla.h offset=0,0 size=8,2 metasize=2,3 sprorg=0,-8 label=spr_pla genflipped silent
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr1b.bin mode=sprites tsmap=..\dev\assets\spr_plb.h offset=0,6 size=8,1 metasize=2,3 sprorg=0,-8 label=spr_plb tmapoffset=83 silent
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_lin.h offset=0,9 size=8,1 metasize=2,2 sprorg=0,0 label=spr_lin tmapoffset=128 genflipped silent 
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_enf.h offset=0,11 size=8,1 metasize=2,2 sprorg=0,0 label=spr_enf tmapoffset=158 silent
..\utils\mkts.exe in=ss.png pals=palss.png out=sschr4.bin mode=sprites tsmap=..\dev\assets\spr_hs.h offset=0,13 size=8,1 metasize=2,2 sprorg=0,0 label=spr_hs tmapoffset=180 silent

copy /b sschr1a.bin + sschr1b.bin + sschr2.bin + sschr3.bin + sschr4.bin sschra.bin > nul
del sschr1a.bin > nul
del sschr1b.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul
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
..\utils\rlemap.exe mapa.map ..\dev\assets\mapa.h 20 4 99 0
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

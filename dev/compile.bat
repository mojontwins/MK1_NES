@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\paltsb.png out=work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palssb.png out=work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\paltsl.png out=work\palf0.h label=mypal_lighting_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palssl.png out=work\palf1.h label=mypal_lighting_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\utils\mkts.exe mode=pals pals=..\gfx\palredbg.png out=work\palredbg.h label=mypal_redbg silent
copy /b work\palbg0.h + work\palfg0.h + work\palf0.h + work\palf1.h + work\palcuts.h + work\palredbg.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

..\utils\mkts.exe in=font.png pals=paltsb.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
..\utils\mkts.exe in=frame.png pals=paltsb.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
..\utils\mkts.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=72 max=24 silent
..\utils\mkts.exe in=cuts.png pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=152 max=32 silent

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin tschr.bin > nul
..\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul

rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

..\utils\mkts.exe in=ssb-cheril.png pals=palssb.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=6,5 metasize=2,3 sprorg=0,-8 label=spr_pl max=26 silent
..\utils\mkts.exe in=ssb-enems.png pals=palssb.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,0 size=11,2 metasize=2,2 sprorg=0,0 label=spr_en max=15 tmapoffset=129 silent
..\utils\mkts.exe in=hostage.png pals=palssb.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs1.h offset=0,0 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_hs1 max=4 tmapoffset=184 silent
..\utils\mkts.exe in=items.png pals=palssb.png out=sschr4.bin mode=sprites tsmap=..\dev\assets\spr_hs2.h offset=0,0 size=3,1 metasize=2,2 sprorg=0,0 label=spr_hs2 max=3 tmapoffset=208 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin + sschr4.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul

..\utils\fillto sschra.bin 3968

..\utils\mkts.exe in=proj.png pals=palssb.png out=sschr5.bin mode=chars offset=0,0 size=8,1 silent

copy /b sschra.bin + sschr5.bin sschr.bin > nul

del sschra.bin > nul
del sschr5.bin > nul
..\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset0.chr > nul

del tschr.bin > nul
del sschr.bin > nul

echo Copying enems
cd ..\enems
..\utils\eneexp2.exe enemsd.ene ..\dev\assets\enems.h 0 > nul
echo Making map
cd ..\map
..\utils\mapcnvnes dveel.map 5 5 16 12 15 mapa0 packed
copy mapa0.h ..\dev\assets\ > nul
del mapa?.h
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

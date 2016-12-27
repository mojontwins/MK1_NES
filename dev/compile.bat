@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\palts2A.png out=work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts2B.png out=work\palbg1.h label=mypal_game_bg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palbn.png out=work\palbn.h label=mypal_bw silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss2A.png out=work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss2B.png out=work\palfg1.h label=mypal_game_fg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palflash.png out=work\palf.h label=mypal_flash silent
..\utils\mkts.exe mode=pals pals=..\gfx\palblack.png out=work\palb.h label=mypal_black silent
copy /b work\palbg0.h + work\palbg1.h + work\palbn.h + work\palfg0.h + work\palfg1.h + work\palf.h + work\palb.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

..\utils\mkts.exe in=font.png pals=palts2A.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
..\utils\mkts.exe in=frame.png pals=palts2A.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
..\utils\mkts.exe in=ts2.png pals=palts2A.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=72 max=32 silent
..\utils\mkts.exe in=cuts2.png pals=palcuts2.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=148 max=32 silent

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin tschr.bin > nul
..\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul

rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=16,1 metasize=2,3 sprorg=0,-8 label=spr_pl max=16 silent
..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,3 size=16,2 metasize=2,2 sprorg=0,0 label=spr_en max=26 tmapoffset=82 silent
..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs.h offset=24,5 size=5,1 metasize=2,2 sprorg=0,0 label=spr_hs max=5 tmapoffset=162 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul

..\utils\fillto sschra.bin 3584

..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr4.bin mode=chars offset=0,7 size=32,1 silent

copy /b sschra.bin + sschr4.bin sschr.bin > nul

del sschra.bin > nul
del sschr4.bin > nul
..\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset0.chr > nul

del tschr.bin > nul
del sschr.bin > nul

echo Copying enems
cd ..\enems
copy enems0.h ..\dev\assets > nul
copy enems1.h ..\dev\assets > nul
echo Making map
cd ..\map
..\utils\packmap.exe mapa0.map ..\dev\assets\mapa0.h 6 4 15 0
..\utils\packmap.exe mapa1.map ..\dev\assets\mapa1.h 6 4 15 1
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

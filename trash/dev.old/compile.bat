@echo off
path=%path%;c:\cc65\bin\
set CC65_HOME=c:\cc65\
echo Generating pals
..\utils\mkts.exe mode=pals pals=..\gfx\palts2A.png out=work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts2B.png out=work\palbg1.h label=mypal_game_bg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palbn.png out=work\palbn.h label=mypal_bw silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss2A.png out=work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palss2B.png out=work\palfg1.h label=mypal_game_fg1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts2.png out=work\palc1.h label=mypal_cutscene silent
copy /b work\palbg0.h + work\palbg1.h + work\palbn.h + work\palfg0.h + work\palfg1.h + work\palc1.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

rem charset, tileset, fill to 4096
..\utils\mkts.exe in=font.png pals=palts2A.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
..\utils\mkts.exe in=frame.png pals=palts2A.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
..\utils\mkts.exe in=ts2.png pals=palts2A.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=72 max=24 silent
..\utils\mkts.exe in=cuts2.png pals=palcuts2.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=144 max=32 silent
rem next = 235

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin tschr.bin > nul
..\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul

rem main sprite (16x24)x16, enemy sprite frames (16x16)x18, fill to 4096
..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr1.bin mode=tiles offset=0,0 size=16,1 metasize=2,3 silent
..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr2.bin mode=tiles offset=0,3 size=16,2 silent
..\utils\mkts.exe in=ss2.png pals=palss2A.png out=sschr3.bin mode=chars offset=0,7 size=32,1 silent

copy /b sschr1.bin + sschr2.bin +sschr3.bin sschr.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
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
..\utils\mapcnvnes mapa0.map 4 6 16 12 15 mapa0 packed
..\utils\mapcnvnes mapa1.map 4 6 16 12 15 mapa1 packed
copy mapa0.h ..\dev\assets\ > nul
copy mapa1.h ..\dev\assets\ > nul
del mapa?.h
cd ..\script
..\utils\mscnes script.spt 24
copy mscnes.h ..\dev\assets\ > nul
copy scripts.s ..\dev > nul
cd ..\dev
cc65 -Oi game.c --add-source
ca65 ..\..\crt0.s -o crt0.o
ca65 game.s
ca65 scripts.s
ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib

del *.o
del game.s

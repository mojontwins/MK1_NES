@echo off

echo Generating pals
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\paltsb.png out=work\palbg0.h label=mypal_game_bg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palssb.png out=work\palfg0.h label=mypal_game_fg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\paltsl.png out=work\palf0.h label=mypal_lighting_bg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palssl.png out=work\palf1.h label=mypal_lighting_fg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palcuts.png out=work\palcuts.h label=mypal_cuts silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=..\gfx\palredbg.png out=work\palredbg.h label=mypal_redbg silent
copy /b work\palbg0.h + work\palfg0.h + work\palf0.h + work\palf1.h + work\palcuts.h + work\palredbg.h assets\palettes.h > nul

echo Copying enems
cd ..\enems
..\..\..\src\utils\legacy\eneexp2.exe enemsd.ene ..\dev\assets\enems.h 0 > nul
echo Making map
cd ..\map
..\..\..\src\utils\legacy\mapcnvnes.exe dveel.map 5 5 16 12 15 mapa0 packed
copy mapa0.h ..\dev\assets\ > nul
del mapa?.h
rem cd ..\script
rem ..\..\..\src\utils\mscnes script.spt 24
rem copy mscnes.h ..\dev\assets\ > nul
rem copy scripts.s ..\dev > nul

echo Exporting chr
cd ..\gfx

..\..\..\src\utils\legacy\mkts_old.exe in=font.png pals=paltsb.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
..\..\..\src\utils\legacy\mkts_old.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=64 max=24 silent
..\..\..\src\utils\legacy\mkts_old.exe in=cuts.png pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=144 max=32 silent
..\..\..\src\utils\legacy\mkts_old.exe in=tstitle.png pals=palcuts.png out=tschr5.bin mode=mapped offset=0,0 size=9,1 tsmap=..\dev\assets\tiledata_title.h label=title tmapoffset=216 silent

copy /b tschr1.bin + tschr3.bin + tschr4.bin + tschr5.bin tschr.bin > nul
..\..\..\src\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul
del tschr5.bin > nul

rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

..\..\..\src\utils\mkts.exe in=ssb-cheril.png pals=palssb.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=6,5 metasize=2,3 sprorg=0,-8 label=spr_pl max=26 silent
..\..\..\src\utils\mkts.exe in=ssb-enems.png pals=palssb.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,0 size=11,2 metasize=2,2 sprorg=0,0 label=spr_en max=15 tmapoffset=102 silent
..\..\..\src\utils\mkts.exe in=hostage.png pals=palssb.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs1.h offset=0,0 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_hs1 max=4 tmapoffset=144 silent
..\..\..\src\utils\mkts.exe in=items.png pals=palssb.png out=sschr4.bin mode=sprites tsmap=..\dev\assets\spr_hs2.h offset=0,0 size=3,1 metasize=2,2 sprorg=0,0 label=spr_hs2 max=3 tmapoffset=156 silent
..\..\..\src\utils\mkts.exe in=logo.png pals=palcuts.png out=sschr5.bin mode=sprites tsmap=..\dev\assets\spr_logo.h offset=0,0 size=1,1 metasize=6,2 sprorg=0,0 label=spr_logo tmapoffset=168 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin + sschr4.bin + sschr5.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul
del sschr5.bin > nul

..\..\..\src\utils\fillto sschra.bin 3968

..\..\..\src\utils\legacy\mkts_old.exe in=proj.png pals=palssb.png out=sschr5.bin mode=chars offset=0,0 size=8,1 silent

copy /b sschra.bin + sschr5.bin sschr.bin > nul

del sschra.bin > nul
del sschr5.bin > nul
..\..\..\src\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul

del tschr.bin > nul
del sschr.bin > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
rem ca65 scripts.s
rem ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o
del game.s

copy cart.nes ..\..\d'veel'ng.nes > nul

if [%1]==[omv] goto :omv
goto :end
:omv

echo Exporting chr
cd ..\gfx

..\..\..\src\utils\legacy\mkts_old.exe in=font.png pals=paltsb.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
..\..\..\src\utils\legacy\mkts_old.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledata1.h label=ts1 tmapoffset=64 max=24 silent
..\..\..\src\utils\legacy\mkts_old.exe in=cuts.omv pals=palcuts.png out=tschr4.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cuts1.h label=cuts1 tmapoffset=144 max=32 silent
..\..\..\src\utils\legacy\mkts_old.exe in=tstitle.png pals=palcuts.png out=tschr5.bin mode=mapped offset=0,0 size=9,1 tsmap=..\dev\assets\tiledata_title.h label=title tmapoffset=216 silent

copy /b tschr1.bin + tschr3.bin + tschr4.bin + tschr5.bin tschr.bin > nul
..\..\..\src\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul
del tschr5.bin > nul

rem main sprite (16x24)x16, enemy sprite frames (16x16)x32, small charset (8x8)x32,  fill to 4096

..\..\..\src\utils\mkts.exe in=ssb-cheril.omv pals=palssb.png out=sschr1.bin mode=sprites tsmap=..\dev\assets\spr_pl.h offset=0,0 size=6,5 metasize=2,3 sprorg=0,-8 label=spr_pl max=26 silent
..\..\..\src\utils\mkts.exe in=ssb-enems.png pals=palssb.png out=sschr2.bin mode=sprites tsmap=..\dev\assets\spr_en.h offset=0,0 size=11,2 metasize=2,2 sprorg=0,0 label=spr_en max=15 tmapoffset=101 silent
..\..\..\src\utils\mkts.exe in=hostage.omv pals=palssb.png out=sschr3.bin mode=sprites tsmap=..\dev\assets\spr_hs1.h offset=0,0 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_hs1 max=4 tmapoffset=143 silent
..\..\..\src\utils\mkts.exe in=items.png pals=palssb.png out=sschr4.bin mode=sprites tsmap=..\dev\assets\spr_hs2.h offset=0,0 size=3,1 metasize=2,2 sprorg=0,0 label=spr_hs2 max=3 tmapoffset=162 silent
..\..\..\src\utils\mkts.exe in=logo.png pals=palcuts.png out=sschr5.bin mode=sprites tsmap=..\dev\assets\spr_logo.h offset=0,0 size=1,1 metasize=6,2 sprorg=0,0 label=spr_logo tmapoffset=168 silent

copy /b sschr1.bin + sschr2.bin + sschr3.bin + sschr4.bin + sschr5.bin sschra.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul
del sschr5.bin > nul

..\..\..\src\utils\fillto sschra.bin 3968

..\..\..\src\utils\legacy\mkts_old.exe in=proj.png pals=palssb.png out=sschr5.bin mode=chars offset=0,0 size=8,1 silent

copy /b sschra.bin + sschr5.bin sschr.bin > nul

del sschra.bin > nul
del sschr5.bin > nul
..\..\..\src\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul

del tschr.bin > nul
del sschr.bin > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
rem ca65 scripts.s
rem ld65 -v -C ..\..\nes.cfg -o cart.nes crt0.o game.o scripts.o runtime.lib
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o
del game.s

copy cart.nes ..\..\d'veel'ng-omv.nes > nul

:end

echo DONE!

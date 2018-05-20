@echo off

cd ..\gfx

echo Generating pals
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=paltsb.png out=..\dev\work\palbg0.h label=mypal_game_bg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=palbn.png out=..\dev\work\palbn.h label=mypal_bw silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=palssb.png out=..\dev\work\palfg0.h label=mypal_game_fg0 silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=palcuts1.png out=..\dev\work\palc1.h label=mypal_cutscene silent
..\..\..\src\utils\legacy\mkts_old.exe mode=pals pals=paltstitle.png out=..\dev\work\palt.h label=mypal_title silent
copy /b ..\dev\work\palbg0.h + ..\dev\work\palbn.h + ..\dev\work\palfg0.h + ..\dev\work\palc1.h + ..\dev\work\palt.h ..\dev\assets\palettes.h > nul

echo Exporting enems

cd ..\dev
..\..\..\src\utils\legacy\eneexp.exe ..\enems\enems.ene assets\enems.h ROM0

echo Making map
cd ..\map
..\..\..\src\utils\legacy\mapcnvnes.exe map_b.map 5 5 16 12 15 map_b packed
copy map_b.h ..\dev\assets\ > nul
del map_b.h > nul

cd ..\gfx

echo Exporting chr
rem charset, tileset, fill to 4096
..\..\..\src\utils\legacy\mkts_old.exe in=font.png pals=paltstitle.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
rem 64 patterns
..\..\..\src\utils\legacy\mkts_old.exe in=frame.png pals=paltsb.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
rem 8 patterns; next offset = 64+8 = 72
..\..\..\src\utils\legacy\mkts_old.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledatab.h label=tsb tmapoffset=72 max=20 silent
rem 61 patterns; next offset = 72 + 61 = 133
..\..\..\src\utils\legacy\mkts_old.exe in=tstitle.png pals=paltstitle.png out=tschr4.bin mode=mapped offset=0,0 size=12,1 tsmap=..\dev\assets\tiledatatitle.h label=tstitle tmapoffset=133 max=12 silent
rem 36 patterns; next offset = 133 + 30 = 169
..\..\..\src\utils\legacy\mkts_old.exe in=cuts1.png pals=palcuts1.png out=tschr5.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cutsb.h label=cutsb tmapoffset=169 max=32 silent
rem 96 patterns; next offset = 163 + 92 = 255

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin + tschr5.bin tschr.bin > nul
..\..\..\src\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul
del tschr5.bin > nul

rem main sprite (16x24)x20, enemy sprite frames (16x16)x8, fill to 4096
..\..\..\src\utils\legacy\mkts_old.exe in=ssb-cheril.png pals=palssb.png out=sschr1.bin mode=tiles offset=0,0 size=5,4 metasize=2,3 silent
rem 120 patterns
..\..\..\src\utils\legacy\mkts_old.exe in=ssb-enems.png pals=palssb.png out=sschr2.bin mode=tiles offset=0,0 size=8,1 metasize=2,2 silent
rem 32 pattern, next 152
..\..\..\src\utils\legacy\mkts_old.exe in=logo.png pals=paltstitle.png out=sschr3.bin mode=chars offset=0,0 size=6,2 silent
rem 12 patterns, next 164
..\..\..\src\utils\legacy\mkts_old.exe in=heart.png pals=palssb.png out=sschr4.bin mode=chars offset=0,0 size=1,1 silent
rem 1 pattern, next 165

copy /b sschr1.bin + sschr2.bin + sschr3.bin + sschr4.bin sschr.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul
..\..\..\src\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul

del tschr.bin > nul
del sschr.bin > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o > nul
del game.s > nul

copy cart.nes ..\..\cheril_of_the_bosque.nes > nul

if [%1]==[omv] goto :omv
goto :end
:omv

cd ..\gfx

echo Exporting chr
rem charset, tileset, fill to 4096
..\..\..\src\utils\legacy\mkts_old.exe in=font.png pals=paltstitle.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
rem 64 patterns
..\..\..\src\utils\legacy\mkts_old.exe in=frame.png pals=paltsb.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
rem 8 patterns; next offset = 64+8 = 72
..\..\..\src\utils\legacy\mkts_old.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledatab.h label=tsb tmapoffset=72 max=20 silent
rem 61 patterns; next offset = 72 + 61 = 133
..\..\..\src\utils\legacy\mkts_old.exe in=tstitle.png pals=paltstitle.png out=tschr4.bin mode=mapped offset=0,0 size=12,1 tsmap=..\dev\assets\tiledatatitle.h label=tstitle tmapoffset=133 max=12 silent
rem 36 patterns; next offset = 133 + 30 = 169
..\..\..\src\utils\legacy\mkts_old.exe in=cuts1.png pals=palcuts1.omv out=tschr5.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cutsb.h label=cutsb tmapoffset=169 max=32 silent
rem 96 patterns; next offset = 163 + 92 = 255

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin + tschr5.bin tschr.bin > nul
..\..\..\src\utils\fillto.exe tschr.bin 4096

del tschr1.bin > nul
del tschr2.bin > nul
del tschr3.bin > nul
del tschr4.bin > nul
del tschr5.bin > nul

rem main sprite (16x24)x20, enemy sprite frames (16x16)x8, fill to 4096
..\..\..\src\utils\legacy\mkts_old.exe in=ssb-cheril.omv pals=palssb.png out=sschr1.bin mode=tiles offset=0,0 size=5,4 metasize=2,3 silent
rem 120 patterns
..\..\..\src\utils\legacy\mkts_old.exe in=ssb-enems.png pals=palssb.png out=sschr2.bin mode=tiles offset=0,0 size=8,1 metasize=2,2 silent
rem 32 pattern, next 152
..\..\..\src\utils\legacy\mkts_old.exe in=logo.png pals=paltstitle.png out=sschr3.bin mode=chars offset=0,0 size=6,2 silent
rem 12 patterns, next 164
..\..\..\src\utils\legacy\mkts_old.exe in=heart.png pals=palssb.png out=sschr4.bin mode=chars offset=0,0 size=1,1 silent
rem 1 pattern, next 165

copy /b sschr1.bin + sschr2.bin + sschr3.bin + sschr4.bin sschr.bin > nul
del sschr1.bin > nul
del sschr2.bin > nul
del sschr3.bin > nul
del sschr4.bin > nul
..\..\..\src\utils\fillto.exe sschr.bin 4096

copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul

del tschr.bin > nul
del sschr.bin > nul

cd ..\dev
cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib

del *.o > nul
del game.s > nul

copy cart.nes ..\..\cheril_of_the_bosque-omv.nes > nul


:end
del work\*.h /q > nul

echo DONE!

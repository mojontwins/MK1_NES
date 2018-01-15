echo Generating pals
..\utils\mkts.exe mode=pals pals=paltsb.png out=..\dev\work\palbg0.h label=mypal_game_bg0 silent
..\utils\mkts.exe mode=pals pals=palbn.png out=..\dev\work\palbn.h label=mypal_bw silent
..\utils\mkts.exe mode=pals pals=palssb.png out=..\dev\work\palfg0.h label=mypal_game_fg0 silent
..\utils\mkts.exe mode=pals pals=palcutse.png out=..\dev\work\palc1.h label=mypal_cutscene silent
..\utils\mkts.exe mode=pals pals=paltstitle.png out=..\dev\work\palt.h label=mypal_title silent
copy /b ..\dev\work\palbg0.h + ..\dev\work\palbn.h + ..\dev\work\palfg0.h + ..\dev\work\palc1.h + ..\dev\work\palt.h ..\dev\assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

rem charset, tileset, fill to 4096
..\utils\mkts.exe in=font.png pals=paltstitle.png out=tschr1.bin mode=chars offset=0,0 size=32,2 silent
rem 64 patterns
..\utils\mkts.exe in=frame.png pals=paltsb.png out=tschr2.bin mode=chars offset=0,0 size=8,1 silent
rem 8 patterns; next offset = 64+8 = 72
..\utils\mkts.exe in=tsb.png pals=paltsb.png out=tschr3.bin mode=mapped offset=0,0 size=16,2 tsmap=..\dev\assets\tiledatab.h label=tsb tmapoffset=72 max=20 silent
rem 62 patterns; next offset = 72 + 62 = 134
..\utils\mkts.exe in=tstitle.png pals=paltstitle.png out=tschr4.bin mode=mapped offset=0,0 size=11,1 tsmap=..\dev\assets\tiledatab.h label=tsb tmapoffset=134 max=11 silent
rem 30 patterns; next offset = 134 + 30 = 164
..\utils\mkts.exe in=cutse.png pals=palcutse.png out=tschr5.bin mode=mapped offset=0,0 size=8,4 tsmap=..\dev\assets\cutsb.h label=cutsb tmapoffset=164 max=32 silent
rem 96 patterns; next offset = 164 + 91 = 255

copy /b tschr1.bin + tschr2.bin + tschr3.bin + tschr4.bin + tschr5.bin tschr.bin > nul
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
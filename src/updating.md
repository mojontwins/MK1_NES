- Overwrite /engine, /mainloop, /ram.
- Make sure /work is empty.
- Add placeholders for behs.h, levelset.h and metasprites.h at /assets, delete every other file.
- Conserve /music
- in /, [generally] conserve compile.bat, config.h, definitions.h, music.s, neslib.*, crt0.s, nes.cfg, setenv.bat, zeropage.inc
- in /, overwrite mainloop.h
- in /, diff game.c

- Basic compile.bat builds one ts pal, one ss pal, builds tileset.chr for import_patterns.spt @ /gfx, builds one set of enems and one map, and attempts to compile a script.
- config.h should ideally contain basic configuration for a non feature rich side view platformer.

// mscnes.h
// Generado por Mojon Script Compiler de la Churrera NES
// Copyleft 2013/2014 The Mojon Twins
 
void __fastcall__ msc_init_all (void) {
    for (gpit = 0; gpit < MAX_FLAGS; gpit ++)
        flags [gpit] = 0;
}
 
unsigned char __fastcall__ read_byte (void) {
    return *script++;
}
 
unsigned char __fastcall__ read_vbyte (void) {
    unsigned char c;
    c = read_byte ();
    if (c & 128) return flags [c & 127];
    return c;
}
 
// Ejecutamos el script apuntado por *script:
void __fastcall__ run_script (void) {
    unsigned char terminado = 0;
    unsigned char continuar = 0;
    unsigned char x, y, n, m, c;
    unsigned char *next_script;
 
    m = 0;
 
    if (script == 0)
        return; 
 
    script_something_done = 0;
 
    while (1) {
        c = read_byte ();
        if (c == 0xFF) break;
        next_script = script + c;
        terminado = continuar = 0;
        while (!terminado) {
            c = read_byte ();
            switch (c) {
                case 0x30:
                    // IF ALL_ENEMIES_DEAD
                    // Opcode: 30
                    terminado = (pkilled != BADDIES_COUNT);
                    break;
                case 0xFF:
                    // THEN
                    // Opcode: FF
                    terminado = 1;
                    continuar = 1;
                    script_something_done = 1;
                    break;
            }
        }
        if (continuar) {
            terminado = 0;
            while (!terminado) {
                c = read_byte ();
                switch (c) {
                    case 0x20:
                        // SET TILE (x, y) = n
                        // Opcode: 20 x y n
                        x = read_vbyte ();
                        y = read_vbyte ();
                        n = read_vbyte ();
                        //clear_update_list ();
                        //update_index = 0;
                        map_set (x, y, n);
                        break;
                    case 0xFF:
                        terminado = 1;
                        break;
                }
            }
        }
        script = next_script;
    }
}

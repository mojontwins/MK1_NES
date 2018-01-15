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
                case 0x10:
                    // IF FLAG x = n
                    // Opcode: 10 x n
                    x = read_vbyte ();
                    n = read_vbyte ();
                    terminado = (flags [x] != n);
                    break;
                case 0x80:
                     // IF LEVEL = n
                     // Opcode: 80 n
                     n = read_vbyte ();
                     terminado = (n != level);
                     break;
                case 0xF0:
                     // IF TRUE
                     // Opcode: F0
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
                    case 0x01:
                        // SET FLAG x = n
                        // Opcode: 01 x n
                        x = read_vbyte ();
                        n = read_vbyte ();
                        flags [x] = n;
                        break;
                    case 0xE3:
                        x = 0;
                        gp_addr = 0x2000 + (LINE_OF_TEXT << 5) + LINE_OF_TEXT_X;
                        clear_update_list ();
                        update_index = 0;
                        while (1) {
                           n = read_byte ();
                           if (n == 0xEE) break;
                           update_list [update_index++] = MSB (gp_addr);
                           update_list [update_index++] = LSB (gp_addr ++);
                           update_list [update_index++] = n;
                           x ++;
                        }
                        ppu_waitnmi ();
                        clear_update_list ();
                        update_index = 0;
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

// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// ..\dev\assets\mscnes.h - Script pools and scripts interpreter
// generated by mscmk1 1.2 by the Mojon Twins

const unsigned char script_pool_0 [] = {
/*            0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F */
/* 0000 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0001 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0002 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0003 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0004 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0005 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0006 */ 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0007 */ 0x03, 0xf0, 0xff, 0xff, 0xff
};

const unsigned char * const script_pool [] = {
    script_pool_0
};

#ifdef CLEAR_FLAGS
void msc_clear_flags (void) {
    memfill (flags, 0, MAX_FLAGS);
}
#endif

unsigned char read_byte (void) {
    return *script ++;
}

unsigned char read_vbyte (void) {
    sc_c = *script ++;
    if (sc_c & 128) return flags [sc_c & 127];
    return sc_c;
}

void readxy (void) {
    sc_x = read_vbyte ();
    sc_y = read_vbyte ();
}

void run_script (unsigned char whichs) {
    // read address offset from index
    gp_gen = (unsigned char *) script_pool [level] + (whichs << 1);
    rda = *gp_gen ++; rdb = *gp_gen;
    script_result = 0;
    if (!(rda | rdb)) return;
    script = (unsigned char *) script_pool [level] + rda + (rdb << 8);

    while ((sc_c = read_byte ()) != 0xff) {
        next_script = script + sc_c;
        sc_terminado = sc_continuar = 0;
        while (!sc_terminado) {
            switch (read_byte ()) {
                case 0xf0: break;
                case 0xff: sc_terminado = sc_continuar = 1; break;
            }
        }

        if (sc_continuar) {
            fire_script_success = 1;
            sc_terminado = 0;
            while (!sc_terminado) {
                switch (read_byte ()) {
                    case 0xff: sc_terminado = 1; break;
                }
            }
        }
        script = next_script;
    }
}

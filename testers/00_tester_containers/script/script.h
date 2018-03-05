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
    gp_gen = (unsigned char *) script_pool + whichs + whichs;
    rda = *gp_gen ++; rdb = *gp_gen;
    script_result = 0;
    sc_continuar = 0;
    if (!(rda | rdb)) return;
    script = (unsigned char *) script_pool + rda + (rdb << 8);

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
            fire_script_success |= sc_continuar;
            sc_terminado = 0;
            while (!sc_terminado) {
                switch (read_byte ()) {
                    case 0x10: readxy (); flags [sc_x] += sc_y; break;
                    case 0x11: readxy (); flags [sc_x] -= sc_y; break;
                    case 0x14: readxy (); rda = flags [sc_x]; flags [sc_x] = flags [sc_y]; flags [sc_y] = rda; break;
                    case 0x15: sc_x = read_vbyte (); flags [sc_x] = 1 - flags [sc_x]; break;
                    case 0xff: sc_terminado = 1; break;
                }
            }
        }
        script = next_script;
    }
}

// script data and interpreter
// generated by msc3nes by the mojon twins

const unsigned char script_pool [] = {
/*            0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F */
/* 0000 */ 0x6c, 0x00, 0xd1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0001 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0002 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0003 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0004 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/* 0005 */ 0x00, 0x00, 0x00, 0x00, 0x27, 0x01, 0x3c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 
/* 0006 */ 0x7c, 0x01, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0xf0, 0xff, 0x20, 
/* 0007 */ 0x06, 0x04, 0x10, 0x20, 0x07, 0x04, 0x11, 0x20, 0x08, 0x04, 0x12, 0x20, 0x06, 0x05, 0x13, 0x20, 
/* 0008 */ 0x07, 0x05, 0x14, 0x20, 0x08, 0x05, 0x15, 0xe1, 0x51, 0x50, 0x20, 0x9f, 0x6f, 0xff, 0x1a, 0x10, 
/* 0009 */ 0x01, 0x01, 0xff, 0x20, 0x04, 0x05, 0x18, 0x20, 0x04, 0x03, 0x18, 0x20, 0x07, 0x02, 0x18, 0x20, 
/* 000A */ 0x0a, 0x03, 0x18, 0x20, 0x0a, 0x05, 0x18, 0xe1, 0xff, 0x26, 0x10, 0x01, 0x00, 0xff, 0xe3, 0x1e, 
/* 000B */ 0x53, 0x45, 0x54, 0x20, 0x42, 0x4f, 0x4d, 0x42, 0x53, 0x20, 0x49, 0x4e, 0x20, 0x43, 0x4f, 0x4d, 
/* 000C */ 0x50, 0x55, 0x54, 0x45, 0x52, 0x20, 0x41, 0x4e, 0x44, 0x20, 0x52, 0x55, 0x4e, 0x21, 0x00, 0xff, 
/* 000D */ 0xff, 0x54, 0x21, 0x50, 0x9f, 0x22, 0x20, 0x6f, 0x41, 0x05, 0x10, 0x01, 0x00, 0xff, 0x01, 0x01, 
/* 000E */ 0x01, 0x20, 0x04, 0x05, 0x18, 0xe1, 0xe0, 0x00, 0x20, 0x04, 0x03, 0x18, 0xe1, 0xe0, 0x00, 0x20, 
/* 000F */ 0x07, 0x02, 0x18, 0xe1, 0xe0, 0x00, 0x20, 0x0a, 0x03, 0x18, 0xe1, 0xe0, 0x00, 0x20, 0x0a, 0x05, 
/* 0010 */ 0x18, 0xe1, 0xe0, 0x00, 0xe3, 0x1e, 0x20, 0x20, 0x44, 0x4f, 0x4e, 0x45, 0x21, 0x20, 0x4e, 0x4f, 
/* 0011 */ 0x57, 0x20, 0x47, 0x4f, 0x20, 0x42, 0x41, 0x43, 0x4b, 0x20, 0x54, 0x4f, 0x20, 0x42, 0x41, 0x53, 
/* 0012 */ 0x45, 0x21, 0x20, 0x20, 0x00, 0xff, 0xff, 0x13, 0xf0, 0xff, 0x20, 0x01, 0x04, 0x16, 0x20, 0x02, 
/* 0013 */ 0x04, 0x17, 0x01, 0x02, 0x00, 0x51, 0x20, 0x20, 0x32, 0x5f, 0xff, 0xff, 0x36, 0x21, 0x20, 0x3f, 
/* 0014 */ 0x22, 0x21, 0x5f, 0x10, 0x02, 0x00, 0xff, 0x01, 0x02, 0x01, 0xe0, 0x09, 0xe3, 0x1e, 0x20, 0x48, 
/* 0015 */ 0x41, 0x4c, 0x46, 0x20, 0x4e, 0x45, 0x57, 0x20, 0x4d, 0x4f, 0x54, 0x4f, 0x52, 0x42, 0x49, 0x4b, 
/* 0016 */ 0x45, 0x20, 0x46, 0x4f, 0x52, 0x20, 0x53, 0x41, 0x4c, 0x45, 0x21, 0x20, 0x00, 0xe1, 0xe0, 0x00, 
/* 0017 */ 0xe0, 0x08, 0xff, 0xff, 0x06, 0x10, 0x01, 0x01, 0xff, 0xf1, 0xff, 0xff, 0x06, 0xf0, 0xff, 0x01, 
/* 0018 */ 0x01, 0x00, 0xff, 0xff, 0x26, 0x10, 0x01, 0x00, 0xff, 0xe3, 0x1e, 0x20, 0x53, 0x45, 0x54, 0x20, 
/* 0019 */ 0x35, 0x20, 0x42, 0x4f, 0x4d, 0x42, 0x53, 0x20, 0x49, 0x4e, 0x20, 0x45, 0x56, 0x49, 0x4c, 0x20, 
/* 001A */ 0x43, 0x4f, 0x4d, 0x50, 0x55, 0x54, 0x45, 0x52, 0x20, 0x00, 0xff, 0x26, 0x10, 0x01, 0x01, 0xff, 
/* 001B */ 0xe3, 0x1e, 0x42, 0x4f, 0x4d, 0x42, 0x53, 0x20, 0x41, 0x52, 0x45, 0x20, 0x53, 0x45, 0x54, 0x21, 
/* 001C */ 0x20, 0x52, 0x45, 0x54, 0x55, 0x52, 0x4e, 0x20, 0x54, 0x4f, 0x20, 0x42, 0x41, 0x53, 0x45, 0x21, 
/* 001D */ 0x00, 0xff, 0xff
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
	gp_gen = (unsigned char *) script_pool + whichs + whichs;
	rda = *gp_gen ++; rdb = *gp_gen;
	script_result = 0;
	sc_continuar = 0;
	if (!(rda | rdb)) return;
	script = (unsigned char *) script_pool + rda + (rdb << 8);

	// todo : update selected item flag

	while ((sc_c = read_byte ()) != 0xff) {
		next_script = script + sc_c;
		sc_terminado = sc_continuar = 0;
		while (!sc_terminado) {
			switch (read_byte ()) {
				case 0x10:
					// IF FLAG sc_x = sc_n
					// Opcode: 10 sc_x sc_n
					readxy ();
					sc_terminado = (flags [sc_x] != sc_y);
					break;
				case 0x21:
					// IF PLAYER_IN_X x1, x2
					// Opcode: 21 x1 x2
					sc_x = read_byte ();
					sc_y = read_byte ();
					sc_terminado = (!(prx >= sc_x && prx <= sc_y));
					break;
				case 0x22:
					// IF PLAYER_IN_Y y1, y2
					// Opcode: 22 y1 y2
					sc_x = read_byte ();
					sc_y = read_byte ();
					sc_terminado = (!(pry >= sc_x && pry <= sc_y));
					break;
				case 0x41:
					 // IF OBJECT_COUNT = sc_n
					 // Opcode: 41 sc_n
					 sc_terminado = (pobjs != read_vbyte ());
					 break;
				case 0xF0:
					// IF TRUE
					// Opcode: F0
					break;
				case 0xff:
					// then
					// opcode: ff
					sc_terminado = sc_continuar = 1;
					break;
			}
		}

		if (sc_continuar) {
            fire_script_success |= sc_continuar;
			sc_terminado = 0;
			while (!sc_terminado) {
				switch (read_byte ()) {
					case 0x01:
						// SET FLAG sc_x = sc_n
						// Opcode: 01 sc_x sc_n
						readxy ();
						flags [sc_x] = sc_y;
						break;
					case 0x20:
						// SET TILE (sc_x, sc_y) = sc_n
						// Opcode: 20 sc_x sc_y sc_n
						readxy ();
						_x = sc_x; _y = sc_y; _t = read_vbyte (); map_set ();
						break;
					case 0x51:
						// SET_FIRE_ZONE x1, y1, x2, y2
						// Opcode: 51 x1 y1 x2 y2
						fzx1 = read_byte ();
						fzy1 = read_byte ();
						fzx2 = read_byte ();
						fzy2 = read_byte ();
						f_zone_ac = 1;
						break;
					case 0xE0:
						// SOUND sc_n
						// Opcode: E0 sc_n
						sfx_play (read_vbyte (), 1);
						break;
					case 0xE1:
						// SHOW
						// Opcode: E1
						ppu_waitnmi (); clear_update_list ();
						break;
					case 0xE3:
                        // TEXT
                        // Opcode: E3 length characters* 0
                        _x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT; gp_gen = script;
						sc_n = read_byte (); script += (sc_n + 1);
                        pr_ul_str ();
						break;
					case 0xF1:
						// WIN
						script_result = 1;
						return;
					case 0xff:
						sc_terminado = 1;
						break;
				}
			}
		}
		script = next_script;
	}
}

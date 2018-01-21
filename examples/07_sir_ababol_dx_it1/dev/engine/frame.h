// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// frame.h
// Custom game frame

/*
const unsigned char game_frame [] = {
	 64,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  65,
	 69,   0,   3,  29,  16,  16,   0,   0,   0,   0,   0,   0,   0,  47,  34,  42,  29,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  29,  16,  16,   0,  71,
	 66,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  67
};
*/

void __fastcall__ draw_game_frame (void) {
	/*
	vram_write ((unsigned char *) game_frame, 0x2360, 96);
	for (rdit = 0; rdit < 16; rdit ++) attr_table [48 + rdit] = 0x00;
	*/

	pr_str (2, 3, "LIFE:00   ITEMS:00   KEYS:00");
	pr_str (2, 28, "SIR ABABOL - THE MOJON TWINS");
}

// Churrera for NES 1.0
// Copyleft Mojon Twins 2013

// frame.h
// Custom game frame

const unsigned char game_frame [] = {
	208, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 209,
	213,   0,   3,  29,  16,  16,   0,   0,   0,   0,   0,   0,   0,   0,   4,  29,  16,  16,   0,   0,   0,   0,   0,   0,   0,   0,   5,  29,  16,  16,   0, 215,
	210, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 211
};
/*
const unsigned char game_frame_attrs [] = {
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
};
*/
void __fastcall__ draw_game_frame (void) {
	vram_write ((unsigned char *) game_frame, 0x2360, 96);
	for (rdit = 0; rdit < 16; rdit ++) attr_table [48 + rdit] = 0xff;
}

// Churrera for NES 1.0
// Copyleft Mojon Twins 2013

// frame.h
// Custom game frame

const unsigned char game_frame [] = {
	  0,   0, 210, 211,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 218, 219,   0,   0,
	208, 209, 212, 213, 214,  16,  16, 215, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 217,  16,  16, 214, 220, 221, 222, 223
};
const unsigned char game_frame_attrs [] = {
	0xdd, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x66
};
void __fastcall__ draw_game_frame (void) {
	vram_write ((unsigned char *) game_frame, 0x2360, 64);
	for (rdit = 0; rdit < 8; rdit ++) attr_table [56 + rdit] = game_frame_attrs [rdit];
}

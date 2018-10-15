// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// game_frame.h
// Custom code to draw the frame around your HUD

void draw_game_frame (void) {
	unrle_vram (frame_rle, 0x2000);
	memfill (attr_table, 0xff, 64);
}

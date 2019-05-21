// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// game_frame.h
// Custom code to draw the frame around your HUD

void draw_game_frame (void) {
	unrle_vram (hud_rle, 0x2000);
	oam_spr (213, 30, 0, 0x20, 0);
}

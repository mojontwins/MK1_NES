// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// game_frame.h
// Custom code to draw the frame around your HUD

void draw_game_frame (void) {
	_x = 2; _y = 3;	 pr_str ("LIFE:00   ITEM:      KEYS:00");
	        _y = 28; pr_str ("INTERACTIVES-THE MOJON TWINS");
}

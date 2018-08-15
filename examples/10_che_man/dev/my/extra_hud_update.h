// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// game_frame.h
// Custom code to update the HUD. 

// Show gotten jewels on level 1
if (level == 1) {
	if (pjewels != opjewels) {
		_x = 7; _y = 4; _n = pjewels; p_t ();
		opjewels = pjewels;
	}
}
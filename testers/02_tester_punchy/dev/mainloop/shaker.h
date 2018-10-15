// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Simple screen shaker for a while

if (shaker_ct) {
	-- shaker_ct;
	scroll (254 + (rand8 () & 3), SCROLL_Y - 2 + (rand8 () & 3));
	if (shaker_ct == 0) scroll (0, SCROLL_Y);
}

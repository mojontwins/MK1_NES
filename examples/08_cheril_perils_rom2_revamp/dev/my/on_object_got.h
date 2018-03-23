// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An object has just been got.
// You may add checks and set variables and stuff.

// Player got object rda.

if (level == 1 && ht [0x12] == 9 && ht [0x16] == 10 && ht [0x0c] == 8) {
	// Make player know 
	gp_gen = text_open_gate;
	textbox_do ();
}

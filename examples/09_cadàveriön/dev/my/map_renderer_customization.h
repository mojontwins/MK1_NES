// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

// In this game we are not modifying much stuff, but "taking notes"
// to feed the scripting system.

flags [9] = 0; 	// To count # of statues

gpit = 192; while (gpit --) {
	rda = map_buff [gpit];
	rdb = rand8 () & 1;
	switch (rda) {
		case 0: if (rdb) map_buff [gpit] = 16; break;
		case 14:
			// Count statues
			flags [9] ++;
			break;
		case 15: 
			// Record gate coordinates:
			flags [6] = gpit & 0x0f;	// X
			flags [7] = gpit >> 4;		// Y
			break;
		case 25:
			// Record retry tile coordinates
			flags [4] = gpit & 0x0f;	// X
			flags [5] = gpit >> 4;		// Y
			break;
	}
}

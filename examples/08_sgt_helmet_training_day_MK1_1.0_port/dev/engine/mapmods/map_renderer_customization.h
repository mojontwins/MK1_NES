// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

// Modifications for Sir Ababol DX

if (level) set_rand (n_pant + 1); else set_rand (1);

for (gpit = 0; gpit < 192; gpit ++) {
	rdt = map_buff [gpit];

	// Precalc this random which will be used all around
	rda = ((rand8 () & 15) == 1);

	// Level 0, top row has many features
	if (level == 0 && n_pant < 20 && gpit < 112) {
		if (rdt == 0 && gpit == 68) { 
			rdt = 17;
		} else if (rdt == 0 && gpit >= 96) {
			rdt = 16;
		} else if (rda) {
			if (rdt == 0) rdt = 19;
			else if (rdt == 6) rdt = 18;
		}
	}

	// Level 1, random bubbles
	if (level == 1 && rdt == 10 && rda) rdt = 4;

	map_buff [gpit] = rdt;
}

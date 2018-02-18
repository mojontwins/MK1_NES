// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

if (level == 0) {
	gpit = 192; while (gpit --) {
		rda = map_buff [gpit]; 
		if (rand8 () & 1) {
			switch (rda) {
				case 0: 
				case 1:
					rda += 16; break;
				case 8: rda = 25; break;
			} 
		}
		if (rda == 21 && map_buff [gpit - 16] != 21) rda = 22;
		map_buff [gpit] = rda;
	}

	// Clear gate if all objects in place
	if (n_pant == 6) {
		if (ht [0x12] == 9 && ht [0x16] == 10 && ht [0x0c] == 8)
			map_buff [14+16*4] = 22;
			map_buff [14+16*5] = 22;
	}
}

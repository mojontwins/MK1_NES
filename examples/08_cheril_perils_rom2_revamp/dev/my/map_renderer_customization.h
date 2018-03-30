// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

set_rand (1 + n_pant);

switch (level) {
	case 0:
	case 1:
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

		// Clear gate if already talked to Amador
		if (level == 0 && n_pant == 5 && level0_gate) {
			map_buff [0x94] = 0;
			map_buff [0xA4] = 0;
		}

		// Clear gate if all objects in place
		if (level == 1 && n_pant == 6 && level1_gate) {
			map_buff [0x4E] = 22; 
			map_buff [0x5E] = 22;
		}
		
		break;

	case 2:
		rda = n_pant & 1;
		gp_gen = bgs [level] + (rda << 7) + (rda << 6);
		gpit = 192; while (gpit --) {
			rda = map_buff [gpit]; 
			if (rand8 () & 1) {
				switch (rda) {
					case 1:
					case 4:
					case 6:
					case 9:
					case 10:
					case 14:
						rda += 16; break;
				}
			}
			if (rda == 8 && map_buff [gpit - 16] != 8) rda = 24;
			if (rda == 0) rda = gp_gen [gpit];
			map_buff [gpit] = rda;
		}
		break;

	case 3:
		gp_gen = bgs [level];
		gpit = 129; while (gpit --) {
			if (map_buff [gpit] == 0) map_buff [gpit] = gp_gen [gpit];
		}
		break;

	case 4:
		gpit = 192; while (gpit --) {
			rda = map_buff [gpit]; 
			if ((rand8 () & 1) && (rda == 3 || rda == 10 || rda == 12)) {				
				rda += 16;
			}
			if (rda == 15 && map_buff [gpit - 1] == 15) rda = 31;
			if (rda == 14 && map_buff [gpit - 16] != 14) rda = 30;
			map_buff [gpit] = rda;
		}
		break;
}

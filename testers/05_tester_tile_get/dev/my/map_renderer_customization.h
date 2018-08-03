// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

set_rand (n_pant + 1);

switch (level) {
	case 3:
	case 4:
	case 5:
		// Strip of water at the bottom of the level
		if (level != 3 || n_pant > 14) {
			for (gpit = 176; gpit < 192; gpit ++) map_buff [gpit] = 18;
		}
	case 0:
	case 1:
	case 2:
		// Random clouds, find a nice spot but don't try hard
		gpit = 4; while (gpit --) {
			gpjt = 4; while (gpjt --) {
				rdx = rand8 () & 0xf;
				rdy = rand8 () & 0xf;
				rda = (rdy << 4) | rdx;
				if (map_buff [rda] == 0 && map_buff [rda + 1] == 0 && rdy < 12) {
					map_buff [rda] = 16;
					map_buff [rda + 1] = 17;
					break;
				}
			}
		}
		break;
	case 6:
	case 7:
	case 8:
		// Bosque de Badajoz Zone embellishments
		for (gpit = 0; gpit < 192; gpit ++) {
			rdt = map_buff [gpit];

			if (gpit > 16) {
				rda = map_buff [gpit - 16];
				if (rdt == 8) {
					if (rda == 4) rdt = 16;
					else if (rda == 6) rdt = 17;
				} else if (rdt == 10 && rda == 13) rdt = 20;
				else if (rdt == 2 && rda != 2 && rda != 19) rdt = 3;
			}
			if (gpit < 176) {
				rda = map_buff [gpit + 16];
				if (rdt == 0 && rda == 2) rdt = 19;
				else if (rdt == 2 && rda != 2) rdt = 18;
			}
		
			if (rdt == 0 && (rand8 () & 7) == 1) rdt = 21;

			map_buff [gpit] = rdt;
		}
		break;
}
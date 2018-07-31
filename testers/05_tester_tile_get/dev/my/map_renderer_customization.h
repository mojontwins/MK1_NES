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
		if (level != 3 || n_pant > 14) {
			for (gpit = 176; gpit < 192; gpit ++) map_buff [gpit] = 18;
		}
	case 0:
	case 1:
	case 2:
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
}
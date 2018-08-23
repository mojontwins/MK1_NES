// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

set_rand (n_pant + 1);

switch (level_world) {
	case 1:
		// Strip of water at the bottom of the level
		if (level != 3 || n_pant > 14) {
			for (gpit = 176; gpit < 192; gpit ++) map_buff [gpit] = 18;
		}
	case 0:
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
	case 4:
		rdc = (map_buff [0] != 4);	// Cheap way to distinguish between outside / inside
	case 2:
	case 3:
			
		for (gpit = 0; gpit < 192; gpit ++) {
			rdt = map_buff [gpit];
			rdb = rand8 ();

			if (level_world == 2) {
				// Bosque de Badajoz Zone embellishments

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
			} else {				
				if (level_world == 3) {
					// Wet Ruins Zone embellishments
					if (rdt == 1) rdt = 16 + (rdb & 3);
				} else {
					// Crap Brains embellishments
					if (rdt == 1 || rdt == 3) {
						rda = map_buff [gpit - 1];
						if (rda != rdt && rda != rdt + 16) rdt = rdt + 16;
						else {
							rda = map_buff [gpit + 1];
							if (rda != rdt) rdt = rdt + 17;
						}
					}

					if (level_act == 1) {
						if (rdt == 2 && toggle_switch == 0) rdt = 0;
						if (rdt == 4 && toggle_switch) rdt = 0;
					}
				}

				// Wet Ruins Zone & Crap Brains embellishments
				switch (rdt) {					
					case 8:
					case 12:
						rdt = 16 + rdt + (gpit & 1) + ((gpit >> 3) & 2);
						break;
				}
			}
		
			if (rdt == 0 && (rdb & 7) == 1 && (rdc || level_world < 4)) rdt = 21;

			map_buff [gpit] = rdt;
		}
		break;
	

}
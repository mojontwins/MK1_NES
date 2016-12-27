			// Resonators code.
			if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) {
				// Render resonator, OAM byte 64
				oam_spr (hrx + 4, hry + 14, 224 + res_ct, 2, 156);
			} else {
				oam_spr (0, 240, 0, 0, 156);
			}
			
			if (res_on) {
				// Resonator logic

				if (res_subct) {
					res_subct --;
				} else {
					res_subct = 50;
					if (res_ct) {
						res_ct --;
					} else {
						res_on = 0;
						pal_bg (c_pal_bg);
						if (hrt == HOTSPOT_TYPE_RESONATOR) update_list_tile (hrx >> 3, TOP_ADJUST + (hry >> 3), 19);
					}
				}
			}

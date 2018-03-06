// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Containers definition

void containers_paint (void) {
	gpit = containers_index; while (gpit --) {
		rda = containers_f [gpit];
		oam_index = oam_meta_spr (
			containers_yx [gpit] << 4, (containers_yx [gpit] & 0xf0) + SPRITE_ADJUST,
			oam_index,
			spr_hs [rda & 0x80 ? flags [rda & 0x7f] : rda]
		);
	}
}

#ifdef CONTAINERS_FROM_CODE
	void containers_create (void) {
		gp_gen = c_containers;
		containers_index = 0;
		while (rda = *gp_gen ++) {
			if (rda == n_pant) {
				containers_yx [containers_index] = *gp_gen ++;
				containers_f [containers_index ++] = *gp_gen ++;
			} else gp_gen += 2;
		}
	}
#else
	void containers_add (void) {
		containers_yx [containers_index] = sc_x;
		containers_f [containers_index ++] = sc_y;
	}
#endif

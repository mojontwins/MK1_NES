// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives definition

void interactives_paint (void) {
	gpit = interactives_index; while (gpit --) {
		rda = interactives_f [gpit];
		oam_index = oam_meta_spr (
			interactives_yx [gpit] << 4, (interactives_yx [gpit] & 0xf0) + SPRITE_ADJUST,
			oam_index,
			spr_hs [rda & 0x80 ? flags [rda & 0x7f] : rda]
		);
	}
}

#ifdef INTERACTIVES_FROM_CODE
	void interactives_create (void) {
		gp_gen = c_interactives; if (!gp_gen) return;
		interactives_index = 0;
		while ((rda = *gp_gen ++) != 0xff) {
			if (rda == n_pant) {
				interactives_yx [interactives_index] = 16 + *gp_gen ++;
				interactives_f [interactives_index ++] = *gp_gen ++;
			} else gp_gen += 2;
		}
	}
#else
	void interactives_add (void) {
		interactives_yx [interactives_index] = sc_x;
		interactives_f [interactives_index ++] = sc_y;
	}
#endif

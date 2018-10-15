// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives definition

void interactives_paint (void) {
	gpit = interactives_index; while (gpit --) {
		rda = interactives_f [gpit];
		#ifdef DOUBLE_WIDTH
			rdaa = interactives_x [gpit] << 4;
			if (rdaa >= scroll_x && rdaa <= scroll_x + 240)
				oam_index = oam_meta_spr (
					rdaa - scroll_x, interactives_y [gpit] + SPRITE_ADJUST,
					oam_index,
					spr_hs [rda & 0x80 ? flags [rda & 0x7f] : rda]
				);
		#else
			oam_index = oam_meta_spr (
				interactives_yx [gpit] << 4, (interactives_yx [gpit] & 0xf0) + SPRITE_ADJUST,
				oam_index,
				spr_hs [rda & 0x80 ? flags [rda & 0x7f] : rda]
			);
		#endif
	}
}

#ifdef INTERACTIVES_FROM_CODE
	void interactives_create (void) {
		gp_gen = c_interactives; if (!gp_gen) return;
		interactives_index = 0;
		while ((rda = *gp_gen ++) != 0xff) {
			if (rda == n_pant) {
				#ifdef DOUBLE_WIDTH
					interactives_y [interactives_index] = (1 + *gp_gen ++) << 4;
					interactives_x [interactives_index] = *gp_gen ++;
				#else
					interactives_yx [interactives_index] = 16 + *gp_gen ++;
				#endif
				interactives_f [interactives_index ++] = *gp_gen ++;
			} else 
			#ifdef DOUBLE_WIDTH
				gp_gen += 3;
			#else
				gp_gen += 2;
			#endif
		}
	}
#else
	void interactives_add (void) {
		#ifdef DOUBLE_WIDTH
			interactives_x [interactives_index] = sc_x;
			interactives_y [interactives_index] = (sc_y << 4);
			interactives_f [interactives_index ++] = sc_n;
		#else
			interactives_yx [interactives_index] = sc_x;
			interactives_f [interactives_index ++] = sc_y;
		#endif
	}
#endif

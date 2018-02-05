// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Map renderer fast (original)

void draw_map_tile (unsigned char t) {
	map_buff [rdm] = t;		
	map_attr [rdm] = c_behs [t];
	#ifdef ENABLE_BREAKABLE
		brk_buff [rdm] = 1;
	#endif
	rdm ++;
	draw_tile (rdx + rdx, rdy + rdy + TOP_ADJUST, t);
	rdx = (rdx + 1) & 15; if (!rdx) rdy ++;
}

void draw_scr (void) {

	// Draw Map

	rdx = rdy = rdm = 0;

	#ifdef MAP_FORMAT_PACKED
		// Get pointer
		gp_gen = c_map + (n_pant << 6) + (n_pant << 5); 
		
		// Draw packed
		rdit = 96; while (rdit --) {
			rdt = *gp_gen ++;
			draw_map_tile (rdt >> 4);
			draw_map_tile (rdt & 15);
		}
	#endif

	#ifdef MAP_FORMAT_RLE16
		// Get pointer
		gp_gen = c_map [n_pant];

		// Packed or RLE'd?
		if (rdit = *gp_gen ++) {
			while (rdit) {
				rdct = get_byte ();
				if (rdct & 0xf0) {
					// String
					rdct = 1 + (rdct & 0x0f);
					while (rdct --) {
						rdt = get_byte ();
						draw_map_tile (rdt >> 4);
						if (rdy < 12) draw_map_tile (rdt & 15);
					}
				} else {
					// Counter
					rdt = rdct & 0x0f;
					rdct = get_byte ();
					while (rdct --) draw_map_tile (rdt);
				}
			}
		} else {
			rdit = 96; while (rdit --) {
				rdt = *gp_gen ++;
				draw_map_tile (rdt >> 4);
				draw_map_tile (rdt & 15);
			}
		}
	#endif	

	// Draw decorations

	#ifdef MAP_WITH_DECORATIONS
		if (c_decos) {
			if (c_decos [n_pant]) {
				gp_gen = c_decos [n_pant];
			
				while (rdt = *gp_gen ++) {
					if (rdt & 0x80) {
						rdt &= 0x7F;
						rdct = 1;
					} else {
						rdct = *gp_gen ++;
					}
					while (rdct --) {
						rdm = *gp_gen ++;
						rdx = rdm >> 4; rdy = rdm & 15;
						draw_map_tile (rdt);
					}
				}
			}
		}
	#endif

	// Clear open locks

	#ifndef DEACTIVATE_KEYS	
		gp_gen = c_locks;
		gpit = c_max_bolts; while (gpit --) {
			rda = *gp_gen ++; rdm = *gp_gen ++;
			if (n_pant == rda) {
				if (!lkact [gpit]) {
					rdy = (rdm >> 4); rdx = (rdm & 15);
					draw_map_tile (0);
				}
			}
		}	
	#endif

	vram_write (attr_table, 0x23c0, 64);
}

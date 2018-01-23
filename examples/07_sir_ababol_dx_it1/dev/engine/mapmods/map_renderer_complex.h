// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Map renderer complex:

// 1. render everything to map_buff.
// 2. paint from map_buff

// Between 1 and 2 you can stuff whatever modifications you like.

void add_tile (void) {
	map_buff [rdm] = rda;
	rdm ++;
}

void draw_scr (void) {

	// Draw Map

	rdm = 0;

	#ifdef MAP_FORMAT_PACKED
		// Get pointer
		gp_gen = c_map + (n_pant << 6) + (n_pant << 5); 
		
		// Draw packed
		rdit = 96; while (rdit --) {
			rdt = *gp_gen ++;
			rda = rdt >> 4; add_tile ();
			rda = rdt & 15; add_tile ();
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
						rda = rdt >> 4; add_tile ();
						rda = rdt & 15;
						if (rdy < 12) add_tile ();
					}
				} else {
					// Counter
					rda = rdct & 0x0f;
					rdct = get_byte ();
					while (rdct --) add_tile ();
				}
			}
		} else {
			rdit = 96; while (rdit --) {
				rdt = *gp_gen ++;
				rda = rdt >> 4; add_tile ();
				rda = rdt & 15; add_tile ();
			}
		}
	#endif

	// Edit this file to alter map_buff the way you need:
	#include "engine/mapmods/map_renderer_customization.h"

	// Draw decorations

	#ifdef MAP_WITH_DECORATIONS
		// Draw decorations
		if (c_decos) {
			if (c_decos [n_pant]) {
				gp_gen = c_decos [n_pant];
			
				while (rda = *gp_gen ++) {
					if (rda & 0x80) {
						rda &= 0x7F;
						rdct = 1;
					} else {
						rdct = *gp_gen ++;
					}
					while (rdct --) {
						rdm = *gp_gen ++;
						add_tile ();
					}
				}
			}
		}
	#endif

	// Clear open locks

	#ifndef DEACTIVATE_KEYS	
		gp_gen = c_locks; rda = 0;
		gpit = c_max_bolts; while (gpit --) {
			rdb = *gp_gen ++; rdm = *gp_gen ++;
			if (n_pant == rdb) {
				if (!lkact [gpit]) add_tile ();
			}
		}	
	#endif

	// Now blit the buffer

	rdx = 0; rdy = 0; gp_ram = map_buff;
	for (rdm = 0; rdm < 192; rdm ++) {
		rdt = *gp_ram ++;
		map_attr [rdm] = c_behs [rdt];

		#ifdef BREAKABLE_WALLS
			brk_buff [rdm] = 1;
		#endif

		draw_tile (rdx + rdx, rdy + rdy + TOP_ADJUST, rdt);
		rdx = (rdx + 1) & 15; if (!rdx) rdy ++;
	}
}

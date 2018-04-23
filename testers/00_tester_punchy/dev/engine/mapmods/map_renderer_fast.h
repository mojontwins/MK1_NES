// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Map renderer fast (original)

void draw_map_tile (void) {
	map_buff [rdm] = rdt;		
	map_attr [rdm] = c_behs [rdt];
	#if defined (ENABLE_BREAKABLE) && !defined (BREAKABLES_SOFT)
		brk_buff [rdm] = 1;
	#endif

	#include "engine/mapmods/map_detectors.h"

	rdm ++;
	_x = rdx << 1; _y = (rdy << 1) + TOP_ADJUST; _t = rdt;
	draw_tile ();
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
			rdd = *gp_gen ++;
			rdt = rdd >> 4; draw_map_tile ();
			rdt = rdd & 15; draw_map_tile ();
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
						rdd = get_byte ();
						rdt = rdd >> 4; draw_map_tile ();
						if (rdy < 12) { rdt = rdd & 15; draw_map_tile (); }
					}
				} else {
					// Counter
					rdt = rdct & 0x0f;
					rdct = get_byte ();
					while (rdct --) draw_map_tile ();
				}
			}
		} else {
			rdit = 96; while (rdit --) {
				rdd = *gp_gen ++;
				rdt = rdd >> 4; draw_map_tile ();
				rdt = rdd & 15; draw_map_tile ();
			}
		}
	#endif

	#ifdef MAP_FORMAT_RLE53
		// Get pointer
		gp_gen = c_map [n_pant];

		while (rdm < 192) {
			rdd = *gp_gen ++;
			rdt = rdd & 0x1f;
			/*
			rdct = 1 + (rdd >> 5);
			while (rdct --) draw_map_tile (); 
			*/
			rdct = rdd;
			while (rdct >= 32) {
				draw_map_tile (); 
				rdct -= 32;
			} draw_map_tile (); 
		}
	#endif	

	// Draw decorations

	#ifdef MAP_WITH_DECORATIONS
		if (c_decos) {
			if (c_decos [n_pant]) {
				gp_gen = c_decos [n_pant];
			
				while (rdd = *gp_gen ++) {
					if (rdd & 0x80) {
						rdd &= 0x7F;
						rdct = 1;
					} else {
						rdct = *gp_gen ++;
					}
					while (rdct --) {
						rdm = *gp_gen ++;
						rdx = rdm >> 4; rdy = rdm & 15;
						rdt = rdd; draw_map_tile ();
					}
				}
			}
		}
	#endif

	// Clear open locks

	#ifndef DEACTIVATE_KEYS	
		gp_gen = c_locks;
		rdt = 0;
		gpit = c_max_bolts; while (gpit --) {
			rda = *gp_gen ++; rdm = *gp_gen ++;
			if (n_pant == rda) {
				if (!lkact [gpit]) {
					rdy = (rdm >> 4); rdx = (rdm & 15);
					draw_map_tile ();
				}
			}
		}	
	#endif

	vram_write (attr_table, 0x23c0, 64);
}

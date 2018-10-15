// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Map renderer complex:

// 1. render everything to map_buff.
// 2. paint from map_buff

// Between 1 and 2 you can stuff whatever modifications you like.

void add_tile (void) {
	#ifdef DOUBLE_WIDTH
		buff_ptr [rdm] = rda;
	#else
		map_buff [rdm] = rda;
	#endif
	++ rdm;
}

#ifdef DOUBLE_WIDTH
void draw_half_scr (void)
#else
void draw_scr (void)
#endif
{
	#ifdef DOUBLE_WIDTH
		buff_ptr = map_buff + buff_offset;
		attr_ptr = map_attr + buff_offset;
	#endif

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
		rdx = rdy = 0;

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
						if (rdy < 12) { rda = rdt & 15;	 add_tile (); }
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

	#ifdef MAP_FORMAT_RLE53
		// Get pointer
		gp_gen = c_map [n_pant];

		while (rdm < 192) {
			// rdt = *gp_gen ++;
			SET_FROM_PTR (rdt, gp_gen); 
			gp_gen ++;
			rda = rdt & 0x1f;
			
			rdct = rdt;
			while (rdct >= 32) {
				add_tile (); rdct -= 32;
			} add_tile ();
		}
	#endif

	#ifdef MAP_FORMAT_RLE44
		// Get pointer
		gp_gen = c_map [n_pant];

		while (rdm < 192) {
			// rdt = *gp_gen ++;
			SET_FROM_PTR (rdt, gp_gen); 
			gp_gen ++;
			rda = rdt & 0x0f;
			
			rdct = rdt;
			while (rdct >= 16) {
				add_tile (); rdct -= 16;
			} add_tile ();
		}
	#endif

	#ifdef MAP_FORMAT_RLE53_CHRROM
		bankswitch (c_map_chr_rom_bank);
		vram_adr (c_map [n_pant]);
		rda = VRAM_READ; 	// Dummy read.
		
		// UNRLE into scr_buff
		while (rdm < 192) {
			rdt = VRAM_READ;
			rda = rdt & 0x1f;
			
			rdct = rdt;
			while (rdct >= 32) {
				add_tile (); rdct -= 32;
			} add_tile ();
		}
	#endif

	#ifdef MAP_FORMAT_RLE44_CHRROM
		bankswitch (c_map_chr_rom_bank);
		vram_adr (c_map [n_pant]);
		rdt = VRAM_READ; 	// Dummy read.
		
		// UNRLE into scr_buff
		while (rdm < 192) {
			rdt = VRAM_READ;
			rda = rdt & 0x0f;

			rdct = rdt;
			while (rdct >= 16) {
				add_tile (); rdct -= 16;
			} add_tile ();
		}
	#endif

	// Edit this file to alter map_buff the way you need:
	#include "my/map_renderer_customization.h"

	// Draw decorations

	#ifdef MAP_WITH_DECORATIONS
		// Draw decorations
		if (c_decos) {
			#if defined (MAP_FORMAT_PACKED) || defined (MAP_FORMAT_RLE16)
				if (c_decos [n_pant]) {
					gp_gen = c_decos [n_pant];
			# else 
				{
			#endif

				#if defined (MAP_FORMAT_RLE44_CHRROM) || defined (MAP_FORMAT_RLE53)
					while (rda = VRAM_READ) {
						if (rda & 0x80) {
							rda &= 0x7F;
							rdct = 1;
						} else {
							rdct = VRAM_READ;
						}
						while (rdct --) {
							rdm = VRAM_READ;
							add_tile ();
						}
					}
				#else
					while (rda = *gp_gen ++) {
						if (rda & 0x80) {
							rda &= 0x7F;
							rdct = 1;
						} else {
							// rdct = *gp_gen ++;
							SET_FROM_PTR (rdct, gp_gen); 
							gp_gen ++;
						}
						while (rdct --) {
							// rdm = *gp_gen ++;
							SET_FROM_PTR (rdm, gp_gen); 
							gp_gen ++;
							add_tile ();
						}
					}
				#endif
			}
		}
	#endif

	// Clear open locks

	#ifndef DEACTIVATE_KEYS	
		gp_gen = c_locks; rda = 0;
		gpit = c_max_bolts; while (gpit --) {
			// rdb = *gp_gen ++;
			SET_FROM_PTR (rdb, gp_gen); gp_gen ++;
			// rdm = *gp_gen ++;
			SET_FROM_PTR (rdm, gp_gen); gp_gen ++;
			if (n_pant == rdb) {
				if (!lkact [gpit]) add_tile ();
			}
		}	
	#endif

	// Now blit the buffer

	#if defined (ENABLE_TILE_GET) && defined (PERSISTENT_TILE_GET)
		rdd = 0;
	#endif

	_x = 0; _y = TOP_ADJUST; 
	#ifdef DOUBLE_WIDTH
		gp_ram = buff_ptr;
	#else
		gp_ram = map_buff;
	#endif
	for (rdm = 0; rdm < 192; rdm ++) {
		SET_FROM_PTR (rdt, gp_ram); gp_ram ++;

		#if defined (ENABLE_TILE_GET) && defined (PERSISTENT_TILE_GET)			
			if (tile_got [rdd] & bits [rdm & 7]) rdt = 0;
			if ((rdm & 7) == 7) ++ rdd;
		#endif

		#ifdef DOUBLE_WIDTH
			attr_ptr [rdm] = c_behs [rdt];
		#else
			map_attr [rdm] = c_behs [rdt];
		#endif

		#if defined (ENABLE_BREAKABLE) && !defined (BREAKABLES_SOFT)
			brk_buff [rdm] = 1;
		#endif

		#include "engine/mapmods/map_detectors.h"

		_t = rdt; draw_tile ();
		_x = (_x + 2) & 0x1f; if (!_x) _y += 2;
	}

	#if defined (ENABLE_TILE_CHAC_CHAC) && defined (CHAC_CHACS_CLEAR)
		gpit = max_chac_chacs; while (gpit --) {
			_t = CHAC_CHAC_BASE_TILE + 6;
			_x = (chac_chacs_yx [gpit] & 0xf) << 1;
			_y = ((chac_chacs_yx [gpit] & 0xf0) >> 3) + TOP_ADJUST;
			draw_tile ();
			_y += 2;
			draw_tile ();
			_y += 2;
			draw_tile ();
		}
	#endif

	#ifdef DOUBLE_WIDTH
		vram_write (attr_table + attr_table_offset, NAMETABLE_BASE + 0x3c0, 64);
	#else
		vram_write (attr_table, 0x23c0, 64);
	#endif
}

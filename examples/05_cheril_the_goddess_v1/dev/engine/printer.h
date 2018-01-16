// NES MK1 v0.5
// Copyleft Mojon Twins 2013, 2015

// printer.h
// Draw map, print text, etcetera.

// fade out
void  fade_out (void) {
	for (fader = 4; fader > -1; fader --) {
		pal_bright (fader);
		delay (fade_delay);
	}	
}

// fade in
void  fade_in (void) {
	for (fader = 0; fader < 5; fader ++) {
		pal_bright (fader);
		delay (fade_delay);
	}	
}
/*
void  fade_in_fast (void) {
	pal_bright (2); delay (1);
	pal_bright (4); delay (1);
}

void  fade_out_fast (void) {
	pal_bright (2); delay (1);
	pal_bright (0); delay (1);
}
*/
// Clear update list
void clear_update_list (void) {
	for (i = 0; i < UPDATE_LIST_SIZE * 3; i ++)
		update_list [i] = 0;	
	update_index = 0;
}

void cls (void) {
	vram_adr(0x2000); vram_fill(0x00,0x400);
}

void ul_putc (unsigned char n) {
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = n;
}

void p_t (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = (y << 5) + x + 0x2000;
/*
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = (n / 10) + 16;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr);
	update_list [update_index++] = (n % 10) + 16;
*/
	ul_putc ((n/10)+16);
	ul_putc ((n%10)+16);
}

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};
unsigned char attr_table [64];

void upd_attr_table (unsigned char x, unsigned char y, unsigned char tl) {
	rdc = (x >> 2) + ((y >> 2) << 3);
	rdb = ((x >> 1) & 1) + (((y >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (tileset_pals [tl] << (rdb << 1));
	attr_table [rdc] = rda;
}

void draw_tile (unsigned char x, unsigned char y, unsigned char tl) {
	upd_attr_table (x, y, tl);
	
	gp_tmap = tsmap + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap++);
	gp_addr += 31;
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap);	
}

/*
void wbtul (unsigned char b) {
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr++);
	update_list [update_index++] = b;
}
*/

void update_list_tile (unsigned char x, unsigned char y, unsigned char tl) {
	upd_attr_table (x, y, tl);
	
	gp_addr = 0x23c0 + rdc;
	/*
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr);
	update_list [update_index++] = rda;
	*/
	ul_putc (rda);
	
	// tiles
	//tl = (16 + tl) << 2;
	gp_tmap = tsmap + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	/*
	wbtul (*gp_tmap++);
	wbtul (*gp_tmap++);
	*/
	ul_putc (*gp_tmap ++);
	ul_putc (*gp_tmap ++);
	gp_addr += 30;
	/*
	wbtul (*gp_tmap++);
	wbtul (*gp_tmap);
	*/
	ul_putc (*gp_tmap ++);
	ul_putc (*gp_tmap);
}

void map_set (unsigned char x, unsigned char y, unsigned char n) {
	map_buff [x + (y << 4)] = n;
	map_attr [x + (y << 4)] = tbehs [n];
	update_list_tile (x + x, TOP_ADJUST + y + y, n); 
}

// CUSTOM {
void draw_gate (unsigned char x, unsigned char y, unsigned char s) {
	switch (s) {
		case 0:
			map_set (x, y, 29); map_set (x, y + 1, 30); 
			break;
		case 1:
			map_set (x, y, 0); map_set (x, y + 1, 29); 
			break;
		case 2:
			map_set (x, y, 0); map_set (x, y + 1, 0); 
			break;
	}
}
// } END_OF_CUSTOM

void draw_map_tile (t) {
	rda = rdx + (rdy << 4);
	map_buff [rda] = t;		
	map_attr [rda] = tbehs [t];
	if (t == 0) {
		rda = rand8 ();
		if (level) {
			t = (rda & 31) == 1 ? t = 45 + (rand8 () & 1) : 44;
		} else {
			if ((rda & 15) < 7) t = 40 + ((n_pant + rdx + rdy + rdy) & 3);
		}
	} else if (t == 10) {
		if (map_buff [rda - 16] != 10) t = 16;
	}
	draw_tile (rdx + rdx, rdy + rdy + TOP_ADJUST, t);
	rdx = (rdx + 1) & 15; if (!rdx) rdy ++;
}

unsigned char get_byte (void) {
	rdit --;
	return *gp_gen ++;
}

void draw_scr (void) {

	// What
	level = (n_pant == 0 || n_pant == 60 || 
		(n_pant >= 57 && n_pant <= 59) ||
		(n_pant >= 76 && n_pant <= 79) );

	// Draw current screen
	set_rand (n_pant + 1);
	rdx = rdy = 0;

	// Get pointer
	gp_gen = (unsigned char *) c_map [n_pant];

	// Full or RLE'd?
	
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

	// Draw decorations
	if (c_decos [n_pant]) {
		gp_gen = (unsigned char *) c_decos [n_pant];
	
		while (rdt = *gp_gen ++) {
			if (rdt & 0x80) {
				rdt &= 0x7F;
				rdct = 1;
			} else {
				rdct = *gp_gen ++;
			}
			while (rdct --) {
				rda = *gp_gen ++;
				rdx = rda >> 4; rdy = rda & 15;
				draw_map_tile (rdt);
			}
		}
	}

	// Clear open locks
#ifndef DEACTIVATE_KEYS	
	for (gpit = 0; gpit < MAX_CERROJOS; gpit ++) {
		if (n_pant == lknp [gpit]) {
			if (!lkact [gpit]) {
				rdx = (lkxy [gpit] >> 4);
				rdy = (lkxy [gpit] & 15);
				draw_map_tile (rdct);
			}
		}
	}	
#endif

#ifdef BREAKABLE_ANIM
	do_process_breakable = 0;
	for (gpit = 0; gpit < MAX_BREAKABLE; gpit ++) brkf [gpit] = 0;
#endif

// CUSTOM {
	// Draw gates
	if (n_pant == 19) {
		if (hact [61]) draw_gate (8, 3, 0);
		if (hact [77]) draw_gate (10, 3, 0);
		if (hact [0]) draw_gate (12, 3, 0);
	}
	if (n_pant == 17) {
		if (hact [17]) map_set (7, 8, 28);
	}
// } END_OF_CUSTOM
}

void pr_str (unsigned char x, unsigned char y, unsigned char *s) {
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr);
	while (gpit = *s++) {
		//vram_adr (gp_addr++);
		if (gpit != '_') vram_put (gpit - 32); else vram_put (0);
	}
}
/*
void pr_str_upd (unsigned char *s) {
	gp_addr = 0x2000 + (LINE_OF_TEXT << 5) + LINE_OF_TEXT_X;
	while (1) {
		if ( (gpit = *s ++) == 0x0) break;
		update_list [update_index++] = MSB (gp_addr);
		update_list [update_index++] = LSB (gp_addr ++);
		update_list [update_index++] = gpit - 32;
	}
}
*/
// CUSTOM {

unsigned char pal_cycle [3];
void palfx_init () {
	for (gpit = 0; gpit < 3; gpit ++) pal_cycle [gpit] = c_pal_bg [13 + gpit];
}
void palfx_do (void) {
	if ((frame_counter & 15) == 0) {
		gpit = pal_cycle [0]; 
		pal_cycle [0] = pal_cycle [1]; 
		pal_cycle [1] = pal_cycle [2]; 
		pal_cycle [2] = gpit; 
	}
	for (gpit = 0; gpit < 3; gpit ++) pal_col (13 + gpit, pal_cycle [gpit]);
}

void fx_flash (void) {
	pal_bg (mypal_light_bg);
	pal_spr (mypal_light_fg);
	ppu_waitnmi ();
	ppu_waitnmi ();
	pal_bg (c_pal_bg);
	pal_spr (c_pal_fg);
}

void sleep_cycles (unsigned char s) {
	gpjt = s; while (--gpjt) ppu_waitnmi ();
}

void batout (void) {
	fade_out ();
	ppu_off ();
	oam_hide_rest (0);
	clear_update_list ();
}

void batin (void) {
	while (pad_poll (0));
	fade_delay = 4;
	ppu_on_all ();
	scroll (0, 0);
	fade_in ();
}

void prepare_scr (void);
void short_cutscene (unsigned char s) {
	if (s != 99) { if (s == 0) s = 2; else s --; }
	on_pant = n_pant;
	n_pant = 19;

	batout ();
	draw_scr ();
	vram_write (attr_table, 0x23c0, 64);
	batin ();

	sleep_cycles (50);

	if (s != 99) {
		for (gpit = 0; gpit < 3; gpit ++) {
			clear_update_list ();
			draw_gate (8 + s + s, 3, gpit);
			sfx_play (6, 2);
			fx_flash ();
			sleep_cycles (25);
		}
	} else sleep_cycles (50);

	sleep_cycles (50);

	n_pant = on_pant;
	//on_pant = 99;		// Force reenter
	clear_update_list ();
	prepare_scr ();
}

void draw_game_frame ();
void show_minimap () {
	batout ();
	cls ();
	pal_bg (mypal_cuts);
	gpit = 0;
	for (rdy = 0; rdy < 4; rdy ++) {
		vram_adr (0x2006 + ((rdy + 13) << 5));
		for (rdx = 0; rdx < 20; rdx ++) {
			vram_put (gpit == n_pant ? 63 : hact [gpit] ? 32 + c_hotspots [gpit << 1] : 32); 
			gpit ++; 
		}
	}
	batin ();
	while (pad_poll (0));
	while (!pad_poll (0));
	on_pant = 99;
	ft = 1;
	batout ();
	draw_game_frame ();
	pal_bg (c_pal_bg);
}

// } END_OF_CUSTOM
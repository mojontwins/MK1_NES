// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// printer.h
// Draw map, print text, etcetera.

// Clear update list
void clear_update_list (void) {
	for (i = 0; i < UPDATE_LIST_SIZE * 3; i ++)
		update_list [i] = 0;	
}

#ifdef DEBUG
unsigned char get_hex_digit (unsigned char n) {
	if (n < 10) return n + 16;
	return n + 23;
}

void debug_print_hex_16_dl (unsigned char x, unsigned char y, unsigned int n) {
	clear_update_list ();
	update_index = 0;

	gp_addr = (y << 5) + x + 0x2000;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = get_hex_digit (n >> 12);

	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = get_hex_digit ((n >> 8) & 0xf);

	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = get_hex_digit ((n >> 4) & 0xf);

	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = get_hex_digit ((n & 0xf));

	ppu_waitnmi ();
}
#endif

// fade out
void fade_out (void) {
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

void cls (void) {
	vram_adr(0x2000);
	vram_fill(255,0x3c0);
	vram_adr (0x23c0);
	vram_fill(0x00,64);
}

void p_t (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = (y << 5) + x + 0x2000;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = (n / 10) + 16;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr);
	update_list [update_index++] = (n % 10) + 16;
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

void wbtul (unsigned char b) {
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr++);
	update_list [update_index++] = b;
}

void update_list_tile (unsigned char x, unsigned char y, unsigned char tl) {
	upd_attr_table (x, y, tl);
	
	gp_addr = 0x23c0 + rdc;
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr);
	update_list [update_index++] = rda;
	
	// tiles
	//tl = (16 + tl) << 2;
	gp_tmap = tsmap + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	wbtul (*gp_tmap++);
	wbtul (*gp_tmap++);
	gp_addr += 30;
	wbtul (*gp_tmap++);
	wbtul (*gp_tmap);
}

void map_set (unsigned char x, unsigned char y, unsigned char n) {
	map_buff [x + (y << 4)] = n;
	map_attr [x + (y << 4)] = tbehs [n];
	update_list_tile (x + x, TOP_ADJUST + y + y, n); 
}

void draw_map_tile (t) {
	rda = rdx + (rdy << 4);
	map_buff [rda] = t;		
	map_attr [rda] = tbehs [t];
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

}
/*
void pr_str (unsigned char x, unsigned char y, unsigned char *s) {
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr);
	while (gpit = *s++) {
		//vram_adr (gp_addr++);
		if (gpit != '_') vram_put (gpit - 32); else vram_put (0);
	}
}

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

// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// printer.h
// Draw map, print text, etcetera.

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

// Clear update list
void clear_update_list (void) {
	gpitu = UPDATE_LIST_SIZE * 3; while (gpitu --) update_list [gpitu] = 0;
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
	ul_putc ((n/10)+16);
	ul_putc ((n%10)+16);
}

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};
unsigned char attr_table [64];

void upd_attr_table (unsigned char x, unsigned char y, unsigned char tl) {
	rdc = (x >> 2) + ((y >> 2) << 3);
	rdb = ((x >> 1) & 1) + (((y >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (c_ts_pals [tl] << (rdb << 1));
	attr_table [rdc] = rda;
}

void draw_tile (unsigned char x, unsigned char y, unsigned char tl) {
	upd_attr_table (x, y, tl);
	
	gp_tmap = c_ts_tmaps + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap++);
	gp_addr += 31;
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap);	
}

void update_list_tile (unsigned char x, unsigned char y, unsigned char tl) {
	upd_attr_table (x, y, tl);
	
	gp_addr = 0x23c0 + rdc;
	ul_putc (rda);
	
	// tiles
	//tl = (16 + tl) << 2;
	gp_tmap = c_ts_tmaps + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	ul_putc (*gp_tmap ++);
	ul_putc (*gp_tmap ++);
	gp_addr += 30;
	ul_putc (*gp_tmap ++);
	ul_putc (*gp_tmap);
}

void map_set (unsigned char x, unsigned char y, unsigned char n) {
	map_buff [x + (y << 4)] = n;
	map_attr [x + (y << 4)] = c_behs [n];
	update_list_tile (x + x, TOP_ADJUST + y + y, n); 
}

unsigned char get_byte (void) {
	rdit --; return *gp_gen ++;
}

#ifdef MAP_RENDERER_COMPLEX
	#include "engine/mapmods/map_renderer_complex.h"
#else
	#include "engine/mapmods/map_renderer_fast.h"
#endif


void pr_str (unsigned char x, unsigned char y, unsigned char *s) {
	vram_adr (((y << 5) | x) + 0x2000);
	while (gpit = *s++) vram_put (gpit - 32); 
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

#ifdef DEBUG
unsigned char get_hex_digit (unsigned char n) {
	if (n < 10) return n + 16;
	return n + 23;
}

void debug_print_hex_16_dl (unsigned char x, unsigned char y, unsigned int n) {
	clear_update_list ();

	gp_addr = (y << 5) + x + 0x2000;
	ul_putc (get_hex_digit (n >> 12));
	ul_putc (get_hex_digit ((n >> 8) & 0xf));
	ul_putc (get_hex_digit ((n >> 4) & 0xf));
	ul_putc (get_hex_digit ((n & 0xf)));

	ppu_waitnmi ();
}
#endif
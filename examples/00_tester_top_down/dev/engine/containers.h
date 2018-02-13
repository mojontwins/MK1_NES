// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

#define MAX_CONTAINERS 		5

unsigned char c_x [MAX_CONTAINERS], c_y [MAX_CONTAINERS], c_f [MAX_CONTAINERS], last_container;

void containers_init (void) {
	last_container = 0;
}

void containers_add (unsigned char x, unsigned char y, unsigned char f) {
	c_x [last_container] = x;
	c_y [last_container] = y;
	c_f [last_container ++] = f;
}

void containers_do (void) {
	rdx = (prx + 8) >> 4; rdy = (pry + 8) >> 4;
	gpit = last_container; while (gpit --) {
		if (ppossee && rdx == c_x [gpit] && rdy == c_y [gpit]){
			pvx = pvy = 0;
			upd_cont_index = c_f [gpit];
			use_ct = 1;
		}
	}
}

void containers_draw (void) {
	gpit = last_container; while (gpit --) {
		oam_index = oam_meta_spr (c_x [gpit] << 4, (c_y [gpit] << 4) + SPRITE_ADJUST, 
			oam_index,
			spr_hs [flags [c_f [gpit]]]);
	}
}

// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// bolts.h
// Lock management

void bolts_load (void) {
	gp_gen = (unsigned char *) (c_locks);
	gpit = c_max_bolts; while (gpit --) {
		lkact [gpit] = 1;
	}	
}

void clear_cerrojo (unsigned char yx) {
	gp_gen = (unsigned char *) c_locks;
	gpit = c_max_bolts; while (gpit --) {
		rda = *gp_gen ++; rdb = *gp_gen ++;
		if (n_pant == rda && yx == rdb) {
			lkact [gpit] = 0;
		}
	}
}
	
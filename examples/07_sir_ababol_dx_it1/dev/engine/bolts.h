// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// bolts.h
// Lock management

void bolts_load (void) {
	gp_gen = (unsigned char *) (c_locks);
	gpit = c_max_bolts; while (gpit --) {
		lknp [gpit] = *gp_gen++;
		lkyx [gpit] = *gp_gen++;
		lkact [gpit] = 1;
	}	
}

void clear_cerrojo (unsigned char yx) {
	gpit = c_max_bolts; while (gpit --) {
		if (n_pant == lknp [gpit] && yx == lkyx [gpit]) {
			lkact [gpit] = 0;
		}
	}
}
	
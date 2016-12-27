// NES MK1 v0.3
// Copyleft Mojon Twins 2013, 2015

// bolts.h
// Lock management

void __fastcall__ bolts_load (void) {
	gp_gen = (unsigned char *) (c_locks);
	for (gpit = 0; gpit < c_max_bolts; gpit ++) {
		lknp [gpit] = *gp_gen++;
		lkxy [gpit] = *gp_gen++;
		lkact [gpit] = 1;
	}	
}

void clear_cerrojo (unsigned char xy) {
	for (gpit = 0; gpit < c_max_bolts; gpit ++) {
		if (n_pant == lknp [gpit] && xy == lkxy [gpit]) {
			lkact [gpit] = 0;
		}
	}
}
	
// NES MK1 v0.1b
// Copyleft Mojon Twins 2013, 2015

// bolts.h
// Lock management

void __fastcall__ bolts_load (void) {
	gp_gen = (unsigned char *) (c_locks);
	for (gpit = 0; gpit < MAX_CERROJOS; gpit ++) {
		lknp [gpit] = *gp_gen++;
		lkxy [gpit] = *gp_gen++;
		lkact [gpit] = 1;
	}	
}

void clear_cerrojo (unsigned char xy) {
	for (gpit = 0; gpit < MAX_CERROJOS; gpit ++) {
		if (n_pant == lknp [gpit] && xy == lkxy [gpit]) {
			lkact [gpit] = 0;
		}
	}
}
	
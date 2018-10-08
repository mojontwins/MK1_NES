// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// bolts.h
// Lock management

void bolts_load (void) {
	gp_gen = (unsigned char *) (c_locks);
	gpit = c_max_bolts; while (gpit --) {
		lkact [gpit] = 1;
	}	
}

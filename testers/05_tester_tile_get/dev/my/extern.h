// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extern.h
// this function is ran from the script:

void do_extern_action (unsigned char n) {
	if (n < 2) {
		rdb = n + 1;
		if (0 == en_ct [rdb] || en_flags [rdb]) {
			en_x [rdb] = en_x1 [rdb]; en_y [rdb] = en_y1 [rdb];
			en_ct [rdb] = 250;
			en_flags [rdb] = 0;
			en_life [rdb] = 1;
			en_t [rdb] = 13;

			enf_x [rdb] = en_x [rdb] << 6;
			enf_y [rdb]= en_y [rdb] << 6;
			enf_vy [rdb] = 0; 
			enf_vx [rdb] = ADD_SIGN2 (en_x2 [rdb], en_x1 [rdb], en_mx [rdb]) << FIXBITS;
		}
	}
}

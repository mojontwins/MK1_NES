// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Pursuing enemies

switch (en_alive [gpit]) {
	case 0:
		// IDLE

		if (en_ct [gpit]) {
			en_ct [gpit] --; en_y [gpit] = 240;
		} else {
			en_alive [gpit] = 1;
			en_x [gpit] = en_x1 [gpit];
			en_y [gpit] = en_y1 [gpit];
			en_rawv [gpit] = 1 << (rand8 () % 5);
			if (en_rawv [gpit] > 4) en_rawv [gpit] = 1;
			if (en_rawv [gpit] == 1) en_status [gpit] = 1; else en_rawv [gpit] >>= 1;
			en_ct [gpit] = 50 + (rand8 () & 31);
		}
		break;
	case 1:
		// Appearing
		en_spr = ENEMS_EXPLODING_CELL;
		if (en_ct [gpit]) en_ct [gpit] --; else en_alive [gpit] = 2;
		break;
	case 2:
		// Pursuing

		if (pstate == EST_NORMAL && (!en_status [gpit] || half_life)) {
			en_mx [gpit] = add_sign (((prx >> 2) << 2) - en_x [gpit], en_rawv [gpit]);
			en_my [gpit] = add_sign (((pry >> 2) << 2) - en_y [gpit], en_rawv [gpit]);

			// Vertical

			en_y [gpit] += en_my [gpit];

#ifdef WALLS_STOP_ENEMIES
			// Collision detection

			if (en_my [gpit]) {
				cx1 = (en_x [gpit] + 4) >> 4;
				cx2 = (en_x [gpit] + 11) >> 4;

				if (en_my [gpit] < 0) {
					cy1 = cy2 = (en_y [gpit] + 8) >> 4;
					rda = ((cy1 + 1) << 4) - 8;
				} else {
					cy1 = cy2 = (en_y [gpit] + 15) >> 4;
					rda = (cy1 - 1) << 4;
				}

				cm_two_points ();
				if (at1 || at2) {
					en_y [gpit] = rda;
				}
			}
#endif

			// Horizontal

			en_x [gpit] += en_mx [gpit];

#ifdef WALLS_STOP_ENEMIES
			// Collision detection

			if (en_mx [gpit]) {
				cy1 = (en_y [gpit] + 8) >> 4;
				cy2 = (en_y [gpit] + 15) >> 4;
	
				if (en_mx [gpit] < 0) {
					cx1 = cx2 = (en_x [gpit] + 4) >> 4;
					rda = ((cx1 + 1) << 4) - 4;
				} else {
					cx1 = cx2 = (en_x [gpit] + 11) >> 4;
					rda = ((cx1 - 1) << 4) + 4;
				}

				cm_two_points ();
				if (at1 || at2) {
					en_x [gpit] = rda;
				}
			}
#endif

		}
		
		en_spr = ((TYPE_7_FIXED_SPRITE - 1) << 3) + en_fr;
		break;
}					

en_facing [gpit] = 0;

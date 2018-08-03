// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Pursuing enemies

switch (_en_state) {
	case 0:
		// IDLE

		if (_en_ct) {
			-- _en_ct; _en_y = 240;
		} else {
			_en_state = 1;
			_en_x = _en_x1;
			_en_y = _en_y1;
			en_rawv [gpit] = 1 << (rand8 () % 5);
			if (en_rawv [gpit] > 4) en_rawv [gpit] = 1;
			if (en_rawv [gpit] == 1) en_status [gpit] = 1; else en_rawv [gpit] >>= 1;
			_en_ct = 50 + (rand8 () & 31);
		}
		break;
	case 1:
		// Appearing
		en_spr = ENEMS_EXPLODING_CELL;
		if (_en_ct) -- _en_ct; else _en_state = 2;
		break;
	case 2:
		// Pursuing

		if (pflickering == 0 && pbouncing == 0 && (!en_status [gpit] || half_life)) {
			_en_mx = add_sign (((prx >> 2) << 2) - _en_x, en_rawv [gpit]);
			_en_my = add_sign (((pry >> 2) << 2) - _en_y, en_rawv [gpit]);

			// Vertical

			_en_y += _en_my;

#ifdef WALLS_STOP_ENEMIES
			// Collision detection

			if (_en_my) {
				cx1 = (_en_x + 4) >> 4;
				cx2 = (_en_x + 11) >> 4;

				if (_en_my < 0) {
					cy1 = cy2 = (_en_y + 8) >> 4;
					rda = ((cy1 + 1) << 4) - 8;
				} else {
					cy1 = cy2 = (_en_y + 15) >> 4;
					rda = (cy1 - 1) << 4;
				}

				cm_two_points ();
				if (at1 || at2) {
					_en_y = rda;
				}
			}
#endif

			// Horizontal

			_en_x += _en_mx;

#ifdef WALLS_STOP_ENEMIES
			// Collision detection

			if (_en_mx) {
				cy1 = (_en_y + 8) >> 4;
				cy2 = (_en_y + 15) >> 4;
	
				if (_en_mx < 0) {
					cx1 = cx2 = (_en_x + 4) >> 4;
					rda = ((cx1 + 1) << 4) - 4;
				} else {
					cx1 = cx2 = (_en_x + 11) >> 4;
					rda = ((cx1 - 1) << 4) + 4;
				}

				cm_two_points ();
				if (at1 || at2) {
					_en_x = rda;
				}
			}
#endif

		}
		
		en_spr = ((TYPE_7_FIXED_SPRITE - 1) << 3) + en_fr;
		break;
}					

_en_facing = 0;

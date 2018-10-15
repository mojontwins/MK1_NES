// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for screen boundaries. Do your custom screen flicking here.

#ifndef ACTIVATE_SCRIPTING
	switch (level) {
		case 0:
			switch (n_pant) {
				case 2:
					if (prx == 244 && pvx > 0) {
						px = 4<<FIXBITS;
						level = 1;
						n_pant = 0;
						warp_to_level = 1;
						flick_override = 1;
					}

					if (b_button) {
						IF_PLAYER_TOUCHES (7, 10) {
							level = 1;
							n_pant = 2;
							px = 14 << 10;
							py = 8 << 10;
							warp_to_level = 1;
						}
					}
					break;
			}

			break;
		case 1:
			switch (n_pant) {
				case 0:
					if (prx == 4 && pvx < 0) {
						px = 244<<FIXBITS;
						level = 0;
						n_pant = 2;
						warp_to_level = 1;
						flick_override = 1;
					}
					break;
				case 2:
					if (b_button) {
						IF_PLAYER_TOUCHES (14, 8) {
							level = 0;
							n_pant = 2;
							px = 7 << 10;
							py = 10 << 10;
							warp_to_level = 1;
						}
					}
					break;
			}

			break;
	}	
#endif

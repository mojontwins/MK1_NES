			// Change screen
			// Very basic. Extend when needed.
			if (prx == 0 && (i & PAD_LEFT)) {
				n_pant --;
				px = 240 * 64;
			} else if (prx >= 240 && (i & PAD_RIGHT)) {
				n_pant ++;
				px = 0;
#ifdef PLAYER_MOGGY_STYLE				
			} else if (pry == 0 && (i & PAD_UP)) {
				n_pant -= MAP_W;
				py = 176 * 64;
			} else if (pry >= 176 && (i & PAD_DOWN)) {
				n_pant += MAP_W;
				py = 0;
#else
			} else if (pry == 0 && (pvy < 0)) {
				n_pant -= MAP_W;
				py = 176 * 64;
			} else if (pry >= 176 && (pvy > 0)) {
				n_pant += MAP_W;
				py = 0;
#endif				
			}

			// Change screen
			// Very basic. Extend when needed.
			//if (prx == 0 && (i & PAD_LEFT)) {
			if (prx == 4 && pvx < 0) {
				n_pant --;
				px = 244 << FIXBITS;
			//} else if (prx >= 240 && (i & PAD_RIGHT)) {
			} else if (prx == 244 && pvx > 0) {
				n_pant ++;
				px = 4 << FIXBITS;
#ifdef PLAYER_TOP_DOWN				
			//} else if (pry == 0 && (i & PAD_UP)) {
			} else if (pry <= 16 && pvy < 0) {
				n_pant -= MAP_W;
				py = 192 << FIXBITS;
			//} else if (pry >= 176 && (i & PAD_DOWN)) {
			} else if (pry >= 192 && pvy > 0) {
				n_pant += MAP_W;
				py = 16 << FIXBITS;
#else
			} else if (pry == 0 && pvy < 0 && n_pant >= MAP_W) {
				n_pant -= MAP_W;
				py = 192 << FIXBITS;
				if (pvy > -192) pvy = -192;
			} else if (pry >= 192 && pvy > 0) {
				n_pant += MAP_W;
				py = 0;
#endif				
			}

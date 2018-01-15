			// Pause?
			if (i & PAD_START) {
				while (pad_poll (0) & PAD_START);
				music_pause (1);
				sfx_play (0, 1);
				pal_bright (3);
				ppu_waitnmi ();
				while (!(pad_poll (0) & PAD_START));
				pal_bright (4);
				music_pause (0);
				while (pad_poll (0) & PAD_START);
				sfx_play (0, 1);
			}

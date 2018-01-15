	switch (en_alive [gpit]) {
		case 0:
			// Wait for idle.
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 1);
				en_mx [gpit] = CHAC_CHAC_IDLE_2;
				en_alive [gpit] = 1;
			}
			break;
		case 1:
			// Show yer teeth
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 3);
				en_mx [gpit] = CHAC_CHAC_IDLE_3;
				en_alive [gpit] = 2;
			}
			break;
		case 2:
			// Closing!
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE + 5, CHAC_CHAC_BASE_TILE + 4, CHAC_CHAC_BASE_TILE + 5);
				en_mx [gpit] = CHAC_CHAC_IDLE_4;
				en_alive [gpit] = 3;
			}
			break;
		case 3:
			// Shut!
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 3);
				en_mx [gpit] = CHAC_CHAC_IDLE_3;
				en_alive [gpit] = 4;
			}
			break;
		case 4:
			// Backoff 1
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 1);
				en_mx [gpit] = CHAC_CHAC_IDLE_3;
				en_alive [gpit] = 5;
			}
			break;
		case 5:
			// Backoff 2
			if (en_mx [gpit]) en_mx [gpit] --; else {
				draw_chac_chac (CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6);
				en_mx [gpit] = en_my [gpit];
				en_alive [gpit] = 0;
			}
			break;
	}

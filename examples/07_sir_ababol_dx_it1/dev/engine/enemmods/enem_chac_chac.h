// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// CHAC CHAC!

switch (en_alive [gpit]) {
	case 0:
		// Wait for idle.
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 1);
			_en_mx = CHAC_CHAC_IDLE_2;
			en_alive [gpit] = 1;
		}
		break;
	case 1:
		// Show yer teeth
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 3);
			_en_mx = CHAC_CHAC_IDLE_3;
			en_alive [gpit] = 2;
		}
		break;
	case 2:
		// Closing!
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE + 5, CHAC_CHAC_BASE_TILE + 4, CHAC_CHAC_BASE_TILE + 5);
			_en_mx = CHAC_CHAC_IDLE_4;
			en_alive [gpit] = 3;
		}
		break;
	case 3:
		// Shut!
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 3);
			_en_mx = CHAC_CHAC_IDLE_3;
			en_alive [gpit] = 4;
		}
		break;
	case 4:
		// Backoff 1
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 1);
			_en_mx = CHAC_CHAC_IDLE_3;
			en_alive [gpit] = 5;
		}
		break;
	case 5:
		// Backoff 2
		if (_en_mx) _en_mx --; else {
			enems_draw_chac_chac (CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6);
			_en_mx = _en_my;
			en_alive [gpit] = 0;
		}
		break;
}

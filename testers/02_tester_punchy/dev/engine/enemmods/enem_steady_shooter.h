// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Steady Shooters. Don't move and shoot from time to time.

if (ticker == 0) {
	if (_en_ct) -- _en_ct; else {
		_en_ct = _en_mx;	// reset counter
		rda = _en_my; 		// direction
		rdx = _en_x + 4;
		rdy = _en_y + 4;
		cocos_shoot_linear ();
	}
}

en_spr = _en_s;

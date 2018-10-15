// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Boioiongs bounce around the screen until a timer exhausts.

if (_en_ct) {
	_en_ct --;		

	// Vertical movement & collision
	// -----------------------------

	_enf_vy += BOIOIONG_G; if (_enf_vy > BOIOIONG_VY_MAX) _enf_vy = BOIOIONG_VY_MAX;
	_enf_y += _enf_vy;

	if (_enf_y < 0) _enf_y = 0;
	if (_enf_y > (192 << FIXBITS)) _enf_y = (192 << FIXBITS);

	_en_y = _enf_y >> FIXBITS; _en_x = _enf_x >> FIXBITS;

	cx1 = _en_x >> 4; cx2 = (_en_x + 15) >> 4; rda = 0;

	if (_enf_vy > 0) {
		cy1 = cy2 = (_en_y + 15) >> 4;
		rda = 13; rds = -16;
	} else if (_enf_vy < 0) {
		cy1 = cy2 = _en_y >> 4;
		rda = 9; rds = 16;
	}

	if (rda) {
		cm_two_points ();
		if ((at1 & rda) || (at2 & rda)) {
			_en_y = rds + (cy1 << 4);
			_enf_vy = -_enf_vy;
		}
	}

	_enf_y = _en_y << FIXBITS;

	// Horizontal movement & collision
	// -------------------------------

	_enf_x += _enf_vx;
	_en_x = _enf_x >> FIXBITS;

	if (_enf_x < 0) _enf_x = 0;
	if (_enf_x > (240 << FIXBITS)) _enf_x = (240 << FIXBITS);

	cy1 = _en_y >> 4; cy2 = (_en_y + 15) >> 4; rda = 0;

	if (_enf_vx) {
		if (_enf_vx > 0) {
			cx1 = cx2 = (_en_x + 15) >> 4; rds = -16;
		} else {
			cx1 = cx2 = _en_x >> 4; rds = 16;
		}

		cm_two_points ();
		if ((at1 & 9) || (at2 & 9)) {
			_en_x = rds + (cx1 << 4);
			_enf_vx = -_enf_vx;
		}
	}

	_enf_x = _en_x << FIXBITS;

	// Sprite

	if (_en_ct > 50 || half_life) {
		en_spr = _en_s + ((frame_counter >> 2) & 1);
	}
} 
#ifdef BOIOIONG_AUTO_RESPAWN
else {
	_en_x = _en_x1; _en_y = _en_y1;	
	enems_boioiong_init ();
}
#endif

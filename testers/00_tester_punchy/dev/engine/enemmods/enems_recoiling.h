// NES v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Enems recoiling

if (en_life [gpit]) {
	rdx = _en_x; _en_x += en_rmx [gpit];

	#ifdef WALLS_STOP_ENEMIES
		cy1 = _en_y >> 4;
		cy2 = (_en_y + 15) >> 4;

		if (_en_mx < 0) {
			cx1 = cx2 = _en_x >> 4;
		} else {
			cx1 = cx2 = (_en_x + 15) >> 4;
		}
		cm_two_points ();
		en_collx = at1 | at2;
	#endif

	if (_en_x <= _en_x1 || _en_x >= _en_x2 || en_collx) _en_x = rdx;
}

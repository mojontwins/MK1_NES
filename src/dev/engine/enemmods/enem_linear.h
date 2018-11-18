// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Good ol' patrollers

#if defined (ENABLE_SHOOTIES)
	rdx = _en_x;
	rdy = _en_y;
#endif

if (!en_status [gpit] || half_life) {

	// Horizontal

	_en_x += _en_mx;

	#ifdef WALLS_STOP_ENEMIES
		cy1 = _en_y >> 4;
		cy2 = (_en_y + 15) >> 4;

		if (_en_mx < 0) {
			cx1 = cx2 = _en_x >> 4;
		} else {
			cx1 = cx2 = (_en_x + 15) >> 4;
		}
		cm_two_points ();
		en_collx = (at1 | at2) & 13;
	#endif

	// Vertical

	_en_y += _en_my;

	#ifdef WALLS_STOP_ENEMIES
		cx1 = _en_x >> 4;
		cx2 = (_en_x + 15) >> 4;

		if (_en_my < 0) {
			cy1 = cy2 = _en_y >> 4;
		} else {
			cy1 = cy2 = (_en_y + 15) >> 4;
		}
		cm_two_points ();
		en_colly = (at1 | at2) & 13;
	#endif

	// Change direction ?
	
	#ifdef PLAYER_SAFE_LANDING
		#ifdef WALLS_STOP_ENEMIES
			if (_en_x == _en_x1 || _en_x == _en_x2 || en_collx) _en_mx = -_en_mx;
		#else
			if (_en_x == _en_x1 || _en_x == _en_x2) _en_mx = -_en_mx;
		#endif
		#ifdef WALLS_STOP_ENEMIES
			if (en_colly) _en_my = -_en_my;
			else 
		#endif
			{
				if (_en_y <= _en_y1) { _en_y = _en_y1; _en_my = ABS (_en_my); }
				else if (_en_y >= _en_y2) { _en_y = _en_y2; _en_my = -ABS (_en_my); }
			}
	#else
		#ifdef WALLS_STOP_ENEMIES
			if (_en_x == _en_x1 || _en_x == _en_x2 || en_collx) _en_mx = -_en_mx;
			if (_en_y == _en_y1 || _en_y == _en_y2 || en_colly) _en_my = -_en_my;
		#else	
			if (_en_x == _en_x1 || _en_x == _en_x2) _en_mx = -_en_mx;
			if (_en_y == _en_y1 || _en_y == _en_y2) _en_my = -_en_my;						
		#endif
	#endif
}

//_en_facing = (_en_mx > 0 || _en_my > 0) ? 0 : 4;
//rda = ( _en_mx < 0 || _en_my < 0); enems_facing ();

rda = _en_mx ? (_en_mx < 0) : (_en_my < 0); enems_facing ();

en_spr = _en_s + en_fr + _en_facing;

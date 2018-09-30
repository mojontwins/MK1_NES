// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

	// Fanties
	
	// Modify v

	if (px < _enf_x) {
		_enf_vx -= FANTY_A; if (_enf_vx < -FANTY_MAXV) _enf_vx = -FANTY_MAXV;
	} else {
		_enf_vx += FANTY_A; if (_enf_vx > FANTY_MAXV) _enf_vx = FANTY_MAXV;
	}

	if (py < _enf_y) {
		_enf_vy -= FANTY_A; if (_enf_vy < -FANTY_MAXV) _enf_vy = -FANTY_MAXV;
	} else {
		_enf_vy += FANTY_A; if (_enf_vy > FANTY_MAXV) _enf_vy = FANTY_MAXV;
	}

	// Horizontal

	_enf_x += _enf_vx; 
	if (_enf_x < 0) _enf_x = 0;
	if (_enf_x > MAX_ENX<<FIXBITS) _enf_x = MAX_ENX<<FIXBITS;
	EN_X_ABSOLUTE = _enf_x >> 6;

	#ifdef FANTY_COLLIDES

		if (_enf_vx) {
			cy1 = (_en_y + 4) >> 4;
			cy2 = (_en_y + 11) >> 4;

			if (_enf_vx > 0) {
				cx1 = cx2 = (EN_X_ABSOLUTE + 11) >> 4;
				ENXA = ((cx2 - 1) << 4) + 4;
			} else {
				cx1 = cx2 = (EN_X_ABSOLUTE + 4) >> 4;
				ENXA = ((cx1 + 1) << 4) - 4;
			}
			cm_two_points ();
			if ((at1 & 8) || (at2 & 8)) {
				_enf_vx = -_enf_vx;
				EN_X_ABSOLUTE = ENXA; 
				_enf_x = ENXA << FIXBITS;
			}
		}

	#endif

	// Vertical
		
	_enf_y += _enf_vy; 
	if (_enf_y < 0) _enf_y = 0;
	if (_enf_y > 11264) _enf_y = 11264;
	_en_y = _enf_y >> 6;

	#ifdef FANTY_COLLIDES

		if (_enf_vy) {
			cx1 = (EN_X_ABSOLUTE + 4) >> 4;
			cx2 = (EN_X_ABSOLUTE + 11) >> 4;

			if (_enf_vy > 0) {
				rdb = 12;
				cy1 = cy2 = (_en_y + 11) >> 4;
				rda = ((cy2 - 1) << 4) + 4;
			} else {
				rdb = 8;
				cy1 = cy2 = (_en_y + 4) >> 4;
				rda = ((cy1 + 1) << 4) - 4;
			}

			cm_two_points ();
			if ((at1 & rdb) || (at2 & rdb)) {
				_enf_vy = -_enf_vy;
				_en_y = rda;
				_enf_y = rda << FIXBITS;
			}
		}

	#endif

	#ifdef FANTY_KILLED_BY_TILE
		cx1 = (EN_X_ABSOLUTE + 8) >> 4;
		cy1 = (_en_y + 8) >> 4;
		cm_two_points ();
		if (at1 & 1) {
			en_cttouched [gpit] = 8;
			enems_kill ();
		}
	#endif

#ifdef FANTY_FAST_ANIM
	en_fr = half_life;
#else
	en_fr = (EN_X_ABSOLUTE >> 3) & 1;
#endif

#ifdef FANTY_WITH_FACING
	//_en_facing = ((EN_X_ABSOLUTE < prx) ? 0 : 4);
	rda = (prx < EN_X_ABSOLUTE); enems_facing ();
	en_spr = _en_s + en_fr + _en_facing;
#else
	en_spr = _en_s + en_fr;
#endif


// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Precalculated, all-integer fanty from Goddess. 

// Needs FANTY_INCS_MAX and fanty_incs
// Needs fanty_retreat_incs

// Like homing fanties but with states removed, i.e. always "pursuing"

	// Accelerate X
	if (prx > FANTY_ENX) {		
		_en_mx ++; if (_en_mx >= FANTY_INCS_MAX - 1) _en_mx = FANTY_INCS_MAX - 1;
	} else if (prx < FANTY_ENX) {
		_en_mx --; if (_en_mx <= -(FANTY_INCS_MAX - 1)) _en_mx = - (FANTY_INCS_MAX - 1);
	}
	
	// Move X
	FANTY_RDX = FANTY_ENX; if (rand8 () & 0x7) {
		if (_en_mx < 0) FANTY_ENX -= fanty_incs [- _en_mx];
		else if (_en_mx > 0) FANTY_ENX += fanty_incs [_en_mx];
	}
	
	// Collide X
	if ((FANTY_ENX < 8 && _en_mx < 0) || (FANTY_ENX > MAX_ENX && _en_mx > 0)) _en_mx = -_en_mx;
#ifdef FANTY_COLLIDES
	if (_en_mx) {
		cx1 = cx2 = _en_mx < 0 ? ((FANTY_ENX + 4) >> 4) : ((FANTY_ENX + 11) >> 4);
		cy1 = (_en_y + 4) >> 4; cy2 = (_en_y + 11) >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) { _en_mx = -_en_mx; FANTY_ENX = FANTY_RDX; }
	}
#endif	

	// Accelerate Y
	if (pry > _en_y) {
		_en_my ++; if (_en_my >= FANTY_INCS_MAX - 1) _en_my = FANTY_INCS_MAX - 1;
	} else if (pry < _en_y) {
		_en_my --; if (_en_my <= -(FANTY_INCS_MAX - 1)) _en_my = - (FANTY_INCS_MAX - 1);
	}

	// Move Y
	rdy = _en_y; if (rand8 () & 0x7)  {
		if (_en_my < 0) _en_y -= fanty_incs [- _en_my];
		else if (_en_my > 0) _en_y += fanty_incs [_en_my];
	}

	// Collide Y
	if ((_en_y < 8 && _en_my < 0) || (_en_y > 232 && _en_my > 0)) _en_my = -_en_my;
#ifdef FANTY_COLLIDES
	if (_en_my) {
		if (_en_my < 0) {
			cy1 = cy2 = ((_en_y + 4) >> 4);
			rdb = 8;
		} else {
			cy1 = cy2 = ((_en_y + 11) >> 4);
			rdb = 12;
		}
		cx1 = (FANTY_ENX + 4) >> 4; cx2 = (FANTY_ENX + 11) >> 4;
		cm_two_points ();
		if ((at1 & rdb) || (at2 & rdb)) { _en_my = 0; _en_y = rdy; }
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

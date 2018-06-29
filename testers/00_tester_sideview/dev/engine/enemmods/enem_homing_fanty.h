// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Fanties that have a home

#ifdef FANTY_KILLED_BY_TILE
	#define FANTY_OBSTACLE(a) ((a)>1)
#else
	#define FANTY_OBSTACLE(a) (a)
#endif

// Precalc distance
rdx = _en_x; rdy = _en_y; rdt = distance ();

// Modify v - always upon state

switch (en_alive [gpit]) {
	case 0:
		// Retreating
		_enf_vx = ADD_SIGN2 (_en_x1, _en_x, FANTY_V_RETREAT);
		_enf_vy = ADD_SIGN2 (_en_y1, _en_y, FANTY_V_RETREAT);
		if (rdt < FANTY_DISTANCE) en_alive [gpit] = 1;
		break;
	case 1:
		// Pursuing
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

		if (rdt > FANTY_DISTANCE) {
			// Adjust to pixel
			_enf_x = _en_x << FIXBITS;
			_enf_y = _en_y << FIXBITS;
			en_alive [gpit] = 0;
		}
		break;
}

// Horizontal

_enf_x += _enf_vx; 
if (_enf_x < 0) _enf_x = 0;
if (_enf_x > 15360) _enf_x = 15360;
_en_x = _enf_x >> 6;

#ifdef FANTY_COLLIDES

	if (_enf_vx) {
		cy1 = (_en_y + 4) >> 4;
		cy2 = (_en_y + 11) >> 4;

	if (_enf_vx > 0) {
			cx1 = cx2 = (_en_x + 11) >> 4;
			rda = ((cx2 - 1) << 4) + 4;
		} else {
			cx1 = cx2 = (_en_x + 4) >> 4;
			rda = ((cx1 + 1) << 4) - 4;
		}
		cm_two_points ();
		if (FANTY_OBSTACLE (at1) || FANTY_OBSTACLE (at2)) {
			_enf_vx = -_enf_vx;
			_en_x = rda; 
			_enf_x = rda << FIXBITS;
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
		cx1 = (_en_x + 4) >> 4;
		cx2 = (_en_x + 11) >> 4;

	if (_enf_vy > 0) {	
			cy1 = cy2 = (_en_y + 11) >> 4;
			rda = ((cy2 - 1) << 4) + 4;
		} else {
			cy1 = cy2 = (_en_y + 4) >> 4;
			rda = ((cy1 + 1) << 4) - 4;
		}

		cm_two_points ();
		if (FANTY_OBSTACLE (at1) || FANTY_OBSTACLE (at2)) {
			_enf_vy = -_enf_vy;
			_en_y = rda;
			_enf_y = rda << FIXBITS;
		}
	}

#endif

#ifdef FANTY_KILLED_BY_TILE
	cx1 = (_en_x + 8) >> 4;
	cy1 = (_en_y + 8) >> 4;
	cm_two_points ();
	if (at1 & 1) {
		en_cttouched [gpit] = 8;
		enems_kill ();
	}
#endif

en_fr = (_en_x >> 3) & 1;

#ifdef FANTY_WITH_FACING
	//_en_facing = ((_en_x < prx) ? 0 : 4);
	rda = (prx < _en_x); enems_facing ();
	en_spr = FANTY_BASE_SPRID + en_fr + _en_facing;
#else
	en_spr = FANTY_BASE_SPRID + en_fr;
#endif


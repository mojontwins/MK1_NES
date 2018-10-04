// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Appear, fall, and catacrock.

#ifdef DOUBLE_WIDTH
	if (on_screen) 
#endif

{
	switch (_en_state) {
		case 0:
			// Idling.
			if (_en_ct) _en_ct --; else {
				_en_state = 1;
				_enf_y = _en_y << FIXBITS; _enf_vy = 0;
			}
			break;

		case 1:
			// Falling
			_enf_vy += CATACROCK_G; if (_enf_vy > CATACROCK_MAX_V) _enf_vy = CATACROCK_MAX_V;
			_enf_y += _enf_vy; _en_y = _enf_y >> FIXBITS;

			if (_en_y > _en_y2) {
				_en_state = 2;
				_en_ct = CATACROCK_CROCK_FRAMES;
				_en_y = _en_y2;
			}
			break;

		case 2:
			// Catacrock
			if (_en_ct) _en_ct --; else {
				_en_state = 0;
				_en_ct = CATACROCK_WAIT;
				_en_y = _en_y1;
			}

	}

	en_spr = _en_state + CATACROCK_BASE_SPRID;
}

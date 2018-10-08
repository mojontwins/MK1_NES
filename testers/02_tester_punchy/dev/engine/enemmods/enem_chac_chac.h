// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// CHAC CHAC!
// DEPRECATED

#ifdef DOUBLE_WIDTH
	if (on_screen)
#endif
{

	if (_en_mx) --_en_mx; else {
		++ _en_state;
		if (_en_state == 6) _en_state = 0;
		_en_mx = _en_state ?
			chac_chacs_times [_en_state] : _en_my;

		_x = _en_x; _y = _en_y    ; _t = chac_chacs_t1 [_en_state]; map_set ();
		_x = _en_x; _y = _en_y + 1; _t = chac_chacs_t2 [_en_state]; map_set ();
		_x = _en_x; _y = _en_y + 2; _t = chac_chacs_t3 [_en_state]; map_set ();
	}

}

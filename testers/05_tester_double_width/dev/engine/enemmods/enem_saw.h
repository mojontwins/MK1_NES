// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Legs rock.
// Remember: 
// my => emerging sense. 
// mx => moving sense.

rda = (_en_x1 == _en_x2);

switch (_en_state) {
	case 0: // Idling. Goes from 2->1 (backwards).
		if (rda) {
			_en_y -= _en_mx;
			if (_en_y == _en_y1) {
				_en_state = 1;
				_en_ct = SAW_EMERGING_STEPS;
			}
		} else {
			_en_x -= _en_mx;
			if (_en_x == _en_x1) {
				_en_state = 1;
				_en_ct = SAW_EMERGING_STEPS;
			}
		}
		break;
	case 1: // Emerging.
		if (half_life) {
			if (_en_ct --) {
				if (rda) {
					// Vertical
					_en_x += _en_my;
				} else {
					_en_y += _en_my;
				}
			} else {
				_en_state = 2;
			}
		}
		break;
	case 2: // Moving 1->2 (onwards)
		if (rda) {
			_en_y += _en_mx;
			if (_en_y == _en_y2) {
				_en_state = 3;
				_en_ct = SAW_EMERGING_STEPS;
			}
		} else {
			_en_x += _en_mx;
			if (_en_x == _en_x2) {
				_en_state = 3;
				_en_ct = SAW_EMERGING_STEPS;
			}
		}
		break;
	case 3: // Sinking
		if (half_life) {
			if (_en_ct --) {
				if (rda) {
					// Vertical
					_en_x -= _en_my;
				} else {
					_en_y -= _en_my;
				}
			} else {
				_en_state = 0;
			}
		}
		break;
}

// Occlusion
if (rda) {
	rdx = _en_x1; rdy = _en_y;
} else {
	rdx = _en_x; rdy = _en_y1;
}

oam_index = oam_meta_spr (
	rdx, rdy + SPRITE_ADJUST,
	oam_index,
	spr_enems [ENEMS_OCCLUDING_CELL]
);

// Frame selection

en_spr = SAW_BASE_SPRID + half_life;
_en_facing = 0;

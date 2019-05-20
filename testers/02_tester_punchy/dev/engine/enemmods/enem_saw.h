// NES MK1 v2.0
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
#ifdef DOUBLE_WIDTH
		__asm__ ("lda %v", rda);
		__asm__ ("beq %g", es_occ_horz);

		// SAW_RDX = _en_x1; rdy = _en_y;
		__asm__ ("lda %v", _en_x1);
		__asm__ ("sta %v", SAW_RDX);
		__asm__ ("lda #0");
		__asm__ ("sta %v+1", SAW_RDX);
		__asm__ ("lda %v", _en_y);

		// } else {
		__asm__ ("jmp %g", es_occ_endif);

	es_occ_horz:	
		// SAW_RDX = _en_x; rdy = _en_y1;
		__asm__ ("lda %v", _en_x);
		__asm__ ("sta %v", SAW_RDX);
		__asm__ ("lda #0");
		__asm__ ("sta %v+1", SAW_RDX);
		__asm__ ("lda %v", _en_y1);		

	es_occ_endif:
		__asm__ ("sta %v", rdy);
	
		// SAW_RDX += en_x_offs;
		__asm__ ("lda %v", en_x_offs);
		__asm__ ("clc");
		__asm__ ("adc %v", SAW_RDX);
		__asm__ ("sta %v", SAW_RDX);
		__asm__ ("lda %v+1", en_x_offs);
		__asm__ ("adc %v+1", SAW_RDX);
		__asm__ ("sta %v+1", SAW_RDX);

		// if (SAW_RDX < scroll_x) goto es_out_of_screen;
		__asm__ ("lda %v", SAW_RDX);
		__asm__ ("cmp %v", scroll_x);
		__asm__ ("lda %v+1", SAW_RDX);
		__asm__ ("sbc %v+1", scroll_x);
		__asm__ ("bcs %g", es_occ_if2);
		__asm__ ("jmp %g", es_out_of_screen);

	es_occ_if2:
		// if (scroll_x_r < SAW_RDX) goto es_out_of_screen;
		__asm__ ("lda %v", scroll_x_r);
		__asm__ ("cmp %v", SAW_RDX);
		__asm__ ("lda %v+1", scroll_x_r);
		__asm__ ("sbc %v+1", SAW_RDX);
		__asm__ ("bcs %g", es_occ_do);
		__asm__ ("jmp %g", es_out_of_screen);

	es_occ_do:
		oam_index = oam_meta_spr (
			SAW_RDX - scroll_x, rdy + SPRITE_ADJUST,
			oam_index,
			spr_enems [ENEMS_OCCLUDING_CELL]
		);
es_out_of_screen:
#else
if (rda) {
		SAW_RDX = _en_x1; rdy = _en_y;
} else {
		SAW_RDX = _en_x; rdy = _en_y1;
}

oam_index = oam_meta_spr (
		SAW_RDX, rdy + SPRITE_ADJUST,
	oam_index,
	spr_enems [ENEMS_OCCLUDING_CELL]
);
#endif

// Frame selection

en_spr = SAW_BASE_SPRID + half_life;
_en_facing = 0;

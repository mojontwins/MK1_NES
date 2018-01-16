
// Legs rock.
// Remember: 
// my => emerging sense. 
// mx => moving sense.

rda = (en_x1 [gpit] == en_x2 [gpit]);

switch (en_alive [gpit]) {
	case 0: // Idling. Goes from 2->1 (backwards).
		if (rda) {
			en_y [gpit] -= en_mx [gpit];
			if (en_y [gpit] == en_y1 [gpit]) {
				en_alive [gpit] = 1;
				en_ct [gpit] = SAW_EMERGING_STEPS;
			}
		} else {
			en_x [gpit] -= en_mx [gpit];
			if (en_x [gpit] == en_x1 [gpit]) {
				en_alive [gpit] = 1;
				en_ct [gpit] = SAW_EMERGING_STEPS;
			}
		}
		break;
	case 1: // Emerging.
		if (half_life) {
			if (en_ct [gpit] --) {
				if (rda) {
					// Vertical
					en_x [gpit] += en_my [gpit];
				} else {
					en_y [gpit] += en_my [gpit];
				}
			} else {
				en_alive [gpit] = 2;
			}
		}
		break;
	case 2: // Moving 1->2 (onwards)
		if (rda) {
			en_y [gpit] += en_mx [gpit];
			if (en_y [gpit] == en_y2 [gpit]) {
				en_alive [gpit] = 3;
				en_ct [gpit] = SAW_EMERGING_STEPS;
			}
		} else {
			en_x [gpit] += en_mx [gpit];
			if (en_x [gpit] == en_x2 [gpit]) {
				en_alive [gpit] = 3;
				en_ct [gpit] = SAW_EMERGING_STEPS;
			}
		}
		break;
	case 3: // Sinking
		if (half_life) {
			if (en_ct [gpit] --) {
				if (rda) {
					// Vertical
					en_x [gpit] -= en_my [gpit];
				} else {
					en_y [gpit] -= en_my [gpit];
				}
			} else {
				en_alive [gpit] = 0;
			}
		}
		break;
}

if (rda) {
	oam_meta_spr (
		en_x1 [gpit], en_y [gpit] + SPRITE_ADJUST, 
		(gpit << 4), 
		spr_black);
} else {
	oam_meta_spr (
		en_x [gpit], en_y1 [gpit] + SPRITE_ADJUST, 
		(gpit << 4), 
		spr_black);
}

oam_meta_spr (
	en_x [gpit], en_y [gpit] + SPRITE_ADJUST, 
	ENEMS_OAM_BASE + (gpit << 4), 
	spr_enems [SAW_BASE_SPRID + half_life]);

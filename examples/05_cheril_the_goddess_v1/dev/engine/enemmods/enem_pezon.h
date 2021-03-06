	// Set occluding sprite @ origin
	oam_meta_spr (
		en_x1 [gpit], en_y1 [gpit] + SPRITE_ADJUST, 
		(gpit << 4), 
		spr_enems [PEZONS_BASE_SPRID + 2]);
	
	switch (en_alive [gpit]) {
		case 0:	// Idling
			if (en_mx [gpit]) {
				en_mx [gpit] --;
			} else {
				en_alive [gpit] = 1;
				enf_y [gpit] = en_y1 [gpit] << 6;
				enf_vy [gpit] = -PEZON_THRUST;
			}
			break;

		case 1: // on air
			enf_vy [gpit] += PEZON_G;
			if (enf_vy [gpit] > PEZON_VY_FALLING_MAX) enf_vy [gpit] = PEZON_VY_FALLING_MAX;
			enf_y [gpit] += enf_vy [gpit];
			en_y [gpit] = enf_y [gpit] >> 6;

			if (en_y [gpit] >= en_y1 [gpit]) {
				en_alive [gpit] = 0;
				en_mx [gpit] = en_my [gpit];
				oam_meta_spr (
				0, 240, 
				ENEMS_OAM_BASE + (gpit << 4), 
				spr_empty);
			} else {
				oam_meta_spr (
					en_x1 [gpit], en_y [gpit] + SPRITE_ADJUST, 
					ENEMS_OAM_BASE + (gpit << 4), 
					spr_enems [PEZONS_BASE_SPRID + (enf_vy [gpit] >= -64)]);
			}
			break;
	}


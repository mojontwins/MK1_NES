// bridge.h

// Bridge bits falling. Uses bridge_* vars defined in extra_vars.h

void bridge_create (void) {
	++ bridge_idx; bridge_idx = bridge_idx & 3;
	bridge_x [bridge_idx] = _x << 4;
	bridge_y [bridge_idx] = (_y+1) << 4;
	bridge_f [bridge_idx] = 1;
}

void bridge_do (void) {
	gpit = 4; while (gpit) { -- gpit;
		if (rdy = bridge_y [gpit]) {
			rdy += bridge_f [gpit];
			if (bridge_f [gpit] < 8) ++ bridge_f [gpit];
			if (rdy >= 208) rdy = 0;
			else oam_index = oam_meta_spr (
				bridge_x [gpit], rdy + SPRITE_ADJUST,
				oam_index,
				ssbrk_00
			);
			bridge_y [gpit] = rdy;
		}
	}
}
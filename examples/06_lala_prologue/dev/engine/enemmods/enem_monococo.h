// All hail the monococo!

// x1, y1 is where it appears.
// mx is state. my is counter.

	if (en_ud [gpit]) {
		enems_jitter_spr ();
		spr_id = 50;
	} else {
		// Counter & state change
		MONOCOCO_COUNTER [gpit] --; if (!MONOCOCO_COUNTER [gpit]) {
			MONOCOCO_STATE [gpit] = (MONOCOCO_STATE [gpit] + 1) & 3; MONOCOCO_COUNTER [gpit] = monococo_state_times [MONOCOCO_STATE [gpit]] - (rand8 () & 0x15);
		}

		// Shoot
		if (MONOCOCO_STATE [gpit] == 2 && MONOCOCO_COUNTER [gpit] == MONOCOCO_FIRE_COCO_AT) {
			rdx = MONOCOCO_X [gpit] + 4; rdy = MONOCOCO_Y [gpit] + 4; simplecoco_new ();
		}

		// Sprite
		enems_spr ();
		spr_id = monococo_metasprite [MONOCOCO_STATE [gpit]] + ((MONOCOCO_STATE [gpit] == 2 && ((frame_counter >> 4) & 1)) << 1);
	}

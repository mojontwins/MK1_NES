// Super custom for Espinete Bad:
// A complete ring implementation.

// When Espinete gets hit and has rings, 
// A ring will be created, which espinete can grab again.

#define RING_A 4

void ring_create (void) {
	if (!ring_timer) {
		ring_x = px;
		ring_y = py;
		ring_vy = -384;
		ring_vx = pvx >> 2;
		ring_timer = 180;
	}
}

void ring_do (void) {
	if (ring_timer) {
		ring_timer --;

		// Vertical movement & collision
		// -----------------------------

		ring_vy += PLAYER_G;
		ring_y += ring_vy; if (ring_y < 0) ring_y = 0;

		rdy = ring_y >> FIXBITS;
		rdx = 8 + (ring_x >> FIXBITS); cx1 = rdx >> 4;

		// Note how we have to substract 16 from pixel coordinates
		// To adjust to "screen", virtual coordinates.

		if (ring_vy > 0) {
			cy1 = (rdy - 1); // rdy + 15 - 16
			if (map_attr [(cy1 & 0xf0) | cx1] & 9) {
				rdy = cy1 & 0xf0; ring_vy = - ((ring_vy >> 1) + (ring_vy >> 2)) ;
			}
		} else if (ring_vy < 0) {
			cy1 = (rdy - 16);
			if (map_attr [(cy1 & 0xf0) | cx1] & 9) {
				rdy = 32 + (cy1 & 0xf0); ring_vy = 0;
			}
		}
		ring_y = rdy << FIXBITS;

		// Horizontal movement & collision
		// -------------------------------
	
		if (ring_vx > 0) {
			ring_vx -= RING_A; if (ring_vx < 0) ring_vx = 0;
		} else if (ring_vx < 0) {
			ring_vx += RING_A; if (ring_vx > 0) ring_vx = 0;
		}

		rdx = ring_x >> FIXBITS;

		ring_x += ring_vx;
		if (ring_x < 0 || ring_x > 15360) {
			ring_timer = 0;
			return;
		}

		// -16 is the adjust, + 15 is "check at bottom"
		rdy = (ring_y >> FIXBITS) - 16 + 15;
		cx1 = (ring_x >> FIXBITS) + 8;

		if (map_attr [(rdy & 0xf0)|(cx1>>4)] & 8) {
			ring_x = rdx << FIXBITS;
			ring_vx = -ring_vx;
		} else rdx = ring_x >> FIXBITS;

		// Collide with player

		// Render
		if (ring_timer > 50 || half_life) oam_index = oam_meta_spr (
			rdx, rdy + SPRITE_ADJUST,
			oam_index,
			spr_ring [(frame_counter >> 1) & 3]
		);
	}
}
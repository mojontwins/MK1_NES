// Super custom for Espinete Bad:
// A complete ring implementation.

// When Espinete gets hit and has rings, 
// A ring will be created, which espinete can grab again.

#define RING_A 				4
#define RING_TIMER_CREATE	180
#define RING_TIMER_GET		130

void ring_create (void) {
	if (!ring_timer) {
		ring_x = px;
		ring_y = py;
		ring_vy = -384;
		ring_vx = pvx >> 2;
		ring_timer = RING_TIMER_CREATE;
	}
}

void ring_do (void) {
	if (ring_timer) {
		ring_timer --;

		// The collide function was designed to work with enemies.
		// We can repurpose it here using _en_x and _en_y. That's why
		// I'm using those variables as temp. data.

		// Vertical movement & collision
		// -----------------------------

		ring_vy += PLAYER_G;
		ring_y += ring_vy; if (ring_y < 0) ring_y = 0;

		_en_y = ring_y >> FIXBITS;
		_en_x = 8 + (ring_x >> FIXBITS); cx1 = _en_x >> 4;

		// Note how we have to substract 16 from pixel coordinates
		// To adjust to "screen", virtual coordinates.

		if (ring_vy > 0) {
			cy1 = (_en_y - 1); // _en_y + 15 - 16
			if (map_attr [(cy1 & 0xf0) | cx1] & 13) {
				_en_y = cy1 & 0xf0; ring_vy = - ((ring_vy >> 1) + (ring_vy >> 2)) ;
			}
		} else if (ring_vy < 0) {
			cy1 = (_en_y - 16);
			if (map_attr [(cy1 & 0xf0) | cx1] & 9) {
				_en_y = 32 + (cy1 & 0xf0); ring_vy = 0;
			}
		}
		ring_y = _en_y << FIXBITS;

		// Horizontal movement & collision
		// -------------------------------
	
		if (ring_vx > 0) {
			ring_vx -= RING_A; if (ring_vx < 0) ring_vx = 0;
		} else if (ring_vx < 0) {
			ring_vx += RING_A; if (ring_vx > 0) ring_vx = 0;
		}

		_en_x = ring_x >> FIXBITS;

		ring_x += ring_vx;
		if (ring_x < 0 || ring_x > 15360) {
			ring_timer = 0;
			return;
		}

		// -16 is the adjust, + 15 is "check at bottom"
		_en_y = (ring_y >> FIXBITS) - 16 + 15;
		cx1 = (ring_x >> FIXBITS) + 8;

		if (map_attr [(_en_y & 0xf0)|(cx1>>4)] & 8) {
			ring_x = _en_x << FIXBITS;
			ring_vx = -ring_vx;
		} else _en_x = ring_x >> FIXBITS;

		// Collide with player
		// -------------------

		// _en_x and _en_y are set and have updated values, so we
		// can use `collide` to check collisions

		if (ring_timer < RING_TIMER_GET && collide ()) {
			sfx_play (SFX_RING, 1);
			prings = 1; ring_timer = 0; return;
		}

		// Render
		// ------
		if (ring_timer > 50 || half_life) oam_index = oam_meta_spr (
			_en_x, _en_y + SPRITE_ADJUST,
			oam_index,
			spr_ring [(frame_counter >> 1) & 3]
		);
	}
}
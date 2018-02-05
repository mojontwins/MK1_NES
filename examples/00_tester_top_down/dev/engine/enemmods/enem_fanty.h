// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Fanties

#ifdef FANTY_KILLED_BY_TILE
	#define FANTY_OBSTACLE(a) ((a)>1)
#else
	#define FANTY_OBSTACLE(a) (a)
#endif

// Modify v

enf_vx [gpit] = saturate (enf_vx [gpit] + add_sign (prx - en_x [gpit], FANTY_A), FANTY_MAXV);
enf_vy [gpit] = saturate (enf_vy [gpit] + add_sign (pry - en_y [gpit], FANTY_A), FANTY_MAXV);

// Horizontal

enf_x [gpit] += enf_vx [gpit]; 
if (enf_x [gpit] < 0) enf_x [gpit] = 0;
if (enf_x [gpit] > 15360) enf_x [gpit] = 15360;
en_x [gpit] = enf_x [gpit] >> 6;

#ifdef FANTY_COLLIDES

	if (enf_vx [gpit]) {
		cy1 = (en_y [gpit] + 4) >> 4;
		cy2 = (en_y [gpit] + 11) >> 4;

		if (enf_vx [gpit] > 0) {
			cx1 = cx2 = (en_x [gpit] + 11) >> 4;
			rda = ((cx2 - 1) << 4) + 4;
		} else {
			cx1 = cx2 = (en_x [gpit] + 4) >> 4;
			rda = ((cx1 + 1) << 4) - 4;
		}
		cm_two_points ();
		if (FANTY_OBSTACLE (at1) || FANTY_OBSTACLE (at2)) {
			enf_vx [gpit] = -enf_vx [gpit];
			en_x [gpit] = rda; 
			enf_x [gpit] = rda << 6;
		}
	}

#endif

// Vertical
	
enf_y [gpit] += enf_vy [gpit]; 
if (enf_y [gpit] < 0) enf_y [gpit] = 0;
if (enf_y [gpit] > 11264) enf_y [gpit] = 11264;
en_y [gpit] = enf_y [gpit] >> 6;

#ifdef FANTY_COLLIDES

	if (enf_vy [gpit]) {
		cx1 = (en_x [gpit] + 4) >> 4;
		cx2 = (en_x [gpit] + 11) >> 4;
		
		if (enf_vy [gpit] > 0) {
			cy1 = cy2 = (en_y [gpit] + 11) >> 4;
			rda = ((cy2 - 1) << 4) + 4;
		} else {
			cy1 = cy2 = (en_y [gpit] + 4) >> 4;
			rda = ((cy1 + 1) << 4) - 4;
		}

		cm_two_points ();
		if (FANTY_OBSTACLE (at1) || FANTY_OBSTACLE (at2)) {
			enf_vy [gpit] = -enf_vy [gpit];
			en_y [gpit] = rda;
			enf_y [gpit] = rda << 6;
		}
	}

#endif

#ifdef FANTY_KILLED_BY_TILE
	cx1 = (en_x [gpit] + 8) >> 4;
	cy1 = (en_y [gpit] + 8) >> 4;
	cm_two_points ();
	if (at1 & 1) {
		en_cttouched [gpit] = 8;
		enems_kill (gpit);
	}
#endif

en_fr = (en_x [gpit] >> 3) & 1;

#ifdef FANTY_WITH_FACING
	en_spr = FANTY_BASE_SPRID + en_fr + ((en_x [gpit] < prx) ? 0 : 2);
#else
	en_spr = FANTY_BASE_SPRID + en_fr;
#endif

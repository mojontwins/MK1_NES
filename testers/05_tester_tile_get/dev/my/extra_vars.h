// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_vars.h
// If your customs need extra variables (in BSS) define them here

#define HOTSPOT_TYPE_EMMERALD	4

signed int ring_x, ring_y;		// Ring fixed point coordinates (10.6)
signed int ring_vx, ring_vy;	// Ring fixed point velocities (10.6)
unsigned char ring_timer;		// Ring timer + "is on" flag (when != 0)
unsigned char prings, oprings;	// Rings collected.
unsigned char pemmeralds;		// Emmeralds collected, bitwise.
unsigned char level_world;		// We need those
unsigned char level_act;

// To animate the ring we will use those metasprites:

const unsigned char *const spr_ring [] = {
	ssit_00, ssit_04, ssit_05, ssit_06
};

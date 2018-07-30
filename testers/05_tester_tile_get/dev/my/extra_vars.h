// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_vars.h
// If your customs need extra variables (in BSS) define them here

signed int ring_x, ring_y;
signed int ring_vx, ring_vy;
unsigned char ring_timer;
unsigned char prings, oprings;

const unsigned char *const spr_ring [] = {
	ssit_00, ssit_04, ssit_05, ssit_06
};

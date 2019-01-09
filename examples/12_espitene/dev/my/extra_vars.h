// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_vars.h
// If your customs need extra variables (in BSS) define them here

#define HOTSPOT_TYPE_EMMERALD	4
#define HOTSPOT_TYPE_TOGGLE_OFF	6
#define HOTSPOT_TYPE_TOGGLE_ON	7

signed int ring_x, ring_y;		// Ring fixed point coordinates (10.6)
signed int ring_vx, ring_vy;	// Ring fixed point velocities (10.6)
unsigned char ring_timer;		// Ring timer + "is on" flag (when != 0)
unsigned char prings, oprings;	// Rings collected.
unsigned char pemmeralds;		// Emmeralds collected, bitwise.
unsigned char level_world;		// We need those
unsigned char level_act;
unsigned char water_strip;		// To make our life easier in Broken Fridge Zone
unsigned char underwater;		// To make our life easier in Wet Ruins Zone
unsigned char bubble_x; 
unsigned char bubble_y;			// Eye candy
unsigned char toggle_switch;	// Value of the toggle switch for Crap Brain Zone 2
unsigned char toggle_timer; 	// To 'depress' the switch.
unsigned char elec_state; 		// Electricity state in Ola K Ase Zone
unsigned char elec_state_ct; 	// Counter for state
unsigned char somari_state; 	// Somari state
unsigned char somari_ct;		// Somari counter
unsigned char somari_life;		// Somari life
unsigned char free_play;
unsigned char select_level;

// Continue from act 0 always

const unsigned char base_level [] = {
	0, 0, 0, 3, 3, 3, 6, 6, 6, 9, 9, 9, 12, 12, 12, 15, 15, 15
};
const unsigned char base_world [] = {
	0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5
};
const unsigned char base_act [] = {
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2
};

// To animate the ring we will use those metasprites:

const unsigned char *const spr_ring [] = {
	ssit_00, ssit_04, ssit_05, ssit_06
};

// And a palette cycle for the Bosque de Badajoz

unsigned char pal_cycle [3];

// Custom bg for Ola K Ase Zone

const unsigned char level5_bg [] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 17, 0, 0, 0, 0, 17, 0, 0, 16, 17, 0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 16, 16, 17, 0, 0, 17, 0, 0, 0, 0, 0, 16, 16, 0, 17, 0, 0, 16, 16, 0, 0, 0, 0, 0, 17, 0, 0, 0, 16, 17, 16, 16, 17, 0, 0, 16, 17, 0, 0, 0, 0, 0, 17, 0, 0, 16, 17, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char elec_state_max_ct [] = { 6, 1, 4 };

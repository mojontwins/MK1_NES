// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// definitions.h
// Main definitions

#define FIXBITS					6
#define TOP_ADJUST				4
#define SPRITE_ADJUST			8*TOP_ADJUST - 8 - 16 - 1

// Player states

#define EST_NORMAL 				0
#define EST_PARP 				2
#define EST_MUR 				4
#define EST_REBOUND				8

// Macros

#define COORDS(x,y) 			((x) | ((y) << 4))
#define MSB(x)					(((x) >> 8))
#define LSB(x)					(((x) & 0xff))
#define sgni(n)					((n) < 0 ? -1 : 1)
#define sgnc(a,b,c)				((a) < (b) ? -(c) : (c))
#define saturate_z(n)			((n) < 0 ? 0 : (n))
#define abs(n)					((n) < 0 ? -(n) : (n))
#define delta(a,b)				((a) < (b) ? (b) - (a) : (a) - (b))
#define min(a,b)				((a) < (b) ? (a) : (b))

// Wall hits

#define WTOP 					1
#define WBOTTOM 				2
#define WLEFT 					3
#define WRIGHT 					4

// Animation cells 

	// Cell definitions for top-down view
/*
	#define CELL_FACING_RIGHT 	0
	#define CELL_FACING_LEFT 	2
	#define CELL_FACING_UP 		4
	#define CELL_FACING_DOWN 	6
*/
	// Cell definitions for side view
	
	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	10

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	#define CELL_SWIM_CYCLE		6



// NES MK1 v0.6// Copyleft Mojon Twins 2013, 2015

// definitions.h
// Main definitions

#define FIXBITS					6
#define SPRITE_ADJUST			23
#define TOP_ADJUST				4

// Player states

#define EST_NORMAL 				0
#define EST_PARP 				2
#define EST_MUR 				4
#define EST_REBOUND				8

// Macros

#define MSB(x)					(((x)>>8))
#define LSB(x)					(((x)&0xff))
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

#ifdef PLAYER_MOGGY_STYLE

	// Cell definitions for top-down view

	#define CELL_FACING_RIGHT 	0
	#define CELL_FACING_LEFT 	2
	#define CELL_FACING_UP 		4
	#define CELL_FACING_DOWN 	6
#else

	// Cell definitions for side view
	
	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	10

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	#define CELL_SWIM_CYCLE		6
#endif



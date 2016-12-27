// NES MK1 v0.1b
// Copyleft Mojon Twins 2013, 2015

// definitions.h
// Main definitions

#define EST_NORMAL 		0
#define EST_PARP 		2
#define EST_MUR 		4
#define EST_REBOUND		8
#define sgni(n)			(n < 0 ? -1 : 1)
#define sgn(n)			(n == 0 ? 0 : n < 0 ? -1 : 1)
#define saturate(n)		(n < 0 ? 0 : n)
#define abs(n)			(n < 0 ? -n : n)
#define WTOP 1
#define WBOTTOM 2
#define WLEFT 3
#define WRIGHT 4
#define FACING_RIGHT 1
#define FACING_LEFT 6
#define FACING_UP 16
#define FACING_DOWN 11

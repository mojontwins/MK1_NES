// NES MK1 v0.5
// Copyleft Mojon Twins 2013, 2015

// definitions.h
// Main definitions

#define EST_NORMAL 		0
#define EST_PARP 		2
#define EST_MUR 		4
#define sgni(n)			(n < 0 ? -1 : 1)
#define sgnc(a,b,c)		(a < b ? -c : c)
#define saturate_z(n)	(n < 0 ? 0 : n)
#define abs(n)			(n < 0 ? -n : n)
#define delta(a,b)		(a < b ? b - a : a - b)
#define min(a,b)		(a < b ? a : b)
#define WTOP 1
#define WBOTTOM 2
#define WLEFT 3
#define WRIGHT 4
#define FACING_RIGHT 0
#define FACING_LEFT 2
#define FACING_UP 4
#define FACING_DOWN 6
#define ENEMS_OAM_BASE	48


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
#define SGNI(n)					((n) < 0 ? -1 : 1)
#define SGNC(a,b,c)				((a) < (b) ? -(c) : (c))
#define SATURATE_Z(n)			((n) < 0 ? 0 : (n))
#define ABS(n)					((n) < 0 ? -(n) : (n))
#define ADD_SIGN2(a,b,v)		((a)==(b))?(0):(((a)>(b))?(v):(-(v)))
#define ADD_SIGN(a,v) 			((a)?(((a)>0)?(v):(-(v))):(0))
#define DELTA(a,b)				((a) < (b) ? (b) - (a) : (a) - (b))
#define ATTR(x,y)				(map_attr [COORDS((x),(y))])
#define QTILE(x,y)				(map_buff [COORDS((x),(y))])

// Wall hits

#define WTOP 					1
#define WBOTTOM 				2
#define WLEFT 					3
#define WRIGHT 					4

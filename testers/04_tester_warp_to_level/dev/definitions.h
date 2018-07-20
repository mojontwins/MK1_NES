// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// definitions.h
// Main definitions

#define FIXBITS					6

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
#define ENEMS_RECOIL_X 			((_en_t == 5 || _en_t == 9 || _en_t == 11) ? 0 : ADD_SIGN2 (_en_x, prx, ENEMS_RECOIL_ON_HIT))
#define ENEMS_RECOIL_Y 			((_en_t == 5 || _en_t == 9 || _en_t == 11) ? 0 : ADD_SIGN2 (_en_y, pry, ENEMS_RECOIL_ON_HIT))
#define VRAM_READ				(*((unsigned char *) (0x2007)))
#define SET_FROM_PTR(v,p)		{__asm__("ldy #0");__asm__("lda (%v),y",p);__asm__("sta %v",v);}

// Wall hits

#define WTOP 					1
#define WBOTTOM 				2
#define WLEFT 					3
#define WRIGHT 					4

// Use types

#define USE_TYPE_HOTSPOT		1
#define USE_TYPE_INTERACTIVE	2
#define USE_TYPE_SCRIPTING		3

// Enemy states / flags

#define EN_STATE_DEAD			1
#define EN_STATE_SPAWNING		2

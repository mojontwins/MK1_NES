/* V1.4, please see mapwin.htm docs 'Exporting map for GBA' */

#define AN_END -1			/* Animation types, AN_END = end of anims */
#define AN_NONE 0			/* No anim defined */
#define AN_LOOPF 1		/* Loops from start to end, then jumps to start etc */
#define AN_LOOPR 2		/* As above, but from end to start */
#define AN_ONCE 3			/* Only plays once */
#define AN_ONCEH 4		/* Only plays once, but holds end frame */
#define AN_PPFF 5			/* Ping Pong start-end-start-end-start etc */
#define AN_PPRR 6			/* Ping Pong end-start-end-start-end etc */
#define AN_PPRF 7			/* Used internally by playback */
#define AN_PPFR 8			/* Used internally by playback */
#define AN_ONCES 9		/* Used internally by playback */


typedef struct {				/* Structure for data blocks */
long int bgoff, fgoff;			/* offsets from start of graphic blocks */
long int fgoff2, fgoff3; 		/* more overlay blocks */
unsigned long int user1, user2;	/* user long data */
unsigned short int user3, user4;	/* user short data */
unsigned char user5, user6, user7;	/* user byte data */
unsigned char tl : 1;				/* bits for collision detection */
unsigned char tr : 1;
unsigned char bl : 1;
unsigned char br : 1;
unsigned char trigger : 1;			/* bg transparency */
unsigned char unused1 : 1;
unsigned char unused2 : 1;	/* this is the flip-h bit */
unsigned char unused3 : 1;	/* this is the flip-v bit */
} BLKSTR;

typedef struct {		/* Animation control structure */
signed char antype;	/* Type of anim, AN_? */
signed char andelay;	/* Frames to go before next frame */
signed char ancount;	/* Counter, decs each frame, till 0, then resets to andelay */
signed char anuser;	/* User info */
long int ancuroff;	/* Points to current offset in list */
long int anstartoff;	/* Points to start of blkstr offsets list, AFTER ref. blkstr offset */
long int anendoff;	/* Points to end of blkstr offsets list */
} ANISTR;

typedef struct { /* Object structure */
int xpos, ypos;	/* pixel position in map to handle */
int gfxid, tileid;
int gxoff, gyoff; /* offset into graphic */
int gwidth, gheight;
int ghandlexoff, ghandleyoff; /* handle pos, from gxoff, gyoff */
int show; /* display mode */
int user1, user2, user3, user4, user5, user6, user7;
int flags;
} OBJSTR;

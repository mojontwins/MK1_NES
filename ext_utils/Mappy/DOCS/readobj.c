/* Example code segment for reading objects (add to playback lib)
 * (C)2004 R Burrows
 *
 * You should be able to call these funcs from the MapLoad
 * part of a playback lib
 * MapGetchksz and MapGetlong should be in the playback lib
 */ 

#define MAXOBJSTR 4096
#define MAXIMGSTR 4096

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

typedef struct { /* External image structure */
char * fname;
unsigned char * imgpt;
int w, h, pitch;
} OBJIMGSTR;

int curobj, numobjstr;
OBJSTR * objstrpt = NULL;
OBJIMGSTR * objimgpt = NULL;

int DecodeOBFNChunk (unsigned char * mdatpt)
{
int i, obfnsz;
OBJIMGSTR * myobjimgpt;

	if (objimgpt != NULL) free (objimgpt);
	
	objimgpt = malloc (sizeof(OBJIMGSTR)*MAXIMGSTR);
	memset (objimgpt, 0, sizeof(OBJIMGSTR)*MAXIMGSTR);
	myobjimgpt = (OBJIMGSTR *) objimgpt;

	obfnsz = MapGetchksz (mdatpt+4);
	mdatpt += 8;
	i = 0; while (obfnsz > 2) {
		myobjimgpt[i].fname = malloc (strlen(mdatpt)+1);
		strcpy (myobjimgpt[i].fname, mdatpt);
		myobjimgpt[i].imgpt = NULL;
		obfnsz -= (strlen(mdatpt)+1);
		mdatpt += (strlen(mdatpt)+1);
		i++;
	}
	
	return 0;
}

int DecodeOBDTChunk (unsigned char * mdatpt)
{
int i, obstrsz;

	if (objstrpt != NULL) free (objstrpt);
	
	objstrpt = malloc (sizeof(OBJSTR)*MAXOBJSTR);
	memset (objstrpt, 0, sizeof(OBJSTR)*MAXOBJSTR);
	numobjstr = 1; curobj = 0;
	
	mdatpt += 8;
	i = MapGetlong (mdatpt); /* Offset to OBJSTR */
	if (i < 16) return -1;
	numobjstr = MapGetlong (mdatpt+4);
	curobj = MapGetlong (mdatpt+8);
	obstrsz = MapGetlong (mdatpt+12);
	mdatpt += i;
	
	for (i=0;i<numobjstr;i++) {
		((OBJSTR *) objstrpt)[i].xpos = MapGetlong (mdatpt);
		((OBJSTR *) objstrpt)[i].ypos = MapGetlong (mdatpt+4);
		((OBJSTR *) objstrpt)[i].gfxid = MapGetlong (mdatpt+8);
		((OBJSTR *) objstrpt)[i].tileid = MapGetlong (mdatpt+12);
		((OBJSTR *) objstrpt)[i].gxoff = MapGetlong (mdatpt+16);
		((OBJSTR *) objstrpt)[i].gyoff = MapGetlong (mdatpt+20);
		((OBJSTR *) objstrpt)[i].gwidth = MapGetlong (mdatpt+24);
		((OBJSTR *) objstrpt)[i].gheight = MapGetlong (mdatpt+28);
		((OBJSTR *) objstrpt)[i].ghandlexoff = MapGetlong (mdatpt+32);
		((OBJSTR *) objstrpt)[i].ghandleyoff = MapGetlong (mdatpt+36);
		((OBJSTR *) objstrpt)[i].show = MapGetlong (mdatpt+40);
		((OBJSTR *) objstrpt)[i].user1 = MapGetlong (mdatpt+44);
		((OBJSTR *) objstrpt)[i].user2 = MapGetlong (mdatpt+48);
		((OBJSTR *) objstrpt)[i].user3 = MapGetlong (mdatpt+52);
		((OBJSTR *) objstrpt)[i].user4 = MapGetlong (mdatpt+56);
		((OBJSTR *) objstrpt)[i].user5 = MapGetlong (mdatpt+60);
		((OBJSTR *) objstrpt)[i].user6 = MapGetlong (mdatpt+64);
		((OBJSTR *) objstrpt)[i].user7 = MapGetlong (mdatpt+68);
		((OBJSTR *) objstrpt)[i].flags = MapGetlong (mdatpt+72);
		mdatpt += obstrsz;
	}

	return 0;
}

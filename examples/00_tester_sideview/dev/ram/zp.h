// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// zp

// General, all-purpose variables

unsigned char i, pad_this_frame, gpit, gpjt, gpitu, gpaux;
signed int rds16;

const unsigned char *gp_gen, *gp_tmap, *gp_tma2;
unsigned char *gp_ram, *gp_ram_aux;
unsigned int gp_addr;
unsigned char rdx, rdy, rdt, rdit;
unsigned char rda, rdb, rdc, rdm;
unsigned char rds;
unsigned char rdct;

// Used for two-points collision

unsigned char at1, at2, cx1, cx2, cy1, cy2, caux;

// Sprite related

unsigned char oam_index;

// Enemies

unsigned char en_initial;
unsigned char en_offs;
unsigned char en_x [3], en_y [3];
unsigned char en_x1 [3], en_y1 [3];
unsigned char en_x2 [3], en_y2 [3];
signed char en_mx [3], en_my [3];
unsigned char en_t [3], en_s [3], en_facing;
unsigned char en_life [3], en_alive [3], en_status [3], en_rawv [3], en_ct [3];

#ifdef ENABLE_GENERATORS
	unsigned char en_generator_life [3], gen_was_hit [3];
#endif

unsigned char en_fr;
unsigned char en_collx, en_colly;
unsigned char en_spr;
unsigned char touched;
unsigned char en_cttouched [3];
unsigned char pregotten;

#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
	signed int enf_x [3], enf_vx [3];
#endif
#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
	signed int enf_y [3], enf_vy [3];
#endif

// Main player

signed int px, py, pvx, pvy;
unsigned char prx, pry;
unsigned char pfacing, pfr, pctfr, psprid;
unsigned char b_button;

#ifdef PLAYER_TOP_DOWN
	unsigned char pfacinghlast;
#endif

unsigned char pj, pctj, pjb, pgotten, ppossee, psprint;
unsigned char pushed_any;
unsigned char pstate, pctstate;
unsigned char phit;
signed int pgtmx, pgtmy;
#ifdef CARRY_ONE_HS_OBJ
unsigned char pinv;
#endif

#ifdef PLAYER_TOP_DOWN
	unsigned char pfacingv, pfacingh;
#endif

unsigned char wall, hitv, hith;

#ifdef ENABLE_PROPELLERS
	unsigned char ppropelled;
#endif

#if defined (PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE) || defined (FANTY_KILLED_BY_TILE)
	unsigned char pkilled;
#endif

// Bullets

#ifdef PLAYER_CAN_FIRE
	unsigned char bi;
#endif

#ifdef PLAYER_GARGAJO
	unsigned char gst [MAX_GARGAJOS], gx [MAX_GARGAJOS], gy [MAX_GARGAJOS];
	signed char gmx [MAX_GARGAJOS];
	unsigned char ghsx, ghsy, pgargajocounter;
#endif

// Hotspots
unsigned char hrx, hry, hrt;

// Process breakable?
#ifdef BREAKABLE_ANIM
	unsigned char do_process_breakable;
#endif

unsigned char ft;

// Current level

const unsigned char *c_ts_pals;
const unsigned char *c_ts_tmaps;
const unsigned char *c_behs;

const unsigned char * const *spr_enems;